#include "myDefine.hpp"
#include "Piece.hpp"
#include <vector>

class Board{
    public:
        Piece* board[60];
        bool ply;
        Piece pieces[33];
        Piece* pieceList[2][16];
        int numPieces[2];
        int alivePieces[2][7];

        std::vector<std::pair<char,char>> moveList();
        bool move(char from, char dir);
        void removePiece(int index, bool ply); //removePieceFromPieceList
        int getScore();
        bool isLegalMove(int from, int to, bool ply);
};


/*
- currentBoard[60]
- turn : bool
- pieces : Piece[33]
- pieceList : pointer of Piece [2][16]
- numPieces[2] : int

+ moveList() : vector<pair<from,dir>>
+ move(from, dir) : bool
+ removePiece(int index, int color) : void // removePieceFromPieceList
+ getScore() : int
+ isLegalMove(int from, int to, bool turn) : bool
*/