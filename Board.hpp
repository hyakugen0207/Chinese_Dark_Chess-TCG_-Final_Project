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
        int convertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,-1,16,17,19,21,23,25,27,-1,32,-1};
        const int convertPiecesIndexToPiece[35] = {0,1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,8,9,9,10,10,11,11,12,12,13,13,14,14,14,14,14,16,17,17}; 
        std::vector<std::pair<char,char>> moveList();
        void initBoard();
        bool move(char from, char to);
        bool flip(char pos, char piece);
        void removePiece(int index, bool ply); //removePieceFromPieceList
        int addPiece(Piece* piece, bool ply); //addPieceToPieceList
        int getScore();
        bool isLegalMove(int from, int to, bool ply);
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