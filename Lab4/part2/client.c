#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

#define PORT 3232 /* default port number */
#define MAXDATALEN 256

int sockfd;
int n, x;                     /*variables for socket*/
struct sockaddr_in serv_addr; /* structure to hold server's address */
char buffer[MAXDATALEN];
char buf[10];

void *quit();
void *chat_write(int);
void *chat_read(int);

/***************main starts************/

int main(int argc, char *argv[])
{

    pthread_t thr1, thr2; /* variable to hold thread ID */

    if (argc != 2)
    {
        printf("help:u need to put server ip\n");
        exit(0);
    }

    /*=============socket creating==============*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        printf("client socket error\n");
    else
        printf("socket\t\tcreated\n");

    /*===============set info===================*/
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    /*=========for username=====================*/
    bzero(buf, 10);
    printf("\nENTER YOUR NAME: ");
    fgets(buf, 10, stdin);
    __fpurge(stdin);
    buf[strlen(buf) - 1] = ':';

    /*=============client connect to server============*/
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("client connect error\n");
        exit(0);
    }
    else
        printf("%s connected to server\n", buf);

    printf("\rYOU JOINED AS- %s", buffer - 1);
    send(sockfd, buf, strlen(buf), 0);

    pthread_create(&thr2, NULL, (void *)chat_write, (void *)sockfd); //thread for writing
    pthread_create(&thr1, NULL, (void *)chat_read, (void *)sockfd);  //thread for reading

    pthread_join(thr2, NULL);
    pthread_join(thr1, NULL);

    return 0;

}

/*======reading continously from socket=============*/
void *chat_read(int sockfd)
{
    signal(SIGINT,(void *)quit);
    while (1)
    {
        n = recv(sockfd, buffer, MAXDATALEN - 1, 0);
        if (n == 0)
        {
            printf("\nSERVER HAS BEEN SHUTDOWN\n");
            exit(0);
        }

        if (n > 0)
        {
            printf("\n%s ", buffer);
            bzero(buffer, MAXDATALEN);
        }
    }
}

/*======writing continously to socket=============*/
void *chat_write(int sockfd)
{
    while (1)
    {
        // printf("%s", buf);
        fgets(buffer, MAXDATALEN - 1, stdin);

        if (strlen(buffer) - 1 > sizeof(buffer))
        {
            printf("buffer size full\t enter within %d characters\n", sizeof(buffer));
            bzero(buffer, MAXDATALEN);
            __fpurge(stdin);
        }

        n = send(sockfd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "/quit", 5) == 0)
            exit(0);

        bzero(buffer, MAXDATALEN);
    } //while ends
}

/*======handling signals==========*/
void *quit()
{ //handling ctrl+c
    printf("\nType '/quit' TO EXIT\n");
}
