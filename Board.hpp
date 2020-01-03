#include "myDefine.hpp"
#include "Piece.hpp"

class Board{
    public:
        Piece* board[60];
        bool ply;
        Piece pieces[33];
        Piece* pieceList[2][16];
        int numPieces[2];
        int alivePieces[2][7];
};


/*
- currentBoard[60]
- turn : bool
- pieces : Piece[33]
- pieceList : pointer of Piece [2][16]
- numPieces[2] : int
*/