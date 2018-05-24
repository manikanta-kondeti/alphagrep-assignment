/*
    Program: onTheFlySort 
    Author: Manikanta 
*/
#include <stdio.h> 
#include <string.h>    
#include <sys/socket.h>    
#include <arpa/inet.h> 
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
     char server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8095 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    int exchange_number;
    cout << "input an exchange number :" << endl;
    cin >> exchange_number;
    string client_info = "";
    long long int stock_number = 10; 
    //keep communicating with server
    while(1)
    {
        stock_number--;
        char *message = (char *)malloc(sizeof(char));
        free(message);
       // printf( "Type 'q' to end the session:\n");
        client_info += to_string(exchange_number) + " " + to_string(stock_number); 
    
        cout << "c_str = " << client_info.c_str() << endl;
        strcpy(message, client_info.c_str());
        client_info.clear();

        if(stock_number == -1)
        {
            //close(sock);
             return 0;
        }

        else{ 

        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        this_thread::sleep_for(chrono::seconds(2));
        puts("Server reply : ");
        puts(server_reply);
        }
    }

    //close(sock);
    return 0;
}
