#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>

#include <parse.h>
#include <load.h>
#include <serialize.h>

#define CAN_NOT_WRITE_CHARS 1
#define BUFFER_OVERFLOW 2

#define MAX_MSG_LEN 1024

volatile sig_atomic_t running = 1;

void handleSignal(int sig) {
    running = 0;
}

int syncPrint(const char* format, ...) {
    va_list list;
    va_start(list, format);
    const int result = vprintf(format, list);
    va_end(list);
    return result;
}

typedef struct {
    int socket;
    struct addrinfo* info;
} SocketInfo;

SocketInfo tryOpenSocket(struct addrinfo* info) {
    for (struct addrinfo* p = info; p != 0; p = p->ai_next) {
        int socketFD = socket(info->ai_family, p->ai_socktype, 0);
        if (socketFD != -1) {
            SocketInfo result;
            result.socket = socketFD;
            result.info = p;
            return result;
        }
    }
    SocketInfo result;
    result.socket = -1;
    result.info = NULL;
    return result;
}

pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;

typedef struct {
    CpuLoad* load;
} SyncData;

SyncData* allocSyncData() {
    return (SyncData*)malloc(sizeof(SyncData));
}

void* netTreadMain(void* param) {
    SyncData* syncData = (SyncData*)param;
    char msg[MAX_MSG_LEN] = {};

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family =  AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo* result;
    int status;
    if ((status = getaddrinfo("127.0.0.1", "1234", &hints, &result))) {
        syncPrint("getaddrinfo failed: %s\n", gai_strerror(status));
    }
    syncPrint("getaddrinfo succeed\n");

    SocketInfo info = tryOpenSocket(result);

    if (info.socket == -1) {
        syncPrint("Error: cannot open socket\n");
    }
    syncPrint("Soocket openning succeed\n");

    while (running) {
        pthread_mutex_lock(&mutex);
        while (syncData->load == NULL) {
            pthread_cond_wait(&condVar, &mutex);
        }
        CpuLoad* load = syncData->load;
        syncData->load = NULL;
        pthread_mutex_unlock(&mutex);

        const int msgLen = serializeCpuLoadTo(load, msg, MAX_MSG_LEN);
        const int sent = sendto(
                info.socket,
                msg,
                msgLen,
                0,
                info.info->ai_addr,
                info.info->ai_addrlen
        );

        free(load);
    }

    close(info.socket);
    freeaddrinfo(result);
    return NULL;
}

int main() {
    int result = 0;
    void* threadResult;

    signal(SIGINT, handleSignal);

    pthread_mutex_init(&mutex, NULL);

    SyncData* syncData = allocSyncData();

    pthread_t netThread;
    pthread_create(&netThread, NULL, netTreadMain, syncData);

    CpuInfo first;
    if (readProcStat(&first) != 0) {
        result = 1;
        goto clearup;
    }
    CpuInfo second;

    CpuInfo* current = &second;
    CpuInfo* previous = &first;

    while (running) {
        sleep(1);
        if (readProcStat(current) != 0) {
            result = 1;
            goto clearup;
        }

        CpuLoad* load = (CpuLoad*)malloc(sizeof(CpuLoad));
        *load = calculateCpuLoad(previous, current);

        while (1) {
            pthread_mutex_lock(&mutex);
            if (syncData->load) {
                pthread_cond_signal(&condVar);
                pthread_mutex_unlock(&mutex);
                continue;
            }
            syncData->load = load;
            pthread_cond_signal(&condVar);
            pthread_mutex_unlock(&mutex);
            break;
        }

        CpuInfo* tmp = current;
        current = previous;
        previous = tmp;
    }

clearup:
    printf("before join\n");
    pthread_join(netThread, &threadResult);

    free(syncData);

    pthread_mutex_destroy(&mutex);

    return 0;
}
