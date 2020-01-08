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


const int Board::oriAlivePieces[7] = {1,2,2,2,2,2,5};
const int Board::oriConvertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,-1,16,17,19,21,23,25,27,-1,32,-1};
const int Board::convertPiecesIndexToPiece[35] = {0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,8,9,9,10,10,11,11,12,12,13,13,14,14,14,14,14,16,17,18};
const int Board::oriDarkPieceNum[2][7] = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
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
        board[to]->position = 0;
        alivePieces[board[to]->piece>>3][board[to]->piece&7]--;
        numAlivePieces[(board[to]->piece>>3)]--;
        removePiece(board[to]->indexInPieceList, board[to]->piece>>3);
        board[to]->indexInPieceList = -1;
        board[to]->piece = 17; // dead
        board[to]->inside = false;
    }
    Piece* tmp = board[to];
    board[to] = board[from]; 
    board[from] = tmp;
    board[from]->position = from;
    board[to]->position = to;
    ply = !ply;
    return true;
};

bool Board::flip(char pos, char piece){
    std::cerr << "Debug : in flip" << std::endl;
    board[pos] = &(pieces[convertPieceToPiecesIndex[piece]]);
    convertPieceToPiecesIndex[piece]++;
    board[pos]->dark = false;
    board[pos]->position = pos;
    board[pos]->inside = true;
    board[pos]->indexInPieceList = addPiece(board[pos], board[pos]->piece>>3);
    darkPieceNumAll--;
    darkPieceNum[piece>>3][piece&7]--;
    updateFlipPossibility();
    std::cerr << "Debug : in addPiece numPiecesInList is " << numPiecesInList[board[pos]->piece>>3] << std::endl;
    ply = !ply;
    return true;
};

void Board::removePiece(int index, bool ply){

    if(pieceList[ply][index+1]==nullptr)
    {
        pieceList[ply][index] = nullptr;
    }
    else
    {
        for(int i = index+2 ; i < 16 ; ++i){
            if(pieceList[ply][i]==nullptr || i == 15)
            {   
                if(pieceList[ply][i]!=nullptr) // 到底了 且是滿的
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
    std::cerr << "Debug : in addPiece" << std::endl;
    pieceList[ply][numPiecesInList[ply]]=piece;
    return numPiecesInList[ply]++;
};

bool Board::isLegalMove(int from, int to, bool ply){
    //假設 from -> to 是合理的移動/吃/跳/翻牌的走法
    //假設 from 一定是自已的棋
    //不考慮翻牌
    return RuleTable::LEGAL_EAT_ARRAY[board[from]->piece][board[to]->piece];
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
        if(i<10 || i > 49 || i%10==0 || i%10==9){
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
    //moveListGenerator
    delete moveListGenerator;
    moveListGenerator = new EarlyGame();
};

/*
bool Board::firstMove(char pos, char piece){
    board[pos] = &(pieces[convertPieceToPiecesIndex[piece]]);
    board[pos]->dark = false;
    board[pos]->inside = true;
    board[pos]->indexInPieceList = addPiece(board[pos], board[pos]->piece>>3);
    ply = !(board[pos]->piece>>3);
    return true;
};
*/

std::pair<char,char> Board::genMove(){

    setMoveList();

    if(moveList[0].first==0)
    {
        // early move
        return moveList[1];
    }

    for(int i = 0; i < moveList.size() ; ++i){
        std::cerr << "Index : " << i << "  move : " << int(moveList[i].first) << "," << int(moveList[i].second) << std::endl;
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

void Board::setMoveList(){
    moveListGenerator->handle(this);
    moveListGenerator->genMoveList(this);
};

void Board::updateFlipPossibility(){
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            flipPossibility[i][j] = darkPieceNum[i][j]*possibilityHelper[darkPieceNumAll];
        }
    }
};

Board::Board(Board* oldBoard){
    ply = oldBoard->ply;
    //pieceList[2][16]
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 16 ; ++j){
            pieceList[i][j] = nullptr;
        }
    }

    //copy piece and put it to board and pieceList
    for(int i = 0 ; i < 35 ; ++i){
        pieces[i].position = oldBoard->pieces[i].position;
        pieces[i].inside = oldBoard->pieces[i].inside;
        pieces[i].dark = oldBoard->pieces[i].dark;
        pieces[i].indexInPieceList = oldBoard->pieces[i].indexInPieceList;
        pieces[i].piece = oldBoard->pieces[i].piece;
        board[pieces[i].position] = &pieces[i];
        pieceList[pieces[i].piece>>3][pieces[i].indexInPieceList] = &pieces[i];
    }

    //copy numPiecesInList
    numPiecesInList[0] = oldBoard->numPiecesInList[0];
    numPiecesInList[1] = oldBoard->numPiecesInList[1];

    //copy alivePieces
    for(int i = 0; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            alivePieces[i][j] = oldBoard->alivePieces[i][j];
        }
    }

    //copy numAlivePieces
    numAlivePieces[0] = oldBoard->numAlivePieces[0];
    numAlivePieces[1] = oldBoard->numAlivePieces[1];

    //copy convertPieceToPiecesIndex
    for(int i = 0 ; i < 18 ; ++i){
        convertPieceToPiecesIndex[i] = oldBoard->convertPieceToPiecesIndex[i];
    }

    //copy darkPieceNumAll
    darkPieceNumAll = oldBoard->darkPieceNumAll;

    //darkPieceNum  flipPossibility
    for(int i = 0 ; i < 2 ; ++i){
        for(int j = 0 ; j < 7 ; ++j){
            darkPieceNum[i][j] = oldBoard->darkPieceNum[i][j];
            flipPossibility[i][j] = oldBoard->flipPossibility[i][j];
        }
    }


    //copy board (empty / dark / outside)
    for(int i = 0; i < 60 ; ++i){
        if(i<10 || i > 49 || i%10==0 || i%10==9){
            board[i] = &pieces[34];
        }
        else if(oldBoard->board[i]->dark)
        {
            board[i] = &pieces[32];
        }
        else if(oldBoard->board[i]->piece == EMPTY)
        {
            board[i] = &pieces[33];
        }  
    }

    //moveListGenerator
    moveListGenerator->handle(this);
};


int Board::getScore(){

    int score = 0;
    if(ply) // 如果我是黑子
    {
        for(int i = 0 ; i < numPiecesInList[ply] ; ++i){
            score += RuleTable::PIECE_SCORE[pieceList[ply][i]->piece-8];   
        }
        
        for(int i = 0 ; i < numPiecesInList[!ply] ; ++i){
            score -= RuleTable::PIECE_SCORE[pieceList[!ply][i]->piece+8];
        }
    }
    else //如果我是紅子
    {
        for(int i = 0 ; i < numPiecesInList[ply] ; ++i){
            score += RuleTable::PIECE_SCORE[pieceList[ply][i]->piece];   
        }
        
        for(int i = 0 ; i < numPiecesInList[!ply] ; ++i){
            score -= RuleTable::PIECE_SCORE[pieceList[!ply][i]->piece];
        }
    }
    return score;
};