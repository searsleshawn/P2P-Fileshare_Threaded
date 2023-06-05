/* 
 * CSCI3240: Project Final
 * Le'Shawn Sears
 * Due: Apr 30, 2023
 */
/*
client.c -  client creates peer to publish and listen to incoming 
            fetch requests from other peers.
            1. connect to peer server
            2. client performs publish, search, or fetch operation
            Publish: this will publish a file to server with the filename, filepath, and client port
            Search:  this allows the client to for a file using keyword from server
            Fetch:   this will fetch the desired file from peer or for incoming clients
                     the operation requires using the client IP, Port, filename and path
*/
//begin client main
#include "csapp.h"
#include "string.h"

int main(int argc, char **argv){
    int listenfd, clientfd, pid, cnt=1;  //client port and client
    char *host, *port, *peer, *fileshare, option[2], buf[MAXLINE], serverOut[MAXLINE], fetchPath[MAXLINE], fetchIP[MAXLINE], fetchPort[MAXLINE];
    rio_t rio;
    socklen_t peerlen;  //peer socket
    struct sockaddr_storage peeraddr;
    
    if (argc != 4){  //check if host and port information have been given
    fprintf(stderr, "usage: %s <host> <port> <peer>\n", argv[0]);  //display format that host and port info are needed
    exit(0);  //exit program since no host or port are given 
    }
    host = argv[1];  //set command line arg 1 as host
    port = argv[2];  //set command line arg 2 as port
    peer = argv[3];  //set command line arg 3 as peer

    clientfd = Open_clientfd(host, port);  //attempt connection to host
    pid=fork();  //background listen process
    
    if(!pid){
        while(1){
            listenfd = Open_listenfd(peer);  //listen for peer port
            char path[MAXLINE], peerPort[MAXLINE], peerIP[MAXLINE];
            peerlen = sizeof(struct sockaddr_storage); //prepare socket connection
            clientfd = Accept(listenfd, (SA *)&peeraddr, &peerlen); //accept socket
            Getnameinfo((SA *) &peeraddr, peerlen, peerPort, MAXLINE, peerIP, MAXLINE,0); //peer connection info
            printf("Peer connected to (%s, %s)\n",peerPort ,peerIP);  //peer connection message
            
            Rio_readinitb(&rio, clientfd);  //intialize client input
            Rio_readlineb(&rio, buf, MAXLINE);  //read string from client
            strcat(path, buf);  //place buf into path
            bzero(buf, MAXLINE);
            strtok(path, "\n");
            FILE *fp = fopen(path, "rt");
            if (fp == NULL){  //check if file is opened using file pointer fp
                printf("File does not exist");  //error msg if file is not opened
                exit(1);  //exit program if error occurs
            }//end file check
            int fbyte;  //bytes recieved
            while((fbyte = fread(buf, 1, MAXLINE, fp))>0)
                Rio_writen(clientfd, buf, fbyte);
            bzero(buf,MAXLINE);
            bzero(path, MAXLINE);
            Close(clientfd);
            exit(0);
        }//end peer connection 
    }//end pid if

    Rio_readinitb(&rio, clientfd);
    printf("(1) Publish/Upload a file\n(2) File search\n(3) Fetch a file\n(4) Terminate\nSelect an option [1, 2, 3 or 4]: ");    //print options for user and take user option choice
 
    while(Fgets(option, MAXLINE, stdin)!=NULL){  //upon connection, provide user options
        // Rio_writen(clientfd, option, MAXLINE);  //write option string to server
        strtok(option, "\n");  //clearn option input
        //debugging buf input
        // puts(option);
        
        //option choice 1 Publish a file
        if(strcmp(option,"1")==0){
            printf("Publish format {file.* ./path/../*}\nPlease enter: ");  //prompt to publish
            Fgets(buf, MAXLINE, stdin);  //get publish string from client
            buf[strlen(buf)-1]=' ';
            strcat(buf, port);
            strcat(buf, " ");
            strcat(buf, peer);
            strcat(buf, " ");
            strcat(buf, option);
            strcat(buf, "\n");
            //debugging buf input
            // puts(buf);
            
            //write buf string to server
            Rio_writen(clientfd, buf, strlen(buf));
            puts("Server response:");
            
            //get string from server
            bzero(serverOut,MAXLINE);
            Rio_readinitb(&rio, clientfd);  //prepare read
            Rio_readlineb(&rio, serverOut, MAXLINE);  //prepare to read message from server
            Fputs(serverOut, stdout);  //print msg from server
           }//end of option 1
        
        //option 2 Search for file
        else if(strcmp(option,"2")==0){
            printf("Enter a file name to search: ");
            Fgets(buf, MAXLINE, stdin);  //get search string from client
            buf[strlen(buf)-1]=' ';
            strcat(buf, option);
            strcat(buf, "\n");
            //debugging buf input
            // puts(buf);
            
            //write buf string to server
            Rio_writen(clientfd, buf, strlen(buf));
            puts("Server response:");
            
            //get string from server
            bzero(serverOut,MAXLINE);
            Rio_readinitb(&rio, clientfd);  //prepare read
            Rio_readlineb(&rio, serverOut, MAXLINE);  //prepare to read message from server
            // Fputs(serverOut, stdout);  //print msg from server
            
            //clean output of fileshare
            fileshare = strtok(serverOut," ");  //extract fileshare token from serverOut
            fileshare = strtok(NULL,":");
            while(fileshare!=NULL){  //loop through string to extract all fileshare tokens
                printf("%s\n",fileshare);  //print the fileshare token
                fileshare=strtok(NULL,":");
                // fileshare=strtok(NULL,"=");
            } 
        }//end of option 2
        
        //option 3 Fetch file
        else if(strcmp(option,"3")==0){
            printf("Fetch format {./dir/<file.*> peerPort peerIP}\nPlease enter: ");
            Fgets(buf, MAXLINE, stdin);  //get publish string from client
            fileshare = strtok(buf," ");
            while(fileshare!=NULL){
                switch(cnt){
                    case 1:
                        strcat(fetchPath, fileshare);
                        break;
                    case 2:
                        strcat(fetchPort, fileshare);
                        break;
                    case 3:
                        strcat(fetchIP, fileshare);
                        strtok(fetchIP, "\n");
                        break;
                }//end switch case
                cnt+=1;
                fileshare = strtok(NULL," ");
            }
            // puts(fetchPath);
            // puts(fetchPort);
            // puts(fetchIP);
            cnt+=1;
            fileshare = strtok(NULL," ");
            // puts(buf);  break;  //debugging
            
            clientfd = Open_clientfd(fetchPort, fetchIP);  //attempt connection to host
            buf[strlen(buf)-1]=' ';
            strcat(buf, fetchPath);
            strcat(buf, "\n");
            
            //debugging buf input buf does not reach here after connecting to other client
            // puts(buf);
            // break;

            //write buf string to server
            Rio_writen(clientfd, buf, strlen(buf));
            puts("Server response:");
            
            //get string from server
            bzero(serverOut,MAXLINE);
            Rio_readinitb(&rio, clientfd);  //prepare read
            Rio_readlineb(&rio, serverOut, MAXLINE);  //prepare to read message from server
            Fputs(serverOut, stdout);  //print msg from server   
        }//end of option 3
        
        //option choice 4 terminate program
        else if(strcmp(option,"4")==0){  //if option 3 is given
            printf("Connection Closed!!\n");
            break;
        }//terminate program option 4 end
        bzero(buf,MAXLINE);
        printf("(1) Publish/Upload a file\n(2) File search\n(3) Fetch a file\n(4) Terminate\nSelect an option [1, 2, 3 or 4]: ");    //print options for user and take user option choice
    }
    Close(clientfd); //if while is not properly entered close connection
    exit(1);  //end program
}
/* $end clientmain */