#include "MidGame.hpp"
#include "Board.hpp"
#include <vector>

const char MidGame::moveOrder[7] = {UK, UG, UM, UR, UN, UC, UP}; 
const char MidGame::eatOrder[7] = {UK, UG, UM, UR, UN, UC, UP}; 
const char MidGame::mixOrder[14] = {1,1,1,1,1,1,1,0,0,0,0,0,0,0}; // 0 = move | 1 = eat
std::vector<std::pair<char,char>> MidGame::moveVector[14];


void MidGame::genMoveList(Board* board) const{
    board->moveList.clear();
    
    //clean
    for(int i = 0 ; i < sizeof(mixOrder) ; ++i) moveVector[i].clear();
    //genPossibleMove
    //std::cerr << "XXX" << std::endl;
    for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
        Piece* currentPiece = board->pieceList[board->ply][i];
        //std::cerr << "currentPiece" << int(currentPiece->piece) << "," << int(((currentPiece->piece)&7)+7) << std::endl;
        genPossibleEat(board, &(moveVector[((currentPiece->piece)&7)+7]), currentPiece);
        genPossibleMove(board, &(moveVector[((currentPiece->piece)&7)]), currentPiece);
    }
    int m_index = 0;
    int e_index = 0;
    //ordering

    for(int i = 0 ; i < sizeof(mixOrder) ; ++i)
    {
        if(mixOrder[i]==0)
        {
            if(moveVector[moveOrder[m_index]].empty())
            {
                m_index++;
                continue;
            }
            board->moveList.insert(board->moveList.end(), moveVector[moveOrder[m_index]].begin(), moveVector[moveOrder[m_index]].end());
            //std::cerr << "Debug : in move" << m_index  << " index:" << int(moveOrder[m_index]) << " size : " << board->moveList.size() << std::endl;
            m_index++;
        }
        else
        { 
            if(moveVector[eatOrder[e_index]+7].empty())
            {
               e_index++;
                continue;
            }
            board->moveList.insert(board->moveList.end(), moveVector[eatOrder[e_index]+7].begin(), moveVector[eatOrder[e_index]+7].end());
            //std::cerr << "Debug : in eat" << e_index  << " index:" << int(eatOrder[e_index]) <<" size : " << board->moveList.size() << std::endl;
            e_index++;
        }
    }
    //std::cerr << "genmove" << board->moveList.size() << std::endl;
    //append flip
    if(board->darkPieceNumAll>0) genPossibleFlipPosition(board, &(board->moveList));
    //std::cerr << "[genmove]" << board->moveList.size() << std::endl;
};


MidGame::~MidGame(){
    
};