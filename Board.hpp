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
#define EMPTY 17
#define DEAD 17

class Board{
    public:
        static const int oriAlivePieces[7];
        static const int oriConvertPieceToPiecesIndex[18];
        static const int convertPiecesIndexToPiece[35]; 
        static const int oriDarkPieceNum[2][7];
        static const double possibilityHelper[33];
        MoveListGenerator* moveListGenerator;
        std::vector<std::pair<char, char>> moveList;
        Piece* board[60];
        bool ply;
        Piece pieces[35];
        Piece* pieceList[2][16];
        int numPiecesInList[2];   
        int alivePieces[2][7]  = {{1,2,2,2,2,2,5},{1,2,2,2,2,2,5}};
        int numAlivePieces[2];
        int convertPieceToPiecesIndex[18] = {0,1,3,5,7,9,11,-1,16,17,19,21,23,25,27,-1,32,-1};
        double flipPossibility [2][7];
        int darkPieceNum [2][7];
        int darkPieceNumAll;

        void initBoard(); // Version 0.4.3 
        bool move(char from, char to); // Version 0.4.3
        bool flip(char pos, char piece); // Version 0.4.3
        void removePiece(int index, bool ply); //removePieceFromPieceList Version 0.4.3
        int addPiece(Piece* piece, bool ply); //addPieceToPieceList Version 0.4.3
        
        std::pair<char,char> genMove();
        void setMoveList(); //Version 0.5.1
        bool isLegalMove(int from, int to, bool ply); //Version 0.5.1
        void updateFlipPossibility();

        Board(){

        };
};

#endif //BOARD