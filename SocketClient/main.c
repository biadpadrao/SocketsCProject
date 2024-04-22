#include "../SocketUtil/socketutil.h"
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

void startListeningAndPrintMessagesOnNewThread(int fd);

void listenAndPrint(int socketFD);

int main()
{
    int socketFD = createTCPIpv4Socket();

    struct sockaddr_in *address = createIPv4Address("54.205.129.69", 2000);

    int result = connect(socketFD, (struct sockaddr *)address, sizeof(*address));

    if (result == 0)
    {
        printf("Connection was succesful!\n");
    }

    char *line = NULL;
    size_t lineSize = 0;
    printf("type and we will send(type exit)...\n");

    startListeningAndPrintMessagesOnNewThread(socketFD);

    while (true)
    {
        ssize_t charCount = getline(&line, &lineSize, stdin);

        if (charCount > 0)
        {
            if (strcmp(line, "exit\n") == 0)
            {
                break;
            }
            else
            {
                ssize_t amountWasSent = send(socketFD, line, charCount, 0);
            }
        }
    }

    close(socketFD);

    return 0;
}

void startListeningAndPrintMessagesOnNewThread(int socketFD)
{
    pthread_t id;
    pthread_create(&id, NULL, listenAndPrint, socketFD);
}

void listenAndPrint(int socketFD)
{
    char buffer[1024];
    while (true)
    {
        ssize_t amountReceived = recv(socketFD, buffer, 1024, 0);
        if (amountReceived > 0)
        {
            buffer[amountReceived] = 0;
            printf("Response was %s\n", buffer);
        }
        if (amountReceived < 0)
            break;
    }
    close(socketFD);
}
