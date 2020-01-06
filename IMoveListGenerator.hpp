
#ifndef IMOVELISTGENERATOR
#define IMOVELISTGENERATOR
#include <vector>


class Board;
class Piece;
class RuleTable;

class IMoveListGenerator
{
    public:
        virtual void genMoveList(Board* board) const = 0;
        static void genPossibleMoveAndEat(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece);
        static void genPossibleMove(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece);
        static void genPossibleEat(Board* board, std::vector<std::pair<char, char>>* moveVector, Piece* piece);
        static void genPossibleFlip(Board* board, std::vector<std::pair<char, char>>* moveVector);
};

#include "EarlyGame.hpp"
#include "EnemyWithKing.hpp"
#include "EnemyWithoutKing.hpp"
#include "RandomMove.hpp"

#endif //IMOVELISTGENERATOR