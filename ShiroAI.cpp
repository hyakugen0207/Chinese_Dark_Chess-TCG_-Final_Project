#include "myDefine.hpp"
#include "Board.hpp"
//#include "ZobristHashTable.hpp"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <signal.h> 

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

char convertToPiece(char command){
    switch (command)
    {
    case 'K':
        return 0;
    case 'G':
        return 1;
    case 'M':
        return 2;
    case 'R':
        return 3;
    case 'N':
        return 4;
    case 'C':
        return 5;
    case 'P':
        return 6;
    case 'k':
        return 8;
    case 'g':
        return 9;
    case 'm':
        return 10;
    case 'r':
        return 11;
    case 'n':
        return 12;
    case 'c':
        return 13;
    case 'p':
        return 14;
    default:
        return -1;
    }
}


void setScoreStrategyByCurrentBoard(Board* board){
    RuleTable::setScoreStrategyByBoard(board);
}

int main(){
    RuleTable::initRuleTable();
    RuleTable::print();
    //ZobristHashTable* hashTable = new ZobristHashTable();
    bool isEnd = false;
    Board* myBoard = new Board();

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
            if(getppid()==1){
                exit(0);
            }
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
            *   flip          : fa2(G)xx\0 ->  re : fxxxxxxx\0
            *   genmove       : grxxxxxx\0 ->  re : a1-d8xxx\0
            *   ready         : kxxxxxxx\0 ->  re : kxxxxxxx\0
            *   time_settings : txxxxxxx\0 ->  re : txxxxxxx\0
            *   time_left     : lr______\0 ->  re : lxxxxxxx\0
            * 
            */

            std::cerr << "(client) get : " << buffer << std::endl;

            // decode & do something
            switch (buffer[0])
            {
                case 'q':
                    std::cerr << "(client) get command : quit" << std::endl;
                    buffer[0] = 'q';
                    isEnd = true;
                    break;
                case 'r':
                    std::cerr << "(client) get command : reset" << std::endl;
                    buffer[0] = 'r';
                    myBoard->initBoard();
                    break;
                case 'm':
                    std::cerr << "(client) get command : move" << std::endl;
                    myBoard->move((buffer[1]-'a'+1)*10+(buffer[2]-'0'),(buffer[4]-'a'+1)*10+(buffer[5]-'0'));
                    buffer[0] = 'm';
                    break;
                case 'f':
                    std::cerr << "(client) get command : flip" << std::endl;
                    myBoard->flip((buffer[1]-'a'+1)*10+(buffer[2]-'0'),convertToPiece(buffer[4]));
                    buffer[0] = 'f';
                    break;
                case 'g':
                {
                    std::cerr << "(client) get command : genmove" << std::endl;

                    std::pair<char,char> result;
                    if(buffer[1]=='r')
                    {
                        myBoard->ply = 0;
                        // alpha-beta scout
                        result = myBoard->genMove(); // for debug
                    }
                    else if(buffer[1]=='b')
                    {
                        myBoard->ply = 1;
                        // alpha-beta scout
                        result = myBoard->genMove(); // for debug
                    }
                    else //first move
                    {
                        result = myBoard->genMove();
                    }
                    
                    buffer[0] = (result.first/10)-1+'a';
                    buffer[1] = (result.first%10)+'0';
                    buffer[2] = '-';
                    buffer[3] = (result.second/10)-1+'a';
                    buffer[4] = (result.second%10)+'0';

                    setScoreStrategyByCurrentBoard(myBoard);
                    std::cout << "Before move my board score is : " << myBoard->getScore() << std::endl;
                    std::vector<std::pair<char,char>> a;
                    myBoard->moveListGenerator->genPossibleFlipPosition(myBoard,&a);
                    if(a.size()==0)
                    {
                        std::cout << "BestFlipPosition X " << std::endl;
                    }
                    else
                    {
                        std::cout << "BestFlipPosition is : " << int(a[0].first) << std::endl;
                    }
                    
                    
                    
                    break;
                }
                case 'k':
                    std::cerr << "(client) get command : ready" << std::endl;
                    myBoard->initBoard();
                    buffer[0] = 'k';
                    break;
                case 't':
                    std::cerr << "(client) get command : time setting" << std::endl;
                    buffer[0] = 't';
                    break;
                case 'l':
                    std::cerr << "(client) get command : time left" << std::endl;
                    buffer[0] = 'l';
                    break;
                default:
                    std::cerr << "wrong command" << std::endl;
            }
            buffer[8] = '\0';
            std::cerr << "(client) send : " << buffer << std::endl;


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
