#ifndef BOARD
#define BOARD
#include "Piece.hpp"
#include "RuleTable.hpp"
#include "IMoveListGenerator.hpp"
#include <vector>

class Board{
    public:
        static const int oriAlivePieces[7];
        static const int oriConvertPieceToPiecesIndex[18];
        static const int convertPiecesIndexToPiece[35]; 
        IMoveListGenerator* moveListGenerator;
        std::vector<std::pair<char, char>>* moveList;
        Piece* board[60];
        bool ply;
        Piece pieces[35];
        Piece* pieceList[2][16];
        int numPiecesInList[2];   
        int alivePieces[2][7]  = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
        int numAlivePieces[2];
        int convertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,-1,16,17,19,21,23,25,27,-1,32,-1};
        void initBoard(); // Version 0.4.3 
        bool move(char from, char to); // Version 0.4.3
        bool flip(char pos, char piece); // Version 0.4.3
        void removePiece(int index, bool ply); //removePieceFromPieceList Version 0.4.3
        int addPiece(Piece* piece, bool ply); //addPieceToPieceList Version 0.4.3
        int getScore();
        
        std::pair<char,char> genMove();
        void setMoveListGenerator(); // set by current board
        void setMoveList(); //Version 0.5.1
        bool isLegalMove(int from, int to, bool ply); //Version 0.5.1
        //缺genFlip
        //bool firstMove(char pos, char piece); //Version 0.4

        Board(){

        };
};

#endif //BOARD