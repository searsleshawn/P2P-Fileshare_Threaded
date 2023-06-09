Le'Shawn Sears
Final Project
CSCI 3240-001
PEER-TO-PEER FILESHARE
-------------------------------------SERVER-----------------------------------------------------------------------
1. server.c : This program creates a threaded server. Taking one commandline input argument, its PORT.
Clients will use this server for publishing and searching files.

The main operations of this program:
JOIN : Server will be connected to any requesting clients
PUBLISH : Clients can publish file to server for sharing this is provided by the client giving a filename with extension, 
the file's path and the client's Port and IP are appended as well, for other clients to connect to the file through the 
publishing client.
SEARCH : Clients can search file provided giving a file name. The server searches through filelist.txt of the stored information
and returns the information to the client.

2. p2p.c : This program is used by server.c to perform the operations for searching and publishing files to the filelist.txt
it contains all important server functionality.

3. filelist.txt : This file contains the list of files published by the peers. This file is exclusively maintained by server. It contains
all file information as {file.* ./path/../<file.*> PORT IP}, the file name is stored at the front for searching purposes and is not
returned to the client.

------------------------------CLIENT-------------------------------------------------------------------------------------
1. client.c : This program creates a peer. File takes three commandline input arguments host name,server port and IP,
for peer connections. The client uses its host and port to connect to the server for publishing and search files.
The client's listening port will be used in the background to listen and accept any incoming fetch requests made by peers.

The main operations for this program:
JOIN : Connect to the server
PUBLISH : Publish file to server for sharing with file name and path with the client's port and IP are appended to this information.
SEARCH : Search file name on the server
FETCH :Fetch desired file from peer with file path, port and IP. The program also handles incoming file fetch requests from other peers in the background.

2. media folder : Conatins the file 'zombie.png' which the client can upload or download.
------------------------------COMPILATION-------------------------------------------------------------------------------------------
Contents of makefile:
CC=gcc
CFLAGS=-g -O1 -Wall
LDLIBS=-lpthread

all: client server 

client: client.c csapp.h csapp.c
server: server.c p2p.c csapp.h csapp.c

clean:
    rm -f *.o *~ *.exe client server csapp.o

usage: make clean; make
------------------------------------------------------------------------------------------------------------------------------------
SERVER INSTRUCTIONS:

1. Execute the server executable: ./server <port>
---------------------------------------------------------------------------------------------------------------------------------------
CLIENT INSTRUCTIONS:

1. Execute the client executable: ./client <host name> <server port> <client IP>
2. Follow prompts provided for functionality: publish,search, fetch and terminate.
        Option 1: Publish a file. Format: file name with extension and file path
        Option 2: Search for file. Format: Filename with extension
        Option 3: Fetch a file. Format: file path, peer's port and IP 
        Option 4: Terminate the program and close connection to server.
 ------------------------------------------------------------------------------------------------------------------------------------
 
