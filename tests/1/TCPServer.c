// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd;
    int new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // char *hello = "Hello from server";
    
    // Only this line has been changed. Everything is same.
    // char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 24\n\n<h1>Hello worldzer!</h1>";
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 44\n\n<html><body><h1>It works!</h1></body></html>";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    // 1
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        // 2
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }    
        printf("%i\n", new_socket);
        char buffer[30000] = {0};
        // 3
        valread = read( new_socket , buffer, 30000);
        printf("buffer:\n%s\n",buffer );
        // 4
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        // 5
        close(new_socket);
    }
    return 0;
}