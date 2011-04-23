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
    m_sockfd = 0;
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
//                  This method will open and return an active socket
//Requirements:     Pass in the domain name, the stream type, and the protocol
//                  of choice:
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
//                  -12      :=      ENOMEM
//                  -13      :=      EACCES
//                  -23      :=      ENFILE
//                  -24      :=      EMFILE
//                  -41      :=      EPROTOTYPE
//                  -43      :=      EPROTONOSUPPORT
//                  -47      :=      EAFNOSUPPORT
//                  -55      :=      ENOBUFS
//                  -56      :=      EISCONN
//
//                  -1000    :=      ERROR_SOCKDOMAIN
//                  -1001    :=      ERROR_SOCKTYPE
//                  -1002    :=      ERROR_SOCKPROTOCOL
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketOpen(int domain, int type, int protocol)
{
    int sockfd = 0;
    
    //Sanity Check of Method Parameters
    if(domain != PF_INET && domain != PF_INET6 && domain != PF_UNIX && 
       domain != AF_INET && domain != AF_INET6)
        return(ERROR_SOCKDOMAIN);
    
    if(type != SOCK_STREAM && type != SOCK_DGRAM && type != SOCK_SEQPACKET && 
       type != SOCK_RAW)
        return(ERROR_SOCKTYPE);
    
    if(protocol != IPPROTO_TCP && protocol != IPPROTO_UDP)
        return(ERROR_SOCKPROTOCOL);
    
    //Create Socket
    if((sockfd = socket(domain, type, protocol)) < 0)
    {
        //**TODO** Log Error here
        return(errno*-1);
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
//                  sockfd(IN)      :=  Must be a valid socket descriptor
//                                  Use SocketOpen to create one.
//
//                  *addr(IN)       :=  Points to a sockaddr_in structure 
//                                  containing the address to be bound to 
//                                  the socket.
//                  
//                  addrlen(IN)     :=  Specifies the length of the sockaddr_in
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
//Returns:          Returns NORMAL (0) if binding successful , otherwise 
//                  the following error codes can be returned if you do not pass
//                  in the right values:
//
//                  -2       :=      ENOENT
//                  -5       :=      EIO
//                  -9       :=      EBADF
//                  -13      :=      EACCES
//                  -20      :=      ENOTDIR
//                  -21      :=      EISDIR
//                  -22      :=      EINVAL
//                  -30      :=      EROFS
//                  -38      :=      ENOTSOCK
//                  -39      :=      EDESTADDRREQ
//                  -45      :=      EOPNOTSUPP
//                  -47      :=      EAFNOSUPPORT
//                  -48      :=      EADDRINUSE
//                  -49      :=      EADDRNOTAVAIL
//                  -55      :=      ENOBUFS
//                  -56      :=      EISCONN
//                  -62      :=      ELOOP
//                  -63      :=      ENAMETOOLONG
//                  
//                  -1003    :=      ERROR_SOCKINVALID
//                  -1004    :=      ERROR_INVADDRFAM
//                  -1005    :=      ERROR_INVADDRLEN
//                  -1006    :=      ERROR_INVPORTNUM
//                  -1007    :=      ERROR_INVZEROPAD
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketBind(int sockfd, struct sockaddr_in *addr, int addrlen)
{
    //Sanity Check of Method Parameters
    if(sockfd < 0)
        return(ERROR_SOCKINVALID);
    if(addr->sin_family != AF_INET && addr->sin_family != AF_INET6)
        return(ERROR_INVADDRFAM);
    if(addr->sin_addr.s_addr != INADDR_ANY)
        return(ERROR_INVADDR);
    //if(addr->sin_port >= (in_port_t)htons(0) && addr->sin_port <= (in_port_t)htons(65535))
        //return(ERROR_INVPORTNUM);
    //if(strncmp(addr->sin_zero, "00000000", sizeof(addr->sin_zero)) != 0)
        //return(ERROR_INVZEROPAD);
    if(addrlen != sizeof(sockaddr_in))
        return(ERROR_INVADDRLEN);
    
    
    //Establish binding
    if(bind(sockfd, (sockaddr*)addr, addrlen) < 0)
    {
         //**TODO** Log Error here
        return(errno*-1);
    }
    return(NORMAL);
}

/********************************************************************************/
//Method:           SocketName
//Purpose:          Wrapper method for the POSIX based getsockname function.
//                  This method will retrieve the locally-bound name of the
//                  specified socket and store that information in the 
//                  sockaddr_in structure passed in, and the length of the
//                  address.
//Requirements:     Pass in the socket file descriptor, a pointer to struct
//                  sockaddr_in, pass in the length in bytes of that address
//
//                  
//                  sockfd(IN)      :=  Must be a valid socket descriptor
//                                      Use SocketOpen to create one.
//
//                  *addr(OUT)      :=  Points to a sockaddr_in structure 
//                                      containing the information about 
//                                      the socket.
//                  
//                  *addrlen(OUT)   :=  Specifies the length of the sockaddr_in
//                                      structure being returned by getsockname. 
//                                 
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
//Returns:          Returns NORMAL (0) if binding successful , otherwise 
//                  the following error codes can be returned if you do not pass
//                  in the right values:
//
//                  -9       :=      EBADF
//                  -14      :=      EFAULT
//                  -22      :=      EINVAL
//                  -38      :=      ENOTSOCK
//                  -45      :=      EOPNOTSUPP
//                  -55      :=      ENOBUFS
//                  -98      :=      ENOSR

//                  
//                  -1003    :=      ERROR_SOCKINVALID
//                  -1004    :=      ERROR_INVADDRFAM
//                  -1005    :=      ERROR_INVADDRLEN
//                  -1006    :=      ERROR_INVPORTNUM
//                  -1007    :=      ERROR_INVZEROPAD
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketName(int sockfd, struct sockaddr_in *addr, socklen_t *addrlen)
{
    //Sanity Check of Method Parameters
    if(sockfd < 0)
        return(ERROR_SOCKINVALID);

    //Grab the socket name
    if(getsockname(sockfd, (sockaddr*)addr, addrlen) < 0)
    {
        //**TODO** Log Error here
        return(errno);
    }
    
    return(NORMAL);
}

/********************************************************************************/
//Method:           SocketHostName
//Purpose:          Wrapper method for the POSIX based gethostname function.
//                  This method will return the standard host name for the 
//                  current machine.
//Requirements:     Pass in the socket file descriptor, a pointer to struct
//                  sockaddr_in, pass in the length in bytes of that address
//
//                  
//                  *hostname(OUT)      :=  NULL terminated character string 
//                                      returned by gethostname that contains the 
//                                      returned hostname.
//
//                  *namelen(IN)        :=  length of the character string
//                                      in bytes. Typically no hostname
//                                      is greater than 255.
//                   
//                                 
//NOTE FROM KOR:
//                  The hostname is typically limited to 255 characters (bytes)
//                  If you provide an insufficent value for the namelen and the
//                  hostname, by chance, is greater than 255, then the returned
//                  name is truncated and it is unspecified if the returned
//                  name is NULL terminated.
//
//Returns:          Returns NORMAL (0) if binding successful , otherwise 
//                  the following error codes can be returned if you do not pass
//                  in the right values:
//
//                  NO ERRORS SPECIFIED
//                  
//                  -1008    :=      ERROR_INVNAMELEN
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketHostName(char *hostname, size_t namelen)
{
    if(namelen <= 0)
        return(ERROR_INVNAMELEN);
    
    if(gethostname(hostname, namelen) < 0)
    {
        //**TODO** Log Error here
        return(errno*-1); 
    }
    
    return(NORMAL);
}

/********************************************************************************/
//Method:           SocketHostByName
//Purpose:          Wrapper method for the POSIX based gethostbyname function.
//                  This method searches the netdb database and finds an entry
//                  which matches the host name specified by the hostname
//                  argument. If the name is found, it will retun information
//                  about that hostname in the database.
//
//Requirements:     Pass in a null terminated character string representing
//                  the hostname, and provide a pointer of type struct
//                  hostent to this method that will return vital information
//                  about the hostname, if that entry is found.
//
//                  
//                  *hostname(IN)       :=  NULL terminated character string 
//                                      returned by gethostname that contains the 
//                                      returned hostname.
//
//                  *hp(OUT)            := host entry found by gethostbyname.
//                                 
//NOTE FROM KOR:
//                  Call SocketHostName first if you don't have the name of
//                  your host. Makesure that the hostname is null terminated.
//
//Returns:          Returns NORMAL (0), otherwise  the following 
//                  error codes can be returned if you do not pass in the right 
//                  values:
//
//                  -1   :=      HOST_NOT_FOUND
//                  -2   :=      TRY_AGAIN
//                  -3   :=      NO_RECOVERY
//                  -4   :=      NO_DATA
//                  
// 
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
int NetworkingAPI::SocketHostByName(char *hostname, struct hostent* hp)
{
    if((hp = gethostbyname(hostname)) == NULL)
    {
        //**TODO** Log Error here
        return(h_errno*-1);    
    }
    
    return(NORMAL);
}

/********************************************************************************/
//Method:           SocketWrite
//Purpose:          Wrapper method for the POSIX based write function.
//                  This method writes data to an open socket. 
//
//Requirements:     Pass in an open socket, a buffer containing the content to
//                  be transmitted, and the buffer length in bytes.
//
//                  
//                  sockfd(IN)          := Must be a valid socket descriptor
//                                      Use SocketOpen to create one.
//
//                  *ptr(IN)            := message buffer
//                  
//                  nbytes(IN)          := size of the message buffer in bytes
//                                 
//
//Returns:          Returns the total number of bytes written, otherwise the 
//                  following error codes can be returned if you do not pass in 
//                  the right values:
//
//                  -4           :=      EINTR
//                  -5           :=      EIO
//                  -9           :=      EBADF
//                  -27          :=      EFBIG
//                  -28          :=      ENOSPC
//                  -32          :=      EPIPE
//                  -34          :=      ERANGE
//                  -35          :=      EAGAIN
//                  
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
ssize_t NetworkingAPI::SocketWrite(int sockfd, char *ptr, ssize_t nbytes)
{
    ssize_t nleft = 0;
    ssize_t nwritten = 0;
    
    nleft = nbytes;
    
    while (nleft > 0) {
        if((nwritten = write(sockfd, ptr, nleft)) < 0)
        {
            //**TODO** Log Error here
            return(errno*-1);
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return(nbytes-nleft);
}

/********************************************************************************/
//Method:           SocketRead
//Purpose:          Wrapper method for the POSIX based read function.
//                  This method reads data from an open socket. 
//
//Requirements:     Pass in an open socket, a buffer containing the content to
//                  be received, and the buffer length in bytes of that buffer.
//
//                  
//                  sockfd(IN)          := Must be a valid socket descriptor
//                                      Use SocketOpen to create one.
//
//                  *ptr(OUT)           := message buffer
//                  
//                  nbytes(OUT)         := size of the message buffer in bytes
//                                 
//
//Returns:          Returns the total number of bytes read, otherwise the 
//                  following error codes can be returned if you do not pass in 
//                  the right values:
//
//                  -4           :=      EINTR
//                  -5           :=      EIO
//                  -9           :=      EBADF
//                  -21          :=      EISDIR
//                  -22          :=      EINVAL
//                  -35          :=      EAGAIN
//                  -84          :=      EOVERFLOW
//                  -94          :=      EBADMSG
//                  
//                  Refer to API Documentation for more details as to why you may
//                  have received one of these codes.
/********************************************************************************/
ssize_t NetworkingAPI::SocketRead(int sockfd, char *ptr, ssize_t nbytes)
{
    ssize_t nread = 0;
    
    return(nread);
}