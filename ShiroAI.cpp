//#include "myDefine.hpp"
//#include "Board.hpp"
//#include "ZobristHashTable.hpp"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define SERVER_PATH     "/tmp/server"
#define BUFFER_LENGTH    6

int recvCommand(char* buffer, int sd){
    int bytesReceived = 0;
    int rc;
    while (bytesReceived < BUFFER_LENGTH)
    {
            
        rc = recv(sd, & buffer[bytesReceived],
                BUFFER_LENGTH - bytesReceived, 0);
        if (rc < 0)
        {
            perror("recv() failed");
            break;
        }
        else if (rc == 0)
        {
            std::cerr << "server closed" << std::endl;
            break;
        }
        bytesReceived += rc;
    } 
    return rc;
}

int main(){

    //ZobristHashTable* hashTable = new ZobristHashTable();
    

    while(true){
        // socket
        int    sd=-1, rc;
        char   buffer[BUFFER_LENGTH];
        buffer[0] = '\0';
        struct sockaddr_un serveraddr;
        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0)
        {
            perror("socket() failed");
            break;
        }
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, SERVER_PATH);
        rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));

        while(rc<0)
        {
            //pondering
            std::cerr << "pondering" << std::endl;
            sleep(1);
            rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        }
        
        // recv command
        if(recvCommand(buffer,sd)){
            std::cout << "get : " << buffer << std::endl;

            // decode & do something
            if(buffer[0]!='g'){
                buffer[0] = '0';
            }else{
                buffer[0] = 'a';
            }
            buffer[1] = '3';
            buffer[2] = '-';
            buffer[3] = 'b';
            buffer[4] = '4';
            buffer[5] = '\0';
            std::cout << "send : " << buffer << std::endl;


            // send reslut
            rc = send(sd, buffer, sizeof(buffer), 0);
            if (rc < 0)
            {
                std::cerr << "send Failed" << std::endl;
                break;
            }
        }   
    }
    


    //get command 
        //move (enemy move)
            //change board
            //check whether ponder is work
                // if work update root
                // else remove ponder root
            //return 10\n
        //flip (enemy flip)
            //change board
            //check whether ponder is work
                // if work update root
                // else remove ponder root
            //return 11\n
        //genmove
            //Scout+Alpha+Beta from root
            
        //none
            //do ponder







    return 0;
}