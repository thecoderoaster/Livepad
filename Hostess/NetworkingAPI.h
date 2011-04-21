//
//  NetworkingAPI.h
//  Hostess
//
//  Created by Krikor Hovasapian on 4/20/11.
//  Copyright 2011 UNLV Grad Student. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "sys/time.h"
#include "ErrorCodes.h"

class NetworkingAPI{

private:
    NetworkingAPI();
    ~NetworkingAPI();
    
public:
    int m_sock;
    int m_msgsock;
    int m_length;
    char m_hostname[80];
    struct timeval m_to;
    struct hostent *m_hp;
    struct sockaddr_in m_name;
    struct sockaddr_in m_server;
    struct sockaddr_in m_client;
    
    int SocketOpen(int domain, int type, int protocol);
    int SocketBind(int sockfd, struct sockaddr_in *addr, int addrlen);
    int SocketName(int sockfd, struct sockaddr_in *addr, int *addrlen);
    int SocketHostName(char *hostname, size_t size);
    int SocketWrite(int sockfd, char *ptr, int nbytes);
    int SocketRead(int sockfd, char *ptr, int nbytes);
    struct hostent* SocketHostByName(char *hostname);
    
};