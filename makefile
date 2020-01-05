DCTP: main.cpp MyAI.cpp 
	g++ -std=c++11 -g main.cpp MyAI.cpp -o DCTP
	rm -f *.o
shiro: ShiroAI.cpp Board.cpp
	g++ -std=c++11 -g ShiroAI.cpp Board.cpp -o SHIRO
	rm -f *.o
clean:

	rm -rf DCTP
