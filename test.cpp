#include <vector>
#include <iostream>

class AAA{
    public:
        int a; 

    AAA(int b){
        a = b;
    }
};

int main(){
    std::vector<int>* killerList = new std::vector<int>();
    char a = 2;

    a++;

    std::cout << int(a) << std::endl;
    return 0;
};