#ifndef BOARD
#define BOARD
#include "Piece.hpp"
#include "RuleTable.hpp"
#include "MoveListGenerator.hpp"
#include <vector>

#define RK 0
#define RG 1
#define RM 2
#define RR 3
#define RN 4
#define RC 5
#define RP 6
#define BK 8
#define BG 9
#define BM 10 
#define BR 11
#define BN 12
#define BC 13
#define BP 14
#define UK 0
#define UG 1
#define UM 2
#define UR 3
#define UN 4
#define UC 5
#define UP 6
#define DARK 16
#define EMPTY 17
#define DEAD 17


class Board{
    public:
        static const char oriAlivePieces[7];
        static const char oriConvertPieceToPiecesIndex[18];
        static const char convertPiecesIndexToPiece[35]; 
        static const char oriDarkPieceNum[2][7];
        static const double possibilityHelper[33];
        MoveListGenerator* moveListGenerator;
        std::vector<std::pair<char, char>> moveList;
        Piece* board[60];
        bool rootPly;
        bool ply;
        Piece pieces[35];
        Piece* pieceList[2][16];
        char numPiecesInList[2];   
        char alivePieces[2][7]  = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
        char numAlivePieces[2];
        char convertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,60,16,17,19,21,23,25,27,33,32,34};
        double flipPossibility [2][7];
        char darkPieceNum [2][7];
        char darkPieceNumAll;

        void initBoard(); // Version 0.4.3 
        bool move(char from, char to); // Version 0.4.3
        bool flip(char pos, char piece); // Version 0.4.3
        void removePiece(int index, bool ply); //removePieceFromPieceList Version 0.4.3
        int addPiece(Piece* piece, bool ply); //addPieceToPieceList Version 0.4.3
        std::pair<char,char> genMove();
        void setMoveList(int tag); //Version 0.5.1
        bool haveEatMove();
        bool isLegalMove(int from, int to, bool ply); //Version 0.5.1
        void updateFlipPossibility();
        int getScore();
        int getNoMoveScore();
        int getRootScore();
        void printBoard();
        void duplicate(Board*);
        int getState();

        Board(){

        };

        Board(Board*);
};

#endif //BOARD