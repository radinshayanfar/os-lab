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
#define MAXUSER 20     //max size of messages to be sent
#define MAXGROUP 10    //max size of messages to be sent
#define PORT 3232      //default port number

typedef struct
{
    int port;
    char username[10];
} User;

void sendtoall(char *, int new_fd); /*send chat msgs to all connected clients*/
void Quitall();                     /*send msg to all if server shuts down*/
// void Delete(int port, head h);                 /*delete client values on client exit*/
void insert_list(int port, char *username, User *list, int *tail); /*inserting new client */
int search_list(int port, User *list, int tail);
void delete_list(int port, User *list, int *tail);
void delete_all(User *list, int *tail);
void display_list(const User *list, int tail); /*list all clients connected*/
void *Quitproc();                              /*signal handler*/
void *server(void *arg);                       /*server instance for every connected client*/
void zzz();
int next_space(char *str);

char username[10];
int sf2;
User users[MAXUSER] = {0};
int user_tail = 0;
User groups[MAXGROUP][MAXUSER] = {0};
int group_tail[MAXUSER] = {0};
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

    printf("==== Starting Server ====\n");
    if (argc == 2)
        portnum = atoi(argv[1]);
    else
        portnum = PORT; //if port number not given as argument then using default port
    printf("PORT NO.:\t%d\n", portnum);

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

        /* getting username */
        bzero(username, 10);
        if (recv(new_fd, username, sizeof(username), 0) > 0)
            ;
        username[strlen(username) - 1] = ':';

        printf("** %d: %s JOINED CHAT **\n", new_fd, username);
        insert_list(new_fd, username, users, &user_tail); //inserting newly accepted client socked fd in list

        User args; //struct to pass multiple arguments to server function
        args.port = new_fd;
        strcpy(args.username, username);

        pthread_create(&thr, NULL, server, (void *)&args); //creating thread for every client connected
        pthread_detach(thr);
    }

    delete_all(users, &user_tail);
    close(sockfd);
}

/* Each client thread */
void *server(void *arguments)
{
    User *args = arguments;

    char buffer[MAXDATALEN], ubuf[50], uname[10]; /* buffer for string the server sends */
    char *strp;
    char *msg = (char *)malloc(MAXDATALEN);
    int ts_fd, x, y;
    int sfd, msglen;

    ts_fd = args->port; /*socket variable passed as arg*/
    strcpy(uname, args->username);

    // Handling messages
    while (1)
    {

        bzero(buffer, 256);
        y = recv(ts_fd, buffer, MAXDATALEN, 0);

        /* Client quits */
        if (!y || strncmp(buffer, "/quit", 5) == 0)
        {
            printf("%d ->%s left chat deleting from list\n", ts_fd, uname);

            delete_list(ts_fd, users, &user_tail);
            for (int i = 0; i < MAXGROUP; i++)
            {
                delete_list(ts_fd, groups[i], &group_tail[i]);
            }

            display_list(users, user_tail);

            close(ts_fd);
            free(msg);

            break;
        }
        else if (strncmp(buffer, "/join", 5) == 0)
        {
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strcpy(group_id_str, buffer + 6);
            int group_id = atoi(group_id_str);
            printf("%d: %s joined group number %d.\n", ts_fd, uname, group_id);

            insert_list(ts_fd, uname, groups[group_id], &group_tail[group_id]);
        }
        else if (strncmp(buffer, "/leave", 6) == 0)
        {
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strcpy(group_id_str, buffer + 7);
            int group_id = atoi(group_id_str);
            printf("%d: %s left group number %d.\n", ts_fd, uname, group_id);

            delete_list(ts_fd, groups[group_id], &group_tail[group_id]);
        }
        else if (strncmp(buffer, "/send", 5) == 0)
        {
            int space_pos = next_space(buffer + 6);
            char *group_id_str = malloc(sizeof(MAXDATALEN));
            strncpy(group_id_str, buffer + 6, space_pos);
            int group_id = atoi(group_id_str);
            printf("\nGROUP ID IS: %d\n", group_id);

            if (search_list(ts_fd, groups[group_id], group_tail[group_id]) == -1) {
                continue;
            }

            /* Send to all */
            printf("%s %s\n", uname, buffer);
            strcpy(msg, uname);
            x = strlen(msg);
            strp = msg;
            strp += x;
            strcat(strp, buffer + 7 + space_pos);
            msglen = strlen(msg);

            for (int i = 0; i < group_tail[group_id]; i++)
            {
                if (groups[group_id][i].port != ts_fd)
                    send(groups[group_id][i].port, msg, msglen, 0);
            }

            display_list(users, user_tail);
            bzero(msg, MAXDATALEN);
        }
    }
    return 0;
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
    if (user_tail == 0)
    {
        printf("......BYE.....\nno clients \n\n");
        exit(0);
    }
    else
    {
        for (int i = 0; i < user_tail; i++)
        {
            send(users[i].port, "/server_down", 14, 0);
        }

        printf("%d clients closed\n\n", user_tail + 1);
    }
}

void zzz()
{
    printf("\rDISPLAYING ONLINE CLIENTS\n\n");
    display_list(users, user_tail);
}

void insert_list(int port, char *username, User *list, int *tail)
{
    if (search_list(port, list, *tail) != -1)
    {
        return;
    }
    User *temp;
    temp = malloc(sizeof(User));
    if (temp == NULL)
        printf("Out of space!!!");
    temp->port = port;
    strcpy(temp->username, username);
    list[(*tail)++] = *temp;
}

int search_list(int port, User *list, int tail)
{
    for (int i = 0; i < tail; i++)
    {
        if (list[i].port == port)
            return i;
    }
    return -1;
}

void delete_list(int port, User *list, int *tail)
{
    int ptr = search_list(port, list, *tail);
    if (ptr == -1)
    {
        return;
    }

    for (int i = ptr; i < *tail - 1; i++)
    {
        list[i] = list[i + 1];
    }
    (*tail)--;
}

void display_list(const User *list, int tail)
{
    for (int i = 0; i < tail; i++)
    {
        printf("%d: %s\t", list[i].port, list[i].username);
    }
    printf("\n");
}

void delete_all(User *list, int *tail)
{
    *tail = 0;
}

int next_space(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            return i;
        }
        i++;
    }
    return -1;
}
