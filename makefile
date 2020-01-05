DCTP: main.cpp MyAI.cpp 
	g++ -std=c++11 -g main.cpp MyAI.cpp -o DCTP
	rm -f *.o
shiro: ShiroAI.cpp Board.cpp EarlyGame.cpp EnemyWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp
	g++ -std=c++11 -O2 ShiroAI.cpp Board.cpp EarlyGame.cpp EnemyWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp -o SHIRO
	rm -f *.o
clean:

	rm -rf DCTP
