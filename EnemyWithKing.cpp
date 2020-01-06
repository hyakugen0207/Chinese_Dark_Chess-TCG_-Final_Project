#include "EnemyWithKing.hpp"
#include "Board.hpp"

const char EnemyWithKing::moveOrder[7] = {UG, UK, UC, UP, UM, UR, UN}; 
const char EnemyWithKing::eatOrder[7] = {UP, UC, UG, UK, UM, UR, UN}; 
const char EnemyWithKing::mixOrder[14] = {1,1,1,0,1,0,1,1,1,0,0,0,0,0}; // 0 = move | 1 = eat

//兵吃人基本上不會虧 / 炮不吃可能被跑掉 / 帥吃將或士用rule解決

void EnemyWithKing::genMoveList(Board* board) const{
    board->moveList.clear();
    
    //clean
    for(int i = 0 ; i < sizeof(mixOrder) ; ++i) moveVector[i].clear();

    //genPossibleMove
    for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
        Piece* currentPiece = board->pieceList[board->ply][i];
        genPossibleEat(board, &moveVector[(currentPiece->piece&7)+7], currentPiece);
        genPossibleMove(board, &moveVector[(currentPiece->piece&7)], currentPiece);
    }

    int m_index = 0;
    int e_index = 0;
    //ordering
    for(int i = 0 ; i < sizeof(mixOrder) ; ++i)
    {
        if(mixOrder[i]==0)
        {
            board->moveList.insert(board->moveList.end(), moveVector[moveOrder[m_index]].begin(), moveVector[moveOrder[m_index]].end());
            m_index++;
        }
        else
        {
            board->moveList.insert(board->moveList.end(), moveVector[eatOrder[e_index]].begin(), moveVector[eatOrder[e_index]].end());
            e_index++;
        }
    }
    
    //append flip
    genPossibleFlip(board, &(board->moveList));
};