/* 
 * CSCI3240: Project Final
 * Le'Shawn Sears
 * Due: Apr 30, 2023
 */
/*
server.c -  clients use server for publishing and searching files.
            Publish:    clients file to server for sharing with filename, filepath,
                        port, which are stored in a publish list (fileshare.txt)
            Search:     clients use this to search for files, this uses a bash script (search.sh),
                        used to search for files stored in the publish list.
                       **After search is completed, client is disconnected to reduce server load**

 $begin server main */
#include "csapp.h"

void p2p(int connfd);
void *thread(void *vargp);
typedef struct{  //Student structure
    char client_hostname[MAXLINE];  //client host name 
    char client_port[MAXLINE];  //client port
    int* connfd;
}client;  //client structure

int main(int argc, char **argv) 
{
    int listenfd, *connfdp;
    socklen_t clientlen;  //socket
    client c;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */
    pthread_t tid;  //thread tid
    
    if (argc != 2) {  //check if server port established
        fprintf(stderr, "usage: %s <port>\n", argv[0]);  //display format to establish port
        exit(0);  //end server program
    }
    
    listenfd = Open_listenfd(argv[1]);  //listen for client port
    while (1) {  //while client port connection found
        clientlen = sizeof(struct sockaddr_storage);  //prepare socket connection
        connfdp=Malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);  //accept connection
        Getnameinfo((SA *) &clientaddr, clientlen, c.client_hostname, MAXLINE,c.client_port, MAXLINE, 0);  //client connection info
        Pthread_create(&tid, NULL, thread, connfdp);
        printf("Connected to (%s, %s)\n", c.client_hostname, c.client_port);  //print client's host and port  //successful disconnection message
        }
}
/* $end server main */

/*Thread BEGIN*/
void *thread(void *vargp){
    //client c;
    int connfd=*((int*)vargp);
    Pthread_detach(pthread_self());
    Free(vargp);
    p2p(connfd);
    Close(connfd);
    return NULL;
}