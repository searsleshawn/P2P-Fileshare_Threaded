/* 
 * CSCI3240: Project Final
 * Le'Shawn Sears
 * Due: Apr 30, 2023
 */
#include "csapp.h"
#include "string.h"

void p2p(int connfd){
    char buf[MAXLINE];  //string of student information
    
    FILE *fp;  //file pointer fp
    char option[MAXLINE], peerFile[MAXLINE],peerPath[MAXLINE],
         peerPort[MAXLINE], peerIP[MAXLINE],  temp[MAXLINE];
    rio_t rio;
    int f=0, cnt=1;  //flag
    char* strptr;
    
    while(1){
        Rio_readinitb(&rio, connfd);  //intialize client input
        Rio_readlineb(&rio, buf, MAXLINE);  //read string from client
        strtok(buf,"\n");
        strcpy(option, &buf[strlen(buf)-1]);
        buf[strlen(buf)-1]='\0';
        
        //perform function based on option choice
        if(strcmp(option,"1")==0){  //begin option choice 1
            // puts("in option 1!");
            fp = fopen("filelist.txt", "a+");  //open studentRecords.txt
            if (fp == NULL){  //check if file is opened using file pointer fp
                printf("File does not exist");  //error msg if file is not opened
                exit(1);  //exit program if error occurs
            }
            //break down buf and store it into appropriate variables for storage of information
            strptr = strtok(buf, " ");  //info stored separated by spaces
            while(strptr !=NULL){       //loop to store all of buf 
                switch(cnt){  //switch case to handle format {file.(*extension) ./path/../ Port and IP} 
                    case 1:
                        strcat(peerFile, strptr);
                        break;
                    case 2:
                        strcat(peerPath, strptr);
                        break;
                    case 3:
                        strcat(peerPort, strptr);
                        break;
                    case 4:
                        strcat(peerIP, strptr);
                        strcat(peerIP, " ");
                        break;
                }//end case statement
                cnt+=1;  //change to next case
                strptr = strtok(NULL, " ");  //clear strptr and take next token
            }//end while loop
            if(strcmp(&peerPath[strlen(peerPath)-1],"/")!=0)
                            strcat(peerPath, "/");
            fprintf(fp,"%s %s%s %s %s\n", peerFile, peerPath, peerFile, peerPort, peerIP);  //print buffer string to file
            fclose(fp);  //close file
            cnt=1;
            Rio_writen(connfd, "File published!!\n", 18);
        }//end option 1
        
        if(strcmp(option,"2")==0){  //begin option choice 2
            fp = fopen("filelist.txt", "rt");  //open studentRecords.txt
            if (fp == NULL){  //check if file is opened using file pointer fp
                printf("File does not exist");  //error msg if file is not opened
                exit(1);  //exit program if error occurs
            }//end file check
            strncat(peerFile, buf, strlen(buf));  //store buf into temp
            bzero(buf,strlen(buf));  //emppty buf
            while(fgets(temp,MAXLINE, fp) != NULL) {  //get line from file and store in c
                strptr = strstr(temp,peerFile);
                if(strptr){  //compare both firstname and lastname strings passed to the function
                    f=1;  //flag
                    strcat(buf,strptr);  //add string to buf for client
                    buf[strlen(buf)-1]=':';  //replace newline with space
                    }//end if-statement
                }//end while
            if(f==0){  //if no matches were found
                bzero(buf,MAXLINE);  //make sure nothing is in buf
                strcat(buf,"n File not published!! ");  //send failure message
            }//end flag if
            buf[strlen(buf)-1]='\n';  //replace last char in buf with newline char
            Rio_writen(connfd, buf, MAXLINE);  //write buf string to client
            fclose(fp);  //close file
            bzero(temp,MAXLINE); //reset temp
            f=0;  //reset f flag
        }//end option 2
        
        if(strcmp(option,"3")==0){  //begin option choice 3
             
         }//end option 3
        
        bzero(peerFile,MAXLINE);  //reset peerFile
        bzero(peerPath,MAXLINE);  //reset peerPath
        bzero(peerPort,MAXLINE);  //reset peerPort
        bzero(peerIP,MAXLINE);  //reset peerIP
        bzero(option,MAXLINE);  //reset option
        bzero(buf,MAXLINE);  //reset buffer 
        }// end of main while
}//end of p2p function