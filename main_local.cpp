#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <iostream>
#include <signal.h> 
#include "myDefine.hpp"
#include "Board.hpp"
#include "ZobristHashTable.hpp"
#include "NegaScoutController.hpp"
#include "CDCNode.hpp"
#include "NodePool.hpp"
#include <stack>


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

// commands enumerate
enum COMMANDS{
  PROTOCOL_VERSION = 0, // 0
  NAME, // 1
  VERSION, // 2
  KNOWN_COMMAND, // 3
  LIST_COMMANDS, // 4
  QUIT, // 5
  BOARDSIZE, // 6
  RESET_BOARD, // 7
  NUM_REPETITION, // 8
  NUM_MOVES_TO_DRAW, // 9
  MOVE, // 10
  FLIP, // 11
  GENMOVE, // 12
  GAME_OVER, // 13
  READY, // 14
  TIME_SETTINGS, // 15
  TIME_LEFT, // 16
  SHOWBOARD // 17
};





int main(){
  char read[1024], write[1024], output[1024], *token;
  const char *data[10];
  int id;
  bool isFailed;
    int time;
    double nextTime = 1;
    double allMove = 100;
    double playCount = 0;
    RuleTable::initRuleTable();
    //RuleTable::print();
    ZobristHashTable::initStaticValue();

    bool isEnd = false;
    Board* myBoard = new Board();
    CDCNode* root = nullptr;
  do{
    // read command
    fgets(read, 1024, stdin);
    fprintf(stderr, "%s", read);
    // remove newline(\n)
    read[strlen(read) - 1] = '\0';
    // get command id
    token = strtok(read, " ");
    sscanf(token, "%d", &id);
    // get command name
    token = strtok(NULL, " ");
    // get command data
    int i = 0;
    while((token = strtok(NULL, " ")) != NULL){
      data[i++] = token;
    }

    write[0] = '\0'; // empty the char array
    std::pair<char,char> result;
    char buffer[9];
    switch (id)
    {
    case 5: //quit 
        isEnd = true;
        buffer[0] = 'q';
        break;
    case 7:
        myBoard->initBoard();
        buffer[0] = 'r';
        break;
    case 10:
	    sprintf(buffer, "m%s-%s", data[0], data[1]);
        myBoard->move((buffer[1]-'a'+1)*10+(buffer[2]-'0'),(buffer[4]-'a'+1)*10+(buffer[5]-'0'));
        buffer[0] = 'm';
        break;
    case 11:
	    sprintf(buffer, "f%s(%s)", data[0], data[1]);
        myBoard->flip((buffer[1]-'a'+1)*10+(buffer[2]-'0'),convertToPiece(buffer[4]));
        buffer[0] = 'f';
        break;
    case 12:
        buffer[0] = 'g';
        if(!strcmp(data[0], "red")){
            buffer[1] = 'r';
        }else if(!strcmp(data[0], "black")){
            buffer[1] = 'b';
        }else{
            buffer[1] = 'u';
        }
        // genmove
        buffer[8] = '\0';
        std::cerr << "(client) get command : genmove" << std::endl;

        
        if(buffer[1]=='r')
        {
            myBoard->ply = 0;
            myBoard->rootPly = 0;
        }
        else if(buffer[1]=='b')
        {
            myBoard->ply = 1;
            myBoard->rootPly = 1;
        }

        myBoard->moveListGenerator->handle(myBoard);
        setScoreStrategyByCurrentBoard(myBoard);
        //std::cerr << "Before move my board score is : " << myBoard->getScore() << std::endl;

        //negaScout
        if(NodePool::empty()) root = new CDCNode(myBoard, 1); 
        else
        {
            root = NodePool::pop();
            root->copy(myBoard, 1);
        }
        std::cerr << "time = " << time << std::endl;
        std::cerr << "search time = " << nextTime/1000.0 << std::endl;
        result = NegaScoutController::iterativeDeepening(root,nextTime/1000.0);
        //result = NegaScoutController::iterativeDeepening(root,2);
        if(result.first==0)
        {
            std::cerr << "GGGGG" << std::endl;
            exit(1);
        }
        //std::cerr << "get result" << result.first << "," << result.second << std::endl;

        //std::cerr << "out" << std::endl;
        buffer[0] = (result.first/10)-1+'a';
        buffer[1] = (result.first%10)+'0';
        buffer[2] = '-';
        buffer[3] = (result.second/10)-1+'a';
        buffer[4] = (result.second%10)+'0';

        if(strlen(buffer)>0 && buffer[2]=='-'){
		    sprintf(write, "%c%c %c%c", buffer[0], buffer[1], buffer[3], buffer[4]);
	    }
        playCount++;
        break;
    case 14:
        myBoard->initBoard();
        buffer[0] = 'k';
        break;
    case 15:
        std::cerr << "(client) get command : time setting" << std::endl;
        //time = atoi(data[1]);
        buffer[0] = 't';
        break;
    case 16:
        time = atoi(data[1]);
        std::cerr << "(client) get command : time left : " << time  << std::endl;

        
        nextTime = (double(time) / (allMove-playCount)) > 15000 ? 15000 : (double(time) / (allMove-playCount));

        if(time<60000)
        {
            nextTime = 2000;
        }

        buffer[0] = 'l';
        break;
    default:
        break;
    }

    if(strlen(write) > 0){
        sprintf(output, "=%d %s\n", id, write);
    }else{
        sprintf(output, "=%d\n", id);
    }

    fprintf(stdout, "%s", output);
    fprintf(stderr, "%s", output);
    // important, do not delete
    fflush(stdout);
    fflush(stderr);

    if(root!=nullptr)
    {
        std::cerr << "clean" << std::endl;
        NodePool::recycle(root);
        root = nullptr;
        std::cerr << "clean end" << std::endl;
    }

  }while(id != QUIT);
  
  fprintf(stderr, "fffff");
  return 0;
}

