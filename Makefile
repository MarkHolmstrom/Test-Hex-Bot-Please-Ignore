main.out: main.cpp board.cpp bot.cpp node.cpp unionfind.cpp board.h bot.h constants.h node.h unionfind.h Makefile
		g++ -std=c++11 -g -O3 -o main.out main.cpp board.cpp bot.cpp node.cpp unionfind.cpp
