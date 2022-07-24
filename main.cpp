//
// Created by pwlqr on 16/07/2022.
//

#include <iostream>
#include "DataBase.h"
#include "BL.h"

///socket includes:
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <unistd.h> // For read

#include <WS2tcpip.h>

using namespace std;

const int RETURN_MESSAGE_LENGTH = sizeof(FileProp);
//const int RECEIVE_MESSAGE_LENGTH = 1024;

int main() {

    //initialise winsock.
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
        return 1;

    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(54000); // htons is necessary to convert a number to
    // network byte order
    if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 54000. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 10 connections in the queue
    if (listen(sockfd, SOMAXCONN) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Grab a connection from the queue
    sockaddr_in client;
    int clientSize = sizeof(client);

    int connection = accept(sockfd, (struct sockaddr *) &client, &clientSize);
    if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (getnameinfo((struct sockaddr *) &client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }


    // session with client... creating the BL.
    BL bl;

    //send information about the database...
    cout << "hand-shaking client..." << endl;
    int size;
    FileProp *databaseInfo = bl.dataBaseState(size);
    send(connection, (char *) &size, sizeof(int), 0);
    send(connection, (char *)&(*databaseInfo), size * sizeof(FileProp), 0);
    delete[] databaseInfo;
    cout << "finish hand-shaking client, listening to orders..." << endl;

    //the active session, wait for commands and operates them.
    while (true) {
        char buff[RECEIVE_MESSAGE_LENGTH] = {0};
        int byteReceive = recv(connection, buff, RECEIVE_MESSAGE_LENGTH, 0);
        if (byteReceive == SOCKET_ERROR) {
            cerr << "error receiving data!" << endl;
            break;
        }
        if (byteReceive == 0) {
            cout << "client disconnected !!" << endl;
            break;
        }
        char sendBuff[RETURN_MESSAGE_LENGTH] = "success!!";
        try {
            FileProp* fp = bl.operateCommand(buff);
            memcpy(sendBuff, (char*)&(*fp), RETURN_MESSAGE_LENGTH);
            delete fp;
        } catch (const char *c) {
            strncpy(sendBuff, c, RETURN_MESSAGE_LENGTH);
        }
        cout << "sending back: " << sendBuff << endl;
        send(connection, sendBuff, RETURN_MESSAGE_LENGTH + 1, 0);
    }


    // Close the connections
    close(connection);
    close(sockfd);

    WSACleanup();

    return 0;
}




