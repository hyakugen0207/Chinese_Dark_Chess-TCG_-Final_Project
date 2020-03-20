#include "Board.hpp"
#include <iostream>
#include <random>


// Strategy State
#include "EarlyGame.hpp"
#include "MidGame.hpp"
#include "BothWithKing.hpp"
#include "EnemyWithoutKing.hpp"
#include "BothWithoutKing.hpp"
#include "WithoutKing.hpp"


const char Board::oriAlivePieces[7] = {1,2,2,2,2,2,5};
const char Board::oriConvertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,60,16,17,19,21,23,25,27,33,32,34};
const char Board::convertPiecesIndexToPiece[35] = {0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,8,9,9,10,10,11,11,12,12,13,13,14,14,14,14,14,16,17,18};
const char Board::oriDarkPieceNum[2][7] = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
const double Board::possibilityHelper[33] = { 
    0, 1, 0.5, 0.333, 0.25, 0.2, 0.167,  0.143, 0.125, 0.111, 0.1,
    0.091, 0.0833, 0.0769, 0.0714, 0.0667, 0.0625, 0.0588, 0.0556, 0.0526, 0.05,
    0.0476, 0.0455, 0.0435, 0.0417, 0.04, 0.0385, 0.037, 0.0357, 0.0345, 0.0333,
    0.0323, 0.03125};

/*
    0, 0.03125, 0.0625, 0.09375, 0.125, 0.15625, 0.1875, 0.21875, 
    0.25, 0.28125, 0.3125, 0.34375, 0.375, 0.40625, 0.4375, 0.46875, 
    0.5, 0.53125, 0.5625, 0.59375, 0.625, 0.65625, 0.6875, 0.71875, 
    0.75, 0.78125, 0.8125, 0.84375, 0.875, 0.90625, 0.9375, 0.96875
*/
bool Board::move(char from, char to){
    // 假設傳進來的一定legal

    // move or eat

    if(board[to]->piece != 17) // eat
    { 
        board[to]->position = -1;
        alivePieces[(board[to]->piece)>>3][(board[to]->piece)&7]--;
        numAlivePieces[(board[to]->piece)>>3]--;
        removePiece(board[to]->indexInPieceList, (board[to]->piece)>>3);
        board[to]->indexInPieceList = -1;
        board[to]->piece = 17; // dead
        board[to]->inside = false;
    }
    Piece* tmp = board[to];
    board[to] = board[from]; 
    board[from] = tmp;
    //board[from]->position = from;
    board[to]->position = to;
    ply = !ply;
    return true;
};

bool Board::flip(char pos, char piece){
    //std::cerr << "Debug : in flip" << std::endl;
    board[pos] = &(pieces[convertPieceToPiecesIndex[piece]]);
    convertPieceToPiecesIndex[piece]++;
    board[pos]->dark = false;
    board[pos]->position = pos;
    board[pos]->inside = true;
    board[pos]->indexInPieceList = addPiece(board[pos], board[pos]->piece>>3);
    darkPieceNumAll--;
    darkPieceNum[piece>>3][piece&7]--;
    updateFlipPossibility();
    //std::cerr << "Debug : in addPiece numPiecesInList is " << numPiecesInList[board[pos]->piece>>3] << std::endl;
    ply = !ply;
    return true;
};

void Board::removePiece(int index, bool ply){

    if(index+1==numPiecesInList[ply])
    {
        pieceList[ply][index] = nullptr;
    }
    else
    {
        for(int i = index+2 ; i < 16 ; ++i){
            if(i==numPiecesInList[ply] || i == 15)
            {   
                if(i!=numPiecesInList[ply]) // 到底了 且是滿的
                {
                    pieceList[ply][index] = pieceList[ply][i];
                    pieceList[ply][index]->indexInPieceList = index;
                    pieceList[ply][i] = nullptr;
                }
                else 
                {
                    pieceList[ply][index] = pieceList[ply][i-1];
                    pieceList[ply][index]->indexInPieceList = index;
                    pieceList[ply][i-1] = nullptr;
                }
                break;
            }
        }
    }
    numPiecesInList[ply]--;
};

int Board::addPiece(Piece* piece, bool ply){
    pieceList[ply][numPiecesInList[ply]]=piece;
    return numPiecesInList[ply]++;
};

bool Board::isLegalMove(int from, int to, bool ply){
    //假設 from -> to 是合理的移動/吃/跳/翻牌的走法
    //假設 from 一定是自已的棋
    //不考慮翻牌
    return RuleTable::LEGAL_EAT_ARRAY[board[from]->piece][board[to]->piece];
};

bool Board::haveEatMove(){
    
    this->moveList.clear();
    for(int i = 0; i < 2 ; ++i){
        for(int j = 0 ; j < numPiecesInList[i] ; ++j){
            moveListGenerator->genPossibleEat(this, &(this->moveList), pieceList[i][j]);
        }
    }
    if(this->moveList.empty())return false;
    else return true;
};


void Board::initBoard(){

    //init piece
    for(int i = 0 ; i < 35 ; ++i){
        pieces[i].position = -1;
        pieces[i].inside = true;
        pieces[i].dark = true;
        pieces[i].indexInPieceList = -1;
        pieces[i].piece = convertPiecesIndexToPiece[i];
    }
    // empty
    pieces[33].dark = false;
    // outside
    pieces[34].inside = false;
    pieces[34].dark = false;
    

    //init board
    for(int i = 0; i < 60 ; ++i){
        if(i<11 || i > 48 || i%10==0 || i%10==9){
            board[i] = &pieces[34];
        }
        else
        {
            board[i] = &pieces[32];
        }
    }

    //pieceList[2][16]
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 16 ; ++j){
            pieceList[i][j] = nullptr;
        }
    }

    //numPiecesInList
    numPiecesInList[0] = numPiecesInList[1] = 0;

    //alivePieces
    for(int i = 0; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            alivePieces[i][j] = oriAlivePieces[j];
        }
    }

    //numAlivePieces
    numAlivePieces[0] = numAlivePieces[1] = 16;

    //convertPieceToPiecesIndex
    for(int i = 0 ; i < 18 ; ++i){
        convertPieceToPiecesIndex[i] = oriConvertPieceToPiecesIndex[i];
    }
    //darkPieceNumAll
    darkPieceNumAll = 32;

    //darkPieceNum  flipPossibility
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            darkPieceNum[i][j] = oriDarkPieceNum[i][j];
            flipPossibility[i][j] = darkPieceNum[i][j]*possibilityHelper[darkPieceNumAll];
        }
    }
    ply = 0;
    rootPly = 0;
    //moveListGenerator
    delete moveListGenerator;
    moveListGenerator = new EarlyGame();
};



std::pair<char,char> Board::genMove(){

    setMoveList(1);

    if(moveList[0].first==0)
    {
        // early move
        return moveList[1];
    }

    if(moveList.size()>=3)
    {
        return moveList[rand()%3];
    }
    else if(moveList.size()>0)
    {
        return moveList[0];
    }
    

    return std::make_pair(-1,-1);
};

void Board::setMoveList(int tag){
    moveListGenerator->handle(this);
    moveListGenerator->genMoveList(this,tag);
};

void Board::updateFlipPossibility(){
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            flipPossibility[i][j] = darkPieceNum[i][j]*possibilityHelper[darkPieceNumAll];
        }
    }
};

void Board::duplicate(Board* oldBoard){
    this->ply = oldBoard->ply;
    this->rootPly = oldBoard->rootPly;
    //pieceList[2][16]
    /*
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 16 ; ++j){
            this->pieceList[i][j] = nullptr;
        }
    }*/
    //copy piece and put it to board and pieceList
    for(int i = 0 ; i < 35 ; ++i){
        this->pieces[i].position = oldBoard->pieces[i].position;
        this->pieces[i].inside = oldBoard->pieces[i].inside;
        this->pieces[i].dark = oldBoard->pieces[i].dark;
        this->pieces[i].indexInPieceList = oldBoard->pieces[i].indexInPieceList;
        this->pieces[i].piece = oldBoard->pieces[i].piece;
        if(this->pieces[i].position!=-1)
        {
            this->board[pieces[i].position] = &(this->pieces[i]);
        }
        if(this->pieces[i].indexInPieceList!= -1)
        {
            this->pieceList[pieces[i].piece>>3][pieces[i].indexInPieceList] = &(this->pieces[i]);
        }
    }
    //copy numPiecesInList
    this->numPiecesInList[0] = oldBoard->numPiecesInList[0];
    this->numPiecesInList[1] = oldBoard->numPiecesInList[1];

    //copy numAlivePieces
    this->numAlivePieces[0] = oldBoard->numAlivePieces[0];
    this->numAlivePieces[1] = oldBoard->numAlivePieces[1];

    //copy convertPieceToPiecesIndex
    for(int i = 0 ; i < 18 ; ++i){
        this->convertPieceToPiecesIndex[i] = oldBoard->convertPieceToPiecesIndex[i];
    }

    //copy darkPieceNumAll
    this->darkPieceNumAll = oldBoard->darkPieceNumAll;
    //darkPieceNum  flipPossibility //copy alivePieces
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            this->darkPieceNum[i][j] = oldBoard->darkPieceNum[i][j];
            this->flipPossibility[i][j] = oldBoard->flipPossibility[i][j];
            this->alivePieces[i][j] = oldBoard->alivePieces[i][j];
        }
    }


    //copy board (empty / dark / outside)
    for(int i = 0; i < 60 ; ++i){
        if(i<11 || i > 48 || i%10==0 || i%10==9){
            this->board[i] = &(pieces[34]);
        }
        else if(oldBoard->board[i]->dark)
        {
            this->board[i] = &(pieces[32]);
        }
        else if(oldBoard->board[i]->piece == EMPTY)
        {
            this->board[i] = &(pieces[33]);
        }  
    }
    //moveListGenerator
    this->moveListGenerator = oldBoard->moveListGenerator->duplicate();
    this->moveListGenerator->handle(this);
}

Board::Board(Board* oldBoard){
    //std::cerr << "new Board" << std::endl;
    this->ply = oldBoard->ply;
    this->rootPly = oldBoard->rootPly;
    //pieceList[2][16]
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 16 ; ++j){
            this->pieceList[i][j] = nullptr;
        }
    }
    //copy piece and put it to board and pieceList
    for(int i = 0 ; i < 35 ; ++i){
        this->pieces[i].position = oldBoard->pieces[i].position;
        this->pieces[i].inside = oldBoard->pieces[i].inside;
        this->pieces[i].dark = oldBoard->pieces[i].dark;
        this->pieces[i].indexInPieceList = oldBoard->pieces[i].indexInPieceList;
        this->pieces[i].piece = oldBoard->pieces[i].piece;
        if(this->pieces[i].position!=-1)
        {
            this->board[pieces[i].position] = &(this->pieces[i]);
        }
        if(this->pieces[i].indexInPieceList!= -1)
        {
            this->pieceList[pieces[i].piece>>3][pieces[i].indexInPieceList] = &(this->pieces[i]);
        }
    }
    //copy numPiecesInList
    this->numPiecesInList[0] = oldBoard->numPiecesInList[0];
    this->numPiecesInList[1] = oldBoard->numPiecesInList[1];

    //copy alivePieces
    for(int i = 0; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            this->alivePieces[i][j] = oldBoard->alivePieces[i][j];
        }
    }
    //copy numAlivePieces
    this->numAlivePieces[0] = oldBoard->numAlivePieces[0];
    this->numAlivePieces[1] = oldBoard->numAlivePieces[1];

    //copy convertPieceToPiecesIndex
    for(int i = 0 ; i < 18 ; ++i){
        this->convertPieceToPiecesIndex[i] = oldBoard->convertPieceToPiecesIndex[i];
    }

    //copy darkPieceNumAll
    this->darkPieceNumAll = oldBoard->darkPieceNumAll;
    //darkPieceNum  flipPossibility
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            this->darkPieceNum[i][j] = oldBoard->darkPieceNum[i][j];
            this->flipPossibility[i][j] = oldBoard->flipPossibility[i][j];
        }
    }


    //copy board (empty / dark / outside)
    for(int i = 0; i < 60 ; ++i){
        if(i<11 || i > 48 || i%10==0 || i%10==9){
            this->board[i] = &(pieces[34]);
        }
        else if(oldBoard->board[i]->dark)
        {
            this->board[i] = &(pieces[32]);
        }
        else if(oldBoard->board[i]->piece == EMPTY)
        {
            this->board[i] = &(pieces[33]);
        }  
    }
    //moveListGenerator
    this->moveListGenerator = oldBoard->moveListGenerator->duplicate();
    this->moveListGenerator->handle(this);
};

int Board::getRootScore(){
    if(this->numAlivePieces[!ply]==0) return 8000;
    else if(this->numAlivePieces[ply]==0) return -8000;
    int score = 0;
    int offset = 0;
    if(rootPly)offset = 8;
    int myKillIndex = -1; int enemyKillIndex = -1;

    for(int i = 0 ; i < 18 ; ++i){
        RuleTable::CAN_EAT_NUM[i] = 0;
    }
    for(int i = 0 ; i < 15 ; ++i){
        if(i==7 || i==5 || i==13)  continue;
        for(int j = (i&7) ; j < 7 ; ++j)    
        {
            if(i<7)RuleTable::CAN_EAT_NUM[i] += this->alivePieces[!rootPly][j];
            else RuleTable::CAN_EAT_NUM[i] += this->alivePieces[rootPly][j];
        }    
    }
    RuleTable::CAN_EAT_NUM[0] -= this->alivePieces[!rootPly][6];
    RuleTable::CAN_EAT_NUM[8] -= this->alivePieces[rootPly][6];
    RuleTable::CAN_EAT_NUM[6] += this->alivePieces[!rootPly][0];
    RuleTable::CAN_EAT_NUM[14] += this->alivePieces[rootPly][0];
    RuleTable::CAN_EAT_NUM[5] = this->numAlivePieces[!rootPly] + (this->alivePieces[!rootPly][0]<<2) + (this->alivePieces[!rootPly][1]<<1);
    RuleTable::CAN_EAT_NUM[13] = this->numAlivePieces[rootPly] + (this->alivePieces[rootPly][0]<<2) + (this->alivePieces[rootPly][1]<<1);
    for(int i = 0 ; i < numPiecesInList[rootPly] ; ++i){
        int index = (pieceList[rootPly][i]->piece)-offset;
        if(index==6)
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[1][0]))+RuleTable::CAN_EAT_NUM[index]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[!rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else if(index==14)
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[0][0]))+RuleTable::CAN_EAT_NUM[index]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]+RuleTable::CAN_EAT_NUM[index]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(RuleTable::CAN_EAT_NUM[index]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        
    }
    for(int i = 0 ; i < numPiecesInList[!rootPly] ; ++i){
        int index = (pieceList[!rootPly][i]->piece)+offset;
        if(index==6)
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[1][0]))+RuleTable::CAN_EAT_NUM[index]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[!rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else if(index==14)
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[0][0]))+RuleTable::CAN_EAT_NUM[index]+1);
           // std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]+RuleTable::CAN_EAT_NUM[index]+1);
           // std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(RuleTable::CAN_EAT_NUM[index]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        
    }

    score += ((this->numAlivePieces[rootPly] - this->numAlivePieces[!rootPly])<<5);

    return score;
}
int Board::getNoMoveScore(){
    if(this->numAlivePieces[!ply]==0) return 8000;
    else if(this->numAlivePieces[ply]==0) return -8000;
    int score = 0;
    int offset = 0;
    if(rootPly)offset = 8;
    std::vector<Piece*> myKillerList;
    std::vector<Piece*> myKillList;
    std::vector<Piece*> enemyKillerList;
    std::vector<Piece*> enemyKillList;
    int myKillIndex = -1; int enemyKillIndex = -1;

    for(int i = 0 ; i < 18 ; ++i){
        RuleTable::CAN_EAT_NUM[i] = 0;
    }
    for(int i = 0 ; i < 15 ; ++i){
        if(i==7 || i==5 || i==13)  continue;
        for(int j = (i&7) ; j < 7 ; ++j)    
        {
            if(i<7)RuleTable::CAN_EAT_NUM[i] += this->alivePieces[!rootPly][j];
            else RuleTable::CAN_EAT_NUM[i] += this->alivePieces[rootPly][j];
        }    
    }
    RuleTable::CAN_EAT_NUM[0] -= this->alivePieces[!rootPly][6];
    RuleTable::CAN_EAT_NUM[8] -= this->alivePieces[rootPly][6];
    RuleTable::CAN_EAT_NUM[6] += this->alivePieces[!rootPly][0];
    RuleTable::CAN_EAT_NUM[14] += this->alivePieces[rootPly][0];
    RuleTable::CAN_EAT_NUM[5] = this->numAlivePieces[!rootPly] + (this->alivePieces[!rootPly][0]<<2) + (this->alivePieces[!rootPly][1]<<1);
    RuleTable::CAN_EAT_NUM[13] = this->numAlivePieces[rootPly] + (this->alivePieces[rootPly][0]<<2) + (this->alivePieces[rootPly][1]<<1);
    for(int i = 0 ; i < numPiecesInList[rootPly] ; ++i){
        int index = (pieceList[rootPly][i]->piece)-offset;
        if(index==6)
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[1][0]))+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[!rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else if(index==14)
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[0][0]))+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(RuleTable::CAN_EAT_NUM[index]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        
       // moveListGenerator->possibleEatList(this,&myKillerList, &myKillList, &myKillIndex, pieceList[rootPly][i]);
    }
    for(int i = 0 ; i < numPiecesInList[!rootPly] ; ++i){
        int index = (pieceList[!rootPly][i]->piece)+offset;
        if(index==6)
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[1][0]))+RuleTable::CAN_EAT_NUM[index]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[!rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else if(index==14)
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[0][0]))+RuleTable::CAN_EAT_NUM[index]+1);
           // std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]+RuleTable::CAN_EAT_NUM[index]+1);
           // std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(RuleTable::CAN_EAT_NUM[index]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
       // moveListGenerator->possibleEatList(this, &enemyKillerList, &enemyKillList, &enemyKillIndex, pieceList[!rootPly][i]);
    }
    //std::cerr << "========"<< std::endl;
    std::cerr << "out" << std::endl;
    while(myKillIndex>=0 || enemyKillIndex>=0){ //有可以a吃到b後跑掉 c吃不到a 但是先不考慮
        std::cerr << "in" << std::endl;
        while(myKillIndex>=0){
            if(myKillerList[myKillIndex]!=nullptr)
            {
                score += (RuleTable::PIECE_SCORE_BASIC[(myKillList[myKillIndex]->piece)+offset]+RuleTable::CAN_EAT_NUM[(myKillList[myKillIndex]->piece)+offset]+1);
                for(int x = myKillIndex-1 ; x >= 0 ; --x){
                    if(myKillList[x]!=nullptr && myKillList[myKillIndex]==myKillList[x])
                    {
                        myKillerList[x] = nullptr;
                        myKillList[x] = nullptr;
                    }
                }
                for(int x = enemyKillIndex ; x >= 0 ; --x){
                    if(enemyKillerList[x]!=nullptr && myKillList[myKillIndex]==enemyKillerList[x])
                    {
                        enemyKillerList[x] = nullptr;
                        enemyKillList[x] = nullptr;
                    }
                }
                myKillIndex--;
                break;
            }
            else
            {
                myKillIndex--;
            }
        }
        while(enemyKillIndex>=0){
            if(enemyKillerList[enemyKillIndex]!=nullptr)
            {
                score -= (RuleTable::PIECE_SCORE_BASIC[(myKillList[myKillIndex]->piece)-offset]+RuleTable::CAN_EAT_NUM[(myKillList[myKillIndex]->piece)-offset]+1);
                for(int x = enemyKillIndex-1 ; x >= 0 ; --x){
                    if(enemyKillList[x]!=nullptr && enemyKillList[myKillIndex]==enemyKillList[x])
                    {
                        enemyKillerList[x] = nullptr;
                        enemyKillList[x] = nullptr;
                    }
                }
                for(int x = myKillIndex ; x >= 0 ; --x){
                    if(myKillerList[x]!=nullptr && enemyKillList[enemyKillIndex]==myKillerList[x])
                    {
                        myKillerList[x] = nullptr;
                        myKillList[x] = nullptr;
                    }
                }
                enemyKillIndex--;
                break;
            }
            else
            {
                enemyKillIndex--;
            }            
        }   
    }  
    
   score += ((this->numAlivePieces[rootPly] - this->numAlivePieces[!rootPly])<<5);

    return score;
}
int Board::getScore(){

    int score = 0;
    int offset = 0;
    if(rootPly)offset = 8;
    std::vector<Piece*> myKillerList;
    std::vector<Piece*> myKillList;
    std::vector<Piece*> enemyKillerList;
    std::vector<Piece*> enemyKillList;
    int myKillIndex = -1; int enemyKillIndex = -1;

    for(int i = 0 ; i < 18 ; ++i){
        RuleTable::CAN_EAT_NUM[i] = 0;
    }
    for(int i = 0 ; i < 15 ; ++i){
        if(i==7 || i==5 || i==13)  continue;
        for(int j = (i&7) ; j < 7 ; ++j)    
        {
            if(i<7)RuleTable::CAN_EAT_NUM[i] += this->alivePieces[!rootPly][j];
            else RuleTable::CAN_EAT_NUM[i] += this->alivePieces[rootPly][j];
        }    
    }
    RuleTable::CAN_EAT_NUM[0] -= this->alivePieces[!rootPly][6];
    RuleTable::CAN_EAT_NUM[8] -= this->alivePieces[rootPly][6];
    RuleTable::CAN_EAT_NUM[6] += this->alivePieces[!rootPly][0];
    RuleTable::CAN_EAT_NUM[14] += this->alivePieces[rootPly][0];
    RuleTable::CAN_EAT_NUM[5] = this->numAlivePieces[!rootPly] + (this->alivePieces[!rootPly][0]<<2) + (this->alivePieces[!rootPly][1]<<1);
    RuleTable::CAN_EAT_NUM[13] = this->numAlivePieces[rootPly] + (this->alivePieces[rootPly][0]<<2) + (this->alivePieces[rootPly][1]<<1);
    for(int i = 0 ; i < numPiecesInList[rootPly] ; ++i){
        int index = (pieceList[rootPly][i]->piece)-offset;
        if(index==6)
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[1][0]))+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[!rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else if(index==14)
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[0][0]))+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else
        {
            score += (RuleTable::PIECE_SCORE_BASIC[index]+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(RuleTable::CAN_EAT_NUM[index]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        
       // moveListGenerator->possibleEatList(this,&myKillerList, &myKillList, &myKillIndex, pieceList[rootPly][i]);
    }
    for(int i = 0 ; i < numPiecesInList[!rootPly] ; ++i){
        int index = (pieceList[!rootPly][i]->piece)+offset;
        if(index==6)
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[1][0]))+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[!rootPly][i]->position]+1);
            //std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[!rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else if(index==14)
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[0][0]))+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[!rootPly][i]->position]+1);
           // std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(alivePieces[rootPly][0]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
        else
        {
            score -= (RuleTable::PIECE_SCORE_BASIC[index]+RuleTable::CAN_EAT_NUM[index]+RuleTable::POSITION_SCORE[pieceList[!rootPly][i]->position]+1);
           // std::cerr << index << "  -> " << RuleTable::PIECE_SCORE_BASIC[index]*int(bool(RuleTable::CAN_EAT_NUM[index]))+RuleTable::CAN_EAT_NUM[index]+1 << std::endl;
        }
       // moveListGenerator->possibleEatList(this, &enemyKillerList, &enemyKillList, &enemyKillIndex, pieceList[!rootPly][i]);
    }
    //std::cerr << "========"<< std::endl;
    /*
    if(myKillIndex>=0 && enemyKillIndex>=0){ //有可以a吃到b後跑掉 c吃不到a 但是先不考慮
        while(myKillIndex>=0){
            if(myKillerList[myKillIndex]!=nullptr)
            {
                score += RuleTable::PIECE_SCORE_BASIC[(myKillList[myKillIndex]->piece)+offset];
                for(int x = myKillIndex-1 ; x >= 0 ; --x){
                    if(myKillList[x]!=nullptr && myKillList[myKillIndex]==myKillList[x])
                    {
                        myKillerList[x] = nullptr;
                        myKillList[x] = nullptr;
                    }
                }
                for(int x = enemyKillIndex ; x >= 0 ; --x){
                    if(enemyKillerList[x]!=nullptr && myKillList[myKillIndex]==enemyKillerList[x])
                    {
                        enemyKillerList[x] = nullptr;
                        enemyKillList[x] = nullptr;
                    }
                }
                myKillIndex--;
                break;
            }
            else
            {
                myKillIndex--;
            }
        }
        while(enemyKillIndex>=0){
            if(enemyKillerList[enemyKillIndex]!=nullptr)
            {
                score -= RuleTable::PIECE_SCORE_BASIC[(enemyKillList[enemyKillIndex]->piece)-offset];
                for(int x = enemyKillIndex-1 ; x >= 0 ; --x){
                    if(enemyKillList[x]!=nullptr && enemyKillList[myKillIndex]==enemyKillList[x])
                    {
                        enemyKillerList[x] = nullptr;
                        enemyKillList[x] = nullptr;
                    }
                }
                for(int x = myKillIndex ; x >= 0 ; --x){
                    if(myKillerList[x]!=nullptr && enemyKillList[enemyKillIndex]==myKillerList[x])
                    {
                        myKillerList[x] = nullptr;
                        myKillList[x] = nullptr;
                    }
                }
                enemyKillIndex--;
                break;
            }
            else
            {
                enemyKillIndex--;
            }            
        }   
    }  
    */
   score += ((this->numAlivePieces[rootPly] - this->numAlivePieces[!rootPly])<<4);

    return score;
};

int Board::getState(){
    return moveListGenerator->getState();
}


void Board::printBoard(){
    /*
    for(int i = 11 ; i < 49 ; ++i){
        if(i%10==9)
        {
            std::cout << std::endl;
        }
        else if(i%10==0)
        {

        }
        else if(board[i]->piece >= 0 && board[i]->piece < 10)
        {
            std::cout << "  " << int(board[i]->piece) << " ||";
        }
        else
        {
            std::cout << " "<< int(board[i]->piece) << " ||";
        }   
    }
    std::cout << std::endl;
    */
};
