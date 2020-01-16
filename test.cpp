#include <vector>
#include "ZobristHashTable.hpp"
#include "Board.hpp"
#include <iostream>

class AAA{
    public:
        int a; 

    AAA(int b){
        a = b;
    }
};


void pp(std::vector<std::vector<int>> v){
    for(int i = 0 ; i < v.size() ; ++i){
        for(int j = 0 ; j < v[i].size() ; ++j){
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(){
    ZobristHashTable::initStaticValue();

    std::cout << ZobristHashTable::hashNodes[0].depth << std::endl;
    std::cout << int(ZobristHashTable::hashNodes[0].exact) << std::endl;
    std::cout << ZobristHashTable::hashNodes[0].key << std::endl;
    std::cout << ZobristHashTable::hashNodes[0].nextMove.first << std::endl;
    std::cout << ZobristHashTable::hashNodes[0].value << std::endl;
    return 0;
};