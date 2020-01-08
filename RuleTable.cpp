#include "RuleTable.hpp"
#include "Board.hpp"
bool RuleTable::LEGAL_EAT_ARRAY[18][18];
int RuleTable::MOVE_NUM[60]; //ok
int RuleTable::MOVE_DIR[60][4]; //ok
int RuleTable::JUMP_NUM[60]; //ok
int RuleTable::JUMP_DIR[60][4]; //ok
int RuleTable::ORI_ALIVE_PIECES[7]; 
int RuleTable::DIR[4]; // {TOP, DOWN, LEFT, RIGHT} //ok
int RuleTable::PIECE_SCORE[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int const RuleTable::PIECE_SCORE_GROUP[4][18]={
    {80,48,15,5,2,29,15,0,80,48,15,5,2,29,15,0,0,0}, 
    {290,162,54,18,6,54,1,0,0,163,55,19,7,55,57,0,0,0},
    {0,150,32,10,4,180,120,0,500,130,32,10,4,54,1,0,0,0},
    {0,70,15,5,2,19,1,0,0,70,15,5,2,19,1,0,0,0}
};
/*
0 : BothWithKing
1 : EnemyWithoutKing
2 : WithoutKing
3 : BothWithoutKing
*/

void RuleTable::initRuleTable(){
    initDir();
    initLegalEatArray();
    initMoveNum();
    initMoveDir();
    initJumpNum();
    initJumpDir();
    initOriAlivePieces();
};

bool RuleTable::isInside(int pos){
    return !(pos<10 || pos>49 || pos%10==0 || pos%10==9);
};

void RuleTable::initDir(){
    DIR[0] = TOP;
    DIR[1] = DOWN;
    DIR[2] = LEFT;
    DIR[3] = RIGHT;
};

void RuleTable::initLegalEatArray(){
    // 0 : 帥   1 : 仕   2 : 像   3 : 俥  4 : 瑪  5 : 炮   6 : 兵  7 : x
    // 8 : 將   9 : 士  10 : 象  11 : 車 12 : 馬 13 : 包  14 : 卒  
    // 16 : dark
    // 17 : dead
    for(int i = 0  ; i < 18 ; ++i){  // init
        for(int j = 0 ; j < 18 ; ++j){
            LEGAL_EAT_ARRAY[i][j] = 0;
        }
        //if(i!=7 && i<15) LEGAL_EAT_ARRAY[i][17] = 1;
        //else LEGAL_EAT_ARRAY[i][17] = 0;
    }

    
    for(int i = 8 ; i < 14 ; ++i ) LEGAL_EAT_ARRAY[0][i] = 1; //帥
    for(int i = 9 ; i < 15 ; ++i ) LEGAL_EAT_ARRAY[1][i] = 1; //仕
    for(int i = 10 ; i < 15 ; ++i ) LEGAL_EAT_ARRAY[2][i] = 1; //像
    for(int i = 11 ; i < 15 ; ++i ) LEGAL_EAT_ARRAY[3][i] = 1; //俥
    for(int i = 12 ; i < 15 ; ++i ) LEGAL_EAT_ARRAY[4][i] = 1; //瑪
    //for(int i = 8 ; i < 15 ; ++i ) LEGAL_EAT_ARRAY[5][i] = 1; //炮
    LEGAL_EAT_ARRAY[6][14] = 1;  LEGAL_EAT_ARRAY[6][8] = 1; //兵
    for(int i = 0 ; i < 6 ; ++i ) LEGAL_EAT_ARRAY[8][i] = 1; //將
    for(int i = 1 ; i < 7 ; ++i ) LEGAL_EAT_ARRAY[9][i] = 1; //士
    for(int i = 2 ; i < 7 ; ++i ) LEGAL_EAT_ARRAY[10][i] = 1; //象
    for(int i = 3 ; i < 7 ; ++i ) LEGAL_EAT_ARRAY[11][i] = 1; //車
    for(int i = 4 ; i < 7 ; ++i ) LEGAL_EAT_ARRAY[12][i] = 1; //馬
    //for(int i = 0 ; i < 7 ; ++i ) LEGAL_EAT_ARRAY[13][i] = 1; //包
    LEGAL_EAT_ARRAY[14][6] = 1;  LEGAL_EAT_ARRAY[14][0] = 1; //卒
};

void RuleTable::initMoveNum(){
    for(int i = 0 ; i < 60 ; ++i){
        if( i<10 || i>49 || i%10==0 || i%10==9) MOVE_NUM[i] = 0;
        else
        {
            if(i==11 || i==18 || i==41 || i==48) MOVE_NUM[i] = 2;
            else if( i<18 || i>41 || i%10==1 || i%10==8) MOVE_NUM[i] = 3;
            else MOVE_NUM[i] = 4;
        }
    }
};

void RuleTable::initMoveDir(){
    for(int i = 0 ; i < 60 ; ++i){
        if(MOVE_NUM[i]==0) continue;
        int p = 0;
        for(int dir = 0 ; dir < 4 ; ++dir){
            if(isInside(i+DIR[dir])){
                MOVE_DIR[i][p] = DIR[dir];
                ++p;
            }
        }
    }
};

void RuleTable::initJumpNum(){
    for(int i = 0 ; i < 60 ; ++i){
        if( i<10 || i>49 || i%10==0 || i%10==9) JUMP_NUM[i] = 0;
        else
        {
            switch (i%10)
            {
                case 1:
                case 2:
                case 7:
                case 8:
                    JUMP_NUM[i] = 2;
                    break;
                default:
                    JUMP_NUM[i] = 3;
                    break;
            }
        }
    }
};

void RuleTable::initJumpDir(){
    for(int i = 0 ; i < 60 ; ++i){
        if(JUMP_NUM[i]==0)continue;
        int p = 0;
        for(int dir = 0 ; dir < 4 ; ++dir){
            if(isInside(i+DIR[dir]*2)){
                JUMP_DIR[i][p] = DIR[dir];
                ++p;
            }
        }
    }
};

void RuleTable::initOriAlivePieces(){
    int a[7] = {1,2,2,2,2,2,5};
    for(int i = 0; i < 7 ; ++i){
        ORI_ALIVE_PIECES[i] = a[i];
    }
};

void RuleTable::print(){
/*
    std::cerr << "MoveNum MoveDir" << std::endl;

    for(int i = 0 ; i < 60 ; ++i){
        std::cerr << MOVE_NUM[i] << " ";
        for(int j = 0 ; j < 4 ; ++j){
            std::cerr << MOVE_DIR[i][j] << " ";
        }
        std::cerr << std::endl;
    }


    std::cerr << "JumpNum JumpDir" << std::endl;

    for(int i = 0 ; i < 60 ; ++i){
        std::cerr << JUMP_NUM[i] << " ";
        for(int j = 0 ; j < 4 ; ++j){
            std::cerr << JUMP_DIR[i][j] << " ";
        }
        std::cerr << std::endl;
    }
*/
    std::cerr << "LEGAL_EAT_ARRAY" << std::endl;
    for(int i = 0 ; i < 18 ; ++i){
        for(int j = 0 ; j < 18 ; ++j){
            
            std::cerr << LEGAL_EAT_ARRAY[i][j] << " ";
            if(j==7 || j==15) std::cerr << "| ";
            
        }
        std::cerr << std::endl;
    }
};


/*
0 : BothWithKing
1 : EnemyWithoutKing
2 : WithoutKing
3 : BothWithoutKing
*/
void RuleTable::setScoreStrategyByBoard(Board* board){
    bool withKing = board->alivePieces[board->ply][0];
    bool enemyWithKing = board->alivePieces[!board->ply][0];
    if(enemyWithKing)
    {
        if(withKing) setScoreStrategy(0);   
        else setScoreStrategy(2);   
    }
    else
    {
        if(withKing) setScoreStrategy(1);   
        else setScoreStrategy(3);   
    }
};

void RuleTable::setScoreStrategy(int strategy){
    for(int i = 0 ; i < 18 ; ++i){
        PIECE_SCORE[i] = PIECE_SCORE_GROUP[strategy][i];
    }
}