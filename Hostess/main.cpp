//
//  main.cpp
//  Hostess
//
//  Created by Krikor Hovasapian on 4/20/11.
//  Copyright 2011 UNLV Grad Student. All rights reserved.
//

#include <iostream>
#include "NetworkingAPI.h"

#define NUM_THREADS     1000

void *ConnectionHandler(void *obj)
{
    NetworkingAPI net;
    int *msgsock = reinterpret_cast<int*>(obj);
    //long tid = (long)threadid;
    
    std::cout << "Hostess(TID: ); INFO: Starting ConnectionHandler...\n";
    std::cout << "Hostess(TID: ); INFO: socket = %d\n" << *msgsock;
    
    //Send a response back
    
    //Wait for a message from client

    std::cout << "Hostess(TID: ); INFO: Connection Closed.\n";
    pthread_exit(NULL);
}

int main (int argc, const char * argv[])
{
    //Networking Related
    NetworkingAPI *net80    = NULL;                //HTTP Socket
    socklen_t length        = 0;
    fd_set ready;
    sigset_t origmask;
    int sock                = 0;
    
    //Thread Related
    pthread_t *threads;
    int t                   = 0;
    
    //Error Handling Related
    int ret                 = 0;
    
    //Launch Hostess and report back to management
    //TO DO here:
    //Run vitals and populate notification to send
    //back to management
    std::cout << "Hello, I'm Hostess!\n";
    std::cout << "***Running System Stats***\n";
    
    //Initialize classes
    net80 = new NetworkingAPI();
    
    //Initialize memory
    threads = new pthread_t[NUM_THREADS]();
    
    //Create main socket
    if((net80->m_msgsock = net80->SocketOpen(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < NORMAL)
    {
        //Notify Management
        //Log to file
        std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed to open socket! (FATAL)\n";
        goto CLEANUP;
    }
    
    //Bind the socket
    net80->m_server.sin_family = AF_INET;
    net80->m_server.sin_addr.s_addr = INADDR_ANY;
    net80->m_server.sin_port = htons(HTTPPORT);
    if((ret = net80->SocketBind(net80->m_msgsock, &net80->m_server, sizeof(net80->m_server))) < NORMAL)
    {
        //Notify Management
        //Log to file
        std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed to bind to socket! (FATAL)\n";
        goto CLEANUP;
    }
    
    //Get the socket name
    length = sizeof(net80->m_server);
    if((ret = net80->SocketName(net80->m_msgsock, &net80->m_server, &length)) < NORMAL)
    {
        //Notify Management
        //Log to file
        std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed to grab socket name! (FATAL)\n";
        goto CLEANUP;
    }
    
    //Get the host name
    length = sizeof(net80->m_hostname);
    if((ret = net80->SocketHostName(net80->m_hostname, length)) < NORMAL)
    {
        //Notify Management
        //Log to file
        std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed to grab the host name! (FATAL)\n";
        goto CLEANUP;
    }
    else if((ret = net80->SocketHostByName(net80->m_hostname, net80->m_hp)) < NORMAL)
    {
        //Notify Management
        //Log to file
        std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed to grab the host name! (FATAL)\n";
        goto CLEANUP;
    }
    
    //Start Accepting Connections
    std::cout << "Hostess(); INFO: Waiting for a socket request...\n";
    
    listen(net80->m_msgsock, SOCKTIMEOUT);
        
    do{
        
        //Wait for incoming request
        FD_ZERO(&ready);
        FD_SET(net80->m_msgsock, &ready);
        net80->m_to.tv_sec = SOCKTIMEOUT;
    
        std::cout <<"Hostess(); INFO: Waiting for clients...\n";
        if((ret = pselect((net80->m_msgsock) + 1, &ready, 0, 0, &net80->m_to, &origmask)) < NORMAL)
        {
            //Notify Management
            //Log to file
            std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed on pselect! (FATAL)\n";
            FD_ZERO(&ready);
            goto CLEANUP;
        }
        
        if(FD_ISSET(net80->m_msgsock, &ready))
        {
            if((sock = accept(net80->m_msgsock, (struct sockaddr*)&net80->m_server, &length)) < NORMAL)
            {
                //Notify Management
                //Log to file
                std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Connection denied! Failed on accept. (NON-FATAL)\n";
            }
            else
            {
                //Spawn Thread
                if(pthread_create(&threads[t++], NULL, ConnectionHandler, (void *)sock))
                {
                    //Notify Management
                    //Log to file
                    std::cout <<"Hostess(); ERROR on line " << __LINE__ << " :Failed to create thread! (NON-FATAL)\n";
                }
            }
        }
    }while(true);
    
CLEANUP:
    //Cleanup
    delete net80;
    delete [] threads;
        
    std::cout <<"Hostess(); INFO: Cleaned up and now exiting (Yay! My shift is done!)\n";
    return 0;
}

