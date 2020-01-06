#include "RandomMove.hpp"
#include "Board.hpp"
#include <random>

void RandomMove::genMoveList(Board* board) const{
    
    board->moveList.clear();
    for(int i = 0 ; i < board->numPiecesInList[board->ply] ; ++i){
        Piece* currentPiece = board->pieceList[board->ply][i];
        std::vector<std::pair<char,char>> moveVector;
        genPossibleMoveAndEat(board, &moveVector, currentPiece);
        board->moveList.insert(board->moveList.end(), moveVector.begin(), moveVector.end());
    }

    genPossibleFlip(board, &(board->moveList));

    int randIndex = rand()%board->moveList.size();
    
    std::pair<char,char> randomMove = board->moveList[randIndex];
    /*
    for(int i = 0 ; i < board->moveList.size() ; ++i){
        std::cerr << "MoveList : [" << i << "] " << int(board->moveList[i].first) << "," << int(board->moveList[i].second) << std::endl;
    }
    */
    //std::cerr << "Debug : randIndex = " << randIndex << " randMove.first / second = " << int(randomMove.first) << "," << int(randomMove.second) << std::endl;
    board->moveList.clear();
    board->moveList.emplace_back(std::make_pair(0,0));
    board->moveList.emplace_back(randomMove);
};