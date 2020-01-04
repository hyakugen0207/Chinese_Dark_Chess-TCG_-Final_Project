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
#define BUFFER_LENGTH    9

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
    bool isEnd = false;

    while(!isEnd){
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
            /*
            *   command in buffer (x = don't care)
            *   
            *   quit          : qxxxxxxx\0 ->  re : qxxxxxxx\0
            *   reset_board   : rxxxxxxx\0 ->  re : rxxxxxxx\0
            *   move          : ma2-b3xx\0 ->  re : mxxxxxxx\0
            *   flip          : fa2(b3)x\0 ->  re : fxxxxxxx\0
            *   genmove       : grxxxxxx\0 ->  re : a1-d8xxx\0
            *   ready         : kxxxxxxx\0 ->  re : kxxxxxxx\0
            *   time_settings : txxxxxxx\0 ->  re : txxxxxxx\0
            *   time_left     : lr______\0 ->  re : lxxxxxxx\0
            * 
            */

            std::cerr << "get : " << buffer << std::endl;

            // decode & do something
            switch (buffer[0])
            {
            case 'q':
                std::cerr << "get command : quit" << std::endl;
                buffer[0] = 'q';
                isEnd = true;
                break;
            case 'r':
                std::cerr << "get command : reset" << std::endl;
                buffer[0] = 'r';
                break;
            case 'm':
                std::cerr << "get command : move" << std::endl;
                buffer[0] = 'm';
                break;
            case 'f':
                std::cerr << "get command : flip" << std::endl;
                buffer[0] = 'f';
                break;
            case 'g':
                std::cerr << "get command : genmove" << std::endl;
                buffer[2] = '-';
                break;
            case 'k':
                std::cerr << "get command : ready" << std::endl;
                buffer[0] = 'k';
                break;
            case 't':
                std::cerr << "get command : time setting" << std::endl;
                buffer[0] = 't';
                break;
            case 'l':
                std::cerr << "get command : time left" << std::endl;
                buffer[0] = 'l';
                break;
            default:
                std::cerr << "wrong command" << std::endl;
                break;
            }
            buffer[8] = '\0';
            std::cout << "send : " << buffer << std::endl;


            // send reslut
            rc = send(sd, buffer, 9, 0);
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