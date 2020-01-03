DCTP: main.cpp MyAI.cpp ZobristHashTable.cpp
	g++ -std=c++11 -g main.cpp MyAI.cpp ZobristHashTable.cpp -o DCTP
	rm -f *.o
clean:

	rm -rf DCTP
