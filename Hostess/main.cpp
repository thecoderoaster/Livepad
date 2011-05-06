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
    //int *msgsock = reinterpret_cast<int*>(obj);
    CLIENTINFO *client = reinterpret_cast<CLIENTINFO*>(obj);
    ssize_t ret = 0;
    char htmlBuffer[363];
    FILE *pFile = NULL;
    
    //Clear Memory
    memset(htmlBuffer, 0, sizeof(htmlBuffer));
    
    std::cout << "Hostess(TID: "<< client->tid << "); INFO: Starting ConnectionHandler...\n";
    std::cout << "Hostess(TID: "<< client->tid << "); INFO: socket = "<< client->msgsock << "\n";
    
    //Read in header
    ret = net.SocketRead(client->msgsock, htmlBuffer, sizeof(htmlBuffer));
    if(ret > 0)
    {
        std::cout<< "Hostess(TID:) read in " << ret << " bytes\n";        
        std::cout<< "Hostess(TID: "<< client->tid << "); " << htmlBuffer << "\n";
        //for(i = 0; i < sizeof(htmlBuffer); i++)
            //printf("0x%x\n", htmlBuffer[i]);
    }
    else
        std::cout<< "Hostess(TID: "<< client->tid << "); Header Missing!\n";
        
    //Read in test .html file
    pFile = fopen("index.html", "r");
    if(pFile == NULL)
        perror("Error opening index.html!");
    else
    {
        while(!feof(pFile))
        {
            fgets(htmlBuffer, 100, pFile);
        }
        fclose(pFile);
        
        //Notify Client
        ret = net.SocketWrite(client->msgsock, htmlBuffer, sizeof(htmlBuffer));
        std::cout << "Hostess(TID: "<< client->tid << "); INFO: Wrote back "<< ret << " bytes\n";
        
    }
        
    //Wait for a message from client

    std::cout << "Hostess(TID: " << client->tid << "); INFO: Connection Closed.\n";
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
    int flags               = 0;
    CLIENTINFO cInfo;
    
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
                //Set socket to NON_BLOCKING
                flags = fcntl(sock, F_GETFL, O_ACCMODE);
                fcntl(sock, F_SETFL, flags | O_NONBLOCK);
                
                //Spawn Thread
                cInfo.msgsock = sock;
                cInfo.tid = t;
                if(pthread_create(&threads[t++], NULL, ConnectionHandler, (void *)&cInfo))
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

