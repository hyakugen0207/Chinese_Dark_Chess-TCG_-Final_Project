#include "myDefine.hpp"
#include "Piece.hpp"
#include <vector>

class Board{
    public:
        Piece* board[60];
        bool ply;
        Piece pieces[34];
        Piece* pieceList[2][16];
        int numPiecesInList[2];
        int alivePieces[2][7]  = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
        int numAlivePieces[2];

        std::vector<std::pair<char,char>> moveList();
        bool move(char from, char to);
        bool flip(char pos, char piece);
        void removePiece(int index, bool ply); //removePieceFromPieceList
        int addPiece(Piece* piece, bool ply); //addPieceToPieceList
        int getScore();
        bool isLegalMove(int from, int to, bool ply);
        void initBoard();
        bool firstMove(char pos, char piece);
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