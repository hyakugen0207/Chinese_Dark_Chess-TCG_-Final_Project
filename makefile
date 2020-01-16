DCTP: main.cpp MyAI.cpp 
	g++ -std=c++11 -g main.cpp MyAI.cpp -o DCTP
	rm -f *.o
shiro: ShiroAI.cpp Board.cpp EarlyGame.cpp BothWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp RandomMove.cpp MoveListGenerator.cpp MidGame.cpp BothWithoutKing.cpp WithoutKing.cpp NegaScoutController.cpp CDCNode.cpp ZobristHashTable.cpp NodePool.cpp
	g++ -std=c++11 -O2 ShiroAI.cpp Board.cpp EarlyGame.cpp BothWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp RandomMove.cpp MoveListGenerator.cpp MidGame.cpp BothWithoutKing.cpp WithoutKing.cpp NegaScoutController.cpp CDCNode.cpp ZobristHashTable.cpp NodePool.cpp -o SHIRO
	rm -f *.o
local: main_local.cpp Board.cpp EarlyGame.cpp BothWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp RandomMove.cpp MoveListGenerator.cpp MidGame.cpp BothWithoutKing.cpp WithoutKing.cpp NegaScoutController.cpp CDCNode.cpp ZobristHashTable.cpp NodePool.cpp
	g++ -std=c++11 -O2 main_local.cpp Board.cpp EarlyGame.cpp BothWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp RandomMove.cpp MoveListGenerator.cpp MidGame.cpp BothWithoutKing.cpp WithoutKing.cpp NegaScoutController.cpp CDCNode.cpp ZobristHashTable.cpp NodePool.cpp -o LOCAL
	rm -f *.o
test: Board.cpp EarlyGame.cpp BothWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp RandomMove.cpp MoveListGenerator.cpp MidGame.cpp BothWithoutKing.cpp WithoutKing.cpp NegaScoutController.cpp CDCNode.cpp ZobristHashTable.cpp NodePool.cpp test.cpp
	g++ -std=c++11 -O2 Board.cpp EarlyGame.cpp BothWithKing.cpp EnemyWithoutKing.cpp RuleTable.cpp RandomMove.cpp MoveListGenerator.cpp MidGame.cpp BothWithoutKing.cpp WithoutKing.cpp NegaScoutController.cpp CDCNode.cpp ZobristHashTable.cpp NodePool.cpp test.cpp -o TEST
	rm -f *.o
clean:

	rm -rf DCTP
