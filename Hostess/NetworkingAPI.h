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
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "sys/time.h"
#include "ErrorCodes.h"

#define HTTPPORT        80          //port80
#define SOCKTIMEOUT     5           //in seconds


typedef struct ClientInfo{
    
    int msgsock;                    //Socket
    int tid;                        //Thread ID
    
}CLIENTINFO;

class NetworkingAPI{

private:

    
public:
    NetworkingAPI();
    ~NetworkingAPI();
    int m_sockfd;
    int m_msgsock;
    char m_hostname[80];
    struct timespec m_to;
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