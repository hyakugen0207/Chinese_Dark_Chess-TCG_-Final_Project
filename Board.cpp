#include "Board.hpp"

const int Board::oriAlivePieces[7] = {1,2,2,2,2,2,5};
const int Board::oriConvertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,-1,16,17,19,21,23,25,27,-1,32,-1};
const int Board::convertPiecesIndexToPiece[35] = {0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,8,9,9,10,10,11,11,12,12,13,13,14,14,14,14,14,16,17,18};

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
    board[to] = board[from]; 
    board[from] = &(pieces[33]);
    board[to]->position = to;
    ply = !ply;
    return true;
};

bool Board::flip(char pos, char piece){
    board[pos] = &(pieces[convertPieceToPiecesIndex[piece]]);
    convertPieceToPiecesIndex[piece]++;
    board[pos]->dark = false;
    board[pos]->position = pos;
    board[pos]->inside = true;
    board[pos]->indexInPieceList = addPiece(board[pos], board[pos]->piece>>3);
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
    for(int i = 0; i < 15 ; ++i){
        if(board[i]->dark == true){
            return std::make_pair(char(i),char(i));
        }
    }
    return std::make_pair(-1,-1);
};

void Board::setMoveList(){
    moveListGenerator->genMoveList(this);
};