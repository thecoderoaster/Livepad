//
//  NetworkingAPI.cpp
//  Hostess
//
//  Created by Krikor Hovasapian on 4/20/11.
//  Copyright 2011 UNLV Grad Student. All rights reserved.
//

#include "NetworkingAPI.h"


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
//                  ERROR_SOCKDOMAIN
//                  ERROR_SOCKTYPE
//                  ERROR_SOCKPROTOCOL
//                  ERROR_SOCKETOPEN
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
        return(ERROR_SOCKETOPEN);
    }
    
    return(sockfd);
}