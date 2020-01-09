#ifndef RULETABLE
#define RULETABLE

#define TOP -10
#define DOWN 10
#define LEFT -1
#define RIGHT 1
#include <iostream>

class Board;

class RuleTable{
    public:
        static bool LEGAL_EAT_ARRAY[18][18];
        static int MOVE_NUM[60];
        static int MOVE_DIR[60][4];
        static int JUMP_NUM[60];
        static int JUMP_DIR[60][4]; 
        static int ORI_ALIVE_PIECES[7];
        static int DIR[4]; // {TOP, DOWN, LEFT, RIGHT}
        static int PIECE_SCORE[18];
        static char currentState;
        static const int PIECE_SCORE_GROUP[4][18];

        static void initRuleTable();
        static void print();
        static bool isInside(int pos);
        static void setScoreStrategyByBoard(Board* );
        static int getPieceScore(char piece);
        

    private:

        static void initDir();
        static void initLegalEatArray();
        static void initMoveNum();
        static void initMoveDir();
        static void initJumpNum();
        static void initJumpDir();
        static void initOriAlivePieces();
        static void setScoreStrategy(int);
};

#endif //RULETABLE
/*
+ legalEatArray : bool[18][18]
+ moveNum[60]
+ moveDir[60][4]
+ jumpNum[60]
+ jumpDir[60][4]
+ oriAlivePieces[7] 
*
*/