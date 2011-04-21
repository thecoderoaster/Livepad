//
//  NetworkingAPI.cpp
//  Hostess
//
//  Created by Krikor Hovasapian on 4/20/11.
//  Copyright 2011 UNLV Grad Student. All rights reserved.
//

#include "NetworkingAPI.h"

//Constructor
NetworkingAPI::NetworkingAPI()
{
    m_sock = 0;
    m_msgsock = 0;
    m_length = 0;
    memset(m_hostname, 0, sizeof(m_hostname));
    memset((timeval*) &m_to, 0, sizeof(timeval));
    memset((hostent*) &m_hp, 0, sizeof(hostent));
    memset((sockaddr_in*) &m_name, 0, sizeof(sockaddr_in));
    memset((sockaddr_in*) &m_server, 0, sizeof(sockaddr_in));
    memset((sockaddr_in*) &m_client, 0, sizeof(sockaddr_in));
}

//Destructor
NetworkingAPI::~NetworkingAPI()
{
    
}

/********************************************************************************/
//Method:           SocketOpen
//Purpose:          Wrapper method for the POSIX based socket function.
//Requirements:     Pass in the domain name, the stream type, and the protocol
//                  of choice.
//                  
//                  domain      :=      PF_INET
//                              :=      PF_INET6
//                              :=      PF_UNIX
//                              :=      AF_INET
//                              :=      AF_INET6
//
//                  type        :=      SOCK_STREAM
//                              :=      SOCK_DGRAM
//                              :=      SOCK_SEQPACKET
//                              :=      SOCK_RAW
//
//                  protocol    :=      IPPROTO_TCP
//                              :=      IPPROTO_UDP
//
//Returns:          Returns the open socket file descriptor, otherwise the
//                  following error codes can be returned if you do not pass
//                  in the right values:
//              
//                  12      :=      ENOMEM
//                  13      :=      EACCES
//                  23      :=      ENFILE
//                  24      :=      EMFILE
//                  41      :=      EPROTOTYPE
//                  43      :=      EPROTONOSUPPORT
//                  47      :=      EAFNOSUPPORT
//                  55      :=      ENOBUFS
//                  56      :=      EISCONN
//
//                  1000    :=      ERROR_SOCKDOMAIN
//                  1001    :=      ERROR_SOCKTYPE
//                  1002    :=      ERROR_SOCKPROTOCOL
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketOpen(int domain, int type, int protocol)
{
    int sockfd = 0;
    
    //Sanity Check of Method Parameters
    if(domain != PF_INET || domain != PF_INET6 || domain != PF_UNIX || 
       domain != AF_INET || domain != AF_INET6)
        return(ERROR_SOCKDOMAIN);
    
    if(type != SOCK_STREAM || type != SOCK_DGRAM || type != SOCK_SEQPACKET || 
       type != SOCK_RAW)
        return(ERROR_SOCKTYPE);
    
    if(protocol != IPPROTO_TCP || protocol != IPPROTO_UDP)
        return(ERROR_SOCKPROTOCOL);
    
    //Create Socket
    if((sockfd = socket(domain, type, protocol)) < 0)
    {
        //**TODO** Log Error here
        return(errno);
    }
    
    return(sockfd);
}

/********************************************************************************/
//Method:           SocketBind
//Purpose:          Wrapper method for the POSIX based bind function.
//                  This method will bind a name to a socket.
//Requirements:     Pass in the socket file descriptor, a pointer to struct
//                  sockaddr containing information about your address
//                  (port and IP), pass in the length in bytes of that address
//
//                  
//                  sockfd      :=  Must be a valid socket descriptor
//                                  Use SocketOpen to create one.
//
//                  *addr      :=  Points to a sockaddr_in structure containing
//                                  the address to be bound to the socket.
//                  
//                  addrlen     :=  Specifies the length of the sockaddr_in
//                                  structure being passed in.
//NOTE FROM KOR:
//                  **addr is a pointer of type sockaddr_in, not sockaddr. 
//                  For those of you not familiar with ipv4, sockaddr is the
//                  socket structure we typecast to within this method 
//                  when calling the "bind" function. This is a wrapper method
//                  and my only expectation from you is that you fill in the
//                  passed sockaddr_in structure( *addr) with the correct
//                  data. If you don't, then the API will kick back denoting
//                  one of the API error codes listed in ErrorCodes.h.
//
//Returns:          Returns NORMAL (1) if binding successful , otherwise 
//                  the following error codes can be returned if you do not pass
//                  in the right values:
//
//                  2       :=      ENOENT
//                  5       :=      EIO
//                  9       :=      EBADF
//                  13      :=      EACCES
//                  20      :=      ENOTDIR
//                  21      :=      EISDIR
//                  22      :=      EINVAL
//                  30      :=      EROFS
//                  38      :=      ENOTSOCK
//                  39      :=      EDESTADDRREQ
//                  45      :=      EOPNOTSUPP
//                  47      :=      EAFNOSUPPORT
//                  48      :=      EADDRINUSE
//                  49      :=      EADDRNOTAVAIL
//                  55      :=      ENOBUFS
//                  56      :=      EISCONN
//                  62      :=      ELOOP
//                  63      :=      ENAMETOOLONG
//                  
//                  1003    :=      ERROR_SOCKINVALID
//                  1004    :=      ERROR_INVADDRFAM
//                  1005    :=      ERROR_INVADDRLEN
//                  1006    :=      ERROR_INVPORTNUM
//                  1007    :=      ERROR_INVZEROPAD
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketBind(int sockfd, struct sockaddr_in *addr, int addrlen)
{
    //Sanity Check of Method Parameters
    if(sockfd < 0)
        return(ERROR_SOCKINVALID);
    if(addr->sin_family != AF_INET || addr->sin_family != AF_INET6)
        return(ERROR_INVADDRFAM);
    if(addr->sin_addr.s_addr != INADDR_ANY)
        return(ERROR_INVADDR);
    if(addr->sin_port >= (uint16_t)0 && addr->sin_port <= (uint16_t)65535)
        return(ERROR_INVPORTNUM);
    if(strncmp(addr->sin_zero, "00000000", sizeof(addr->sin_zero)) != 0)
        return(ERROR_INVZEROPAD);
    if(addrlen != sizeof(sockaddr_in))
        return(ERROR_INVADDRLEN);
    
    //Establish binding
    if(bind(sockfd, (sockaddr*)addr, addrlen) < 0)
    {
         //**TODO** Log Error here
        return(errno);
    }
    return(NORMAL);
}

