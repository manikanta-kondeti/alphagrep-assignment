/*
    Program: onTheFlySort 
    Author: Manikanta 
*/
#include <stdio.h>
#include <string.h>    
#include <stdlib.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>  
#include <pthread.h> 
#include "hashmap.h"

//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8095 );

    //Bind
    if( ::bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    cout << "Run ./client in a new termial " << endl;

    //Accept and incoming connection
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = (int *)malloc(sizeof * new_sock);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL); // was commented before
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    cout << "In connection handler " << endl;
  
    HashMap * hashmap = new HashMap();

    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char message[100];
    strcpy(message, "Status from server - Sucess");
    void *k;
    //Receive a message from client
    char client_message[256];
    while( (read_size = recv(sock , client_message , 25 , 0)) > 0 )
    {
        //Send the message back to client
        write(sock , message , strlen(message));
        printf("%s\n", client_message);
       
        // Extracting info
        int exchange_number;
        long long int stock_number; 

        string temp;
        for(auto i: client_message) {
            if(i == ' '){
                exchange_number = stoi(temp);
                temp = "";
            }
            temp += i;
        }
        stock_number = stoi(temp);
        cout << "ex no: " << exchange_number << " stock num " << stock_number << endl;
        
        hashmap->put(exchange_number, stock_number);
        memset(client_message, 0, 256);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);
    close(sock);
    pthread_exit(NULL); 
    return k;
}
