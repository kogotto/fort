#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <string.h>
#include <stdarg.h>

#include <parse.h>
#include <load.h>

#define CAN_NOT_WRITE_CHARS 1
#define BUFFER_OVERFLOW 2

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

void* netTreadMain(void* param) {
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

    close(info.socket);
    freeaddrinfo(result);
    return NULL;
}

int main() {
    CpuInfo first;
    switch (readProcStat(&first)) {
    case 0:
        printCpuInfo(&first);
        break;
    default:
        printf("failure\n");
        break;
    }
    sleep(1);
    CpuInfo second;
    switch (readProcStat(&second)) {
    case 0:
        printCpuInfo(&second);
        break;
    default:
        printf("failure\n");
        break;
    }

    CpuLoad load = calculateCpuLoad(&first, &second);
    printf("%.2f", load.all);
    for (int i = 0; i < load.coreCount; ++i) {
        printf(" %.2f", load.cores[i]);
    }
    printf("\n");

    pthread_t netThread;
    pthread_create(&netThread, NULL, netTreadMain, NULL);
    void* res;
    pthread_join(netThread, &res);
    return 0;
}
