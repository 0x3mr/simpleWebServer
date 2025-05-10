#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h> // string manipulation
#include <netdb.h>  // getnameinfo

#include <sys/socket.h> // socket APIs
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // open, close

#include <signal.h> // signal handling
#include <time.h>   // time

#define PORT 8080
#define BACKLOG 5

int error(int errorCode)
{
    if (errorCode == 1)
    {
        printf("The server is not bound to the address.");
        return 1;
    }
    else if (errorCode == 2)
    {
        printf("The server is not listening.");
        return 1;
    }

    return 0;
}

int main()
{
    int socket_fd;
    struct sockaddr_in serverAddress;
    
    serverAddress.sin_family = AF_INET; // AF_NET - Used to indicate the IPv4 address family
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    if (bind(socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) return error(1);

    if (listen(socket_fd, BACKLOG) < 0) return error(2);

    char hostBuffer[NI_MAXHOST], serviceBuffer[NI_MAXSERV];

    int errorCheck = getnameinfo(
        (struct sockaddr*)&serverAddress,
        sizeof(serverAddress),
        hostBuffer,
        sizeof(hostBuffer),
        serviceBuffer,
        sizeof(serviceBuffer),
        0
    );

    if (errorCheck != 0)
    {
        printf("%s\n", gai_strerror(errorCheck));
        return 1;
    }

    printf("\nServer is listening on http://%s:%s/\n\n", hostBuffer, serviceBuffer);

    return 0;
}