#include "Piece.hpp"
#include <vector>

class Board{
    public:
        static const int oriAlivePieces[7];
        static const int oriConvertPieceToPiecesIndex[18];
        static const int convertPiecesIndexToPiece[35]; 
        Piece* board[60];
        bool ply;
        Piece pieces[35];
        Piece* pieceList[2][16];
        int numPiecesInList[2];   
        int alivePieces[2][7]  = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
        int numAlivePieces[2];
        int convertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,-1,16,17,19,21,23,25,27,-1,32,-1};
        std::vector<std::pair<char,char>> moveList();
        void initBoard(); // Version 0.4.3 
        bool move(char from, char to); // Version 0.4.3
        bool flip(char pos, char piece); // Version 0.4.3
        void removePiece(int index, bool ply); //removePieceFromPieceList Version 0.4.3
        int addPiece(Piece* piece, bool ply); //addPieceToPieceList Version 0.4.3
        int getScore();
        std::pair<char,char> genMove();
        bool isLegalMove(int from, int to, bool ply);
        bool firstMove(char pos, char piece); //Version 0.4.3

        Board(){

        };
};

