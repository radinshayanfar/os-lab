/* to compile me in Linux, type:   gcc -o server t1.c -lpthread */
/* t1.c - code for  server program that uses TCP/IP */

/****SERVER CODE****/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG 100    // connections in the queue
#define MAXDATALEN 256 //max size of messages to be sent
#define PORT 3232      //default port number

struct Node
{
    int port;
    char username[10];
    struct Node *next;
};

typedef struct Node *ptrtonode;
typedef ptrtonode head;
typedef ptrtonode addr;

void sendtoall(char *, int new_fd);            /*send chat msgs to all connected clients*/
void Quitall();                                /*send msg to all if server shuts down*/
head MakeEmpty(head h);                        /*clearing list*/
void Delete(int port, head h);                 /*delete client values on client exit*/
void Insert(int port, char *, head h, addr a); /*inserting new client */
void DeleteList(head h);                       /*clearing list*/
void Display(const head h);                    /*list all clients connected*/
void *Quitproc();                              /*signal handler*/
void *server(void *arg);                       /*server instance for every connected client*/
void zzz();

char username[10];
int sf2;
head h;
char buffer[MAXDATALEN];

int main(int argc, char *argv[])
{

    int sockfd, new_fd;
    int portnum;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int cli_size, z;
    pthread_t thr;
    int yes = 1;
    addr a;

    printf("==== Starting Server ====\n");
    if (argc == 2)
        portnum = atoi(argv[1]);
    else
        portnum = PORT; //if port number not given as argument then using default port
    printf("PORT NO.:\t%d\n", portnum);

    h = MakeEmpty(NULL); //frees the list

    /*=set info of server =*/
    server_addr.sin_family = AF_INET;                /* set family to Internet     */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* set IP address */
    server_addr.sin_port = htons(portnum);
    printf("IP ADDRESS:\t%s\n", inet_ntoa(server_addr.sin_addr));

    /*=creating socket=*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("server- socket() error"); // debugging
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        printf("setsockopt error"); // debugging
        exit(1);
    }

    /*=binding socket=*/
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("binding failed\n"); // debugging
        exit(1);
    }

    printf("==== PRESS CTRL+z TO VIEW ONLINE CLIENTS ====\n");
    /*=socket on listening mode=*/
    listen(sockfd, BACKLOG);
    printf("waiting for clients......\n");

    signal(SIGINT, (void *)Quitproc); //signal handler
    signal(SIGTSTP, zzz);             //signal handler

    // Accepting new clients
    while (1)
    {
        cli_size = sizeof(struct sockaddr_in);                               //cli_size necessary as an argument for pthread_create
        new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &cli_size); //accepting connection from client
        a = h;

        /* getting username */
        bzero(username, 10);
        if (recv(new_fd, username, sizeof(username), 0) > 0)
            ;
        username[strlen(username) - 1] = ':';

        printf("** %d: %s JOINED CHAT **\n", new_fd, username);
        Insert(new_fd, username, h, a); //inserting newly accepted client socked fd in list
        a = a->next;

        struct Node args; //struct to pass multiple arguments to server function
        args.port = new_fd;
        strcpy(args.username, username);

        pthread_create(&thr, NULL, server, (void *)&args); //creating thread for every client connected
        pthread_detach(thr);
    }

    DeleteList(h); //deleting all clients when server closes
    close(sockfd);

}

/* Each client thread */
void *server(void *arguments)
{
    struct Node *args = arguments;

    char buffer[MAXDATALEN], ubuf[50], uname[10]; /* buffer for string the server sends */
    char *strp;
    char *msg = (char *)malloc(MAXDATALEN);
    int ts_fd, x, y;
    int sfd, msglen;

    ts_fd = args->port; /*socket variable passed as arg*/
    strcpy(uname, args->username);
    addr a;

    // Handling messages
    while (1)
    {

        bzero(buffer, 256);
        y = recv(ts_fd, buffer, MAXDATALEN, 0);

        /* Client quits */
        if (!y || strncmp(buffer, "/quit", 5) == 0)
        {
            printf("%d ->%s left chat deleting from list\n", ts_fd, uname);
            sprintf(buffer, "%s has left the chat\n", uname);

            addr a = h;
            do
            {
                a = a->next;
                sfd = a->port;
                if (sfd == ts_fd)
                    Delete(sfd, h);
            } while (a->next != NULL);

            Display(h);

            close(ts_fd);
            free(msg);

            break;
        }

        /* Send to all */
        printf("%s %s\n", uname, buffer);
        strcpy(msg, uname);
        x = strlen(msg);
        strp = msg;
        strp += x;
        strcat(strp, buffer);
        msglen = strlen(msg);

        addr a = h;
        do
        {
            a = a->next;
            sfd = a->port;
            if (sfd != ts_fd)
                send(sfd, msg, msglen, 0);

        } while (a->next != NULL);

        Display(h);
        bzero(msg, MAXDATALEN);

    }
    return 0;

}

/*=====empties and deletes the list======*/
head MakeEmpty(head h)
{
    if (h != NULL)
        DeleteList(h);
    h = malloc(sizeof(struct Node));
    if (h == NULL)
        printf("Out of memory!");
    h->next = NULL;
    return h;
}
/*======delete list=======*/
void DeleteList(head h)
{
    addr a, Tmp;
    a = h->next;
    h->next = NULL;
    while (a != NULL)
    {
        Tmp = a->next;
        free(a);
        a = Tmp;
    }
}
/*===============inserting new clients to list==========*/
void Insert(int port, char *username, head h, addr a)
{
    addr TmpCell;
    TmpCell = malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        printf("Out of space!!!");
    TmpCell->port = port;
    strcpy(TmpCell->username, username);
    TmpCell->next = a->next;
    a->next = TmpCell;
}

/*========displaying all clients in list==================*/
void Display(const head h)
{
    addr a = h;
    if (h->next == NULL)
        printf("NO ONLINE  CLIENTS\n");

    else
    {
        do
        {
            a = a->next;
            printf("%d->%s \t", a->port, a->username);
        } while (a->next != NULL);
        printf("\n");
    }
}

/*===========client deleted from list if client quits================*/
void Delete(int port, head h)
{
    addr a, TmpCell;
    a = h;
    while (a->next != NULL && a->next->port != port)
        a = a->next;
    if (a->next != NULL)
    {
        TmpCell = a->next;
        a->next = TmpCell->next;
        free(TmpCell);
    }
}

/*======handling signals==========*/
void *Quitproc()
{
    printf("\n\nSERVER SHUTDOWN\n");
    Quitall();
    exit(0);
}

/*===============notifying server shutdown===========*/
void Quitall()
{
    int sfd;
    addr a = h;
    int i = 0;
    if (h->next == NULL)
    {
        printf("......BYE.....\nno clients \n\n");
        exit(0);
    }
    else
    {

        do
        {
            i++;
            a = a->next;
            sfd = a->port;
            send(sfd, "server down", 13, 0);
        } while (a->next != NULL);
        printf("%d clients closed\n\n", i);
    }
}

void zzz()
{
    printf("\rDISPLAYING ONLINE CLIENTS\n\n");
    Display(h);
}

/*=========================================================================================================*/