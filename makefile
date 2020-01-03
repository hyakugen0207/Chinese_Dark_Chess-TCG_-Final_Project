DCTP: main.cpp MyAI.cpp 
	g++ -std=c++11 -g main.cpp MyAI.cpp -o DCTP
	rm -f *.o
shiro: ShiroAI.cpp
	g++ -std=c++11 -g ShiroAI.cpp -o SHIRO
	rm -f *.o
clean:

	rm -rf DCTP
