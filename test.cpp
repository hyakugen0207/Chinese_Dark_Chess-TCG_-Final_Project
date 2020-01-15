#include <vector>
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
    std::vector<std::vector<int>> killerList;
    std::vector<int> a;


    for(int i = 0 ; i < 3 ; ++i){
        a.emplace_back(i);
        a.emplace_back(i+1);
        a.emplace_back(i+2);
        killerList.emplace_back(a);
        a.clear();
        pp(killerList);
    }
    return 0;
};