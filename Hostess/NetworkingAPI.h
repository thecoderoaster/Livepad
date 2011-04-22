//
//  NetworkingAPI.h
//  Hostess
//
//  Created by Krikor Hovasapian on 4/20/11.
//  Copyright 2011 UNLV Grad Student. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
    int m_sockfd;
    int m_msgsock;
    char m_hostname[80];
    struct timeval m_to;
    struct hostent *m_hp;
    struct sockaddr_in m_name;
    struct sockaddr_in m_server;
    struct sockaddr_in m_client;
    
    int SocketOpen(int domain, int type, int protocol);
    int SocketBind(int sockfd, struct sockaddr_in *addr, int addrlen);
    int SocketName(int sockfd, struct sockaddr_in *addr, socklen_t *addrlen);
    int SocketHostName(char *hostname, size_t namelen);
    int SocketHostByName(char *hostname, struct hostent* hp);
    ssize_t SocketWrite(int sockfd, char *ptr, ssize_t nbytes);
    ssize_t SocketRead(int sockfd, char *ptr, ssize_t nbytes);
    
};