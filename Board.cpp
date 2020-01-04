#include "Board.hpp"

bool Board::move(char from, char to){
    // 假設傳進來的一定legal

    // move or eat

    if(board[to]->piece != 17) // eat
    { 
        board[to]->position = 0;
        alivePieces[board[to]->piece>>3][board[to]->piece&7]--;
        numAlivePieces[int(board[to]->piece>>3)]--;
        removePiece(board[to]->indexInPieceList, board[to]->piece>>3);
        board[to]->indexInPieceList = -1;
        board[to]->piece = 17; // dead
        board[to]->inside = false;
    }
    board[to] = board[from]; 
    board[from] = &(pieces[32]);
    board[to]->position = to;
    ply = !ply;
    return true;
};

bool Board::flip(char pos, char piece){
    board[pos]->dark = false;
    board[pos]->piece = piece;
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
                    pieceList[ply][i] = nullptr;
                }
                else 
                {
                    pieceList[ply][index] = pieceList[ply][i-1];
                    pieceList[ply][i-1] = nullptr;
                }
                break;
            }
        }
    }
    numPiecesInList[ply]--;
};

int Board::addPiece(Piece* piece, bool ply){
    numPiecesInList[ply]++;
    pieceList[ply][numPiecesInList[ply]]=piece;
    return numPiecesInList[ply];
};

bool Board::isLegalMove(int from, int to, bool ply){
    //假設 from -> to 是合理的移動/吃/跳/翻牌的走法
    if(from==to) // is flip
    {
        if(board[from]->dark)
        {
            return true;
        }
    }
    else // other
    {
        if(board[from]->piece != 17 && (((board[from]->piece>>3) == ply))) // is my piece
        { 
            // return RuleTable::legalEatArray[board[from]->piece][board[to]->piece];
            return true;   
        }
    }
    return false;
};

void Board::initBoard(){
    int j = 0;
    for(int i = 0; i < 60 ; ++i){
        if(i<10 || i > 39 || i%10==0 || i%10==9){
            board[i] = &pieces[33];
            board[i]->position = i;
            board[i]->inside = false;
            board[i]->dark = false;
            board[i]->indexInPieceList = -1;
            board[i]->piece = 18;
        }
        else
        {
            board[i] = &pieces[j];
            board[i]->position = i;
            board[i]->inside = true;
            board[i]->dark = false;
            board[i]->indexInPieceList = -1;
            board[i]->piece = 16;
            j++;
        }
    }
    //只有32不會被指到 // 0~31各指一格 
};

bool Board::firstMove(char pos, char piece){
    board[pos]->dark = false;
    board[pos]->piece = piece;
    board[pos]->indexInPieceList = addPiece(board[pos], board[pos]->piece>>3);
    ply = !(board[pos]->piece>>3);
    return true;
};