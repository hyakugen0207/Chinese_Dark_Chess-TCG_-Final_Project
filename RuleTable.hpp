#include "myDefine.hpp"

#define TOP -10
#define DOWN 10
#define LEFT -1
#define RIGHT 1

class RuleTable{
    public:
        static bool LEGAL_EAT_ARRAY[18][18];
        static int MOVE_NUM[60];
        static int MOVE_DIR[60][4];
        static int JUMP_NUM[60];
        static int JUMP_DIR[60][4]; 
        static int ORI_ALIVE_PIECES[7];
        static int DIR[4]; // {TOP, DOWN, LEFT, RIGHT }
        

        static void initRuleTable(){
            initDir();
            initLegalEatArray();
            initMoveNum();
            initMoveDir();
            initJumpNum();
            initJumpDir();
            initOriAlivePieces();
        };

    private:

        static void initDir(){
            DIR[0] = TOP;
            DIR[1] = DOWN;
            DIR[2] = LEFT;
            DIR[3] = RIGHT;
        };
        
        static void initLegalEatArray(){

        };

        static void initMoveNum(){
            for(int i = 0 ; i < 60 ; ++i){
                if( i<10 || i>39 || i%10==0 || i%10==9) MOVE_NUM[i] = 0;
                else
                {
                    if(i==11 || i==18 || i==31 || i==38) MOVE_NUM[i] = 2;
                    else if( i<18 || i>31 || i%10==1 || i%10==8) MOVE_NUM[i] = 3;
                    else MOVE_NUM[i] = 4;
                }
            }
        };

        static void initMoveDir(){
            for(int i = 0 ; i < 60 ; ++i){
                for(int j = 0 ; j < MOVE_NUM[i] ; ++j){
                    if(isInside(i+DIR[j])){
                        MOVE_DIR[i][j] = DIR[j];
                    }
                }
            }
        };

        static void initJumpNum(){
            for(int i = 0 ; i < 60 ; ++i){
                if( i<10 || i>39 || i%10==0 || i%10==9) JUMP_NUM[i] = 0;
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

        static void initJumpDir(){
            for(int i = 0 ; i < 60 ; ++i){
                for(int j = 0 ; j < JUMP_NUM[i] ; ++j){
                    if(isInside(i+DIR[j]*2)){
                        JUMP_DIR[i][j] = DIR[j];
                    }
                }
            }
        };

        static void initOriAlivePieces(){
            int a[7] = {1,2,2,2,2,2,5};
            for(int i = 0; i < 7 ; ++i){
                ORI_ALIVE_PIECES[i] = a[i];
            }
        };

        static bool isInside(int pos){
            return !(pos<10 || pos>39 || pos%10==0 || pos%10==9);
        };

        static bool canEat(int from, int to)
        {
            // 0 : 帥   1 : 仕
            // 
        }
};

/*
+ legalEatArray : bool[18][18]
+ moveNum[60]
+ moveDir[60][4]
+ jumpNum[60]
+ jumpDir[60][4]
+ oriAlivePieces[7] 
*
*/