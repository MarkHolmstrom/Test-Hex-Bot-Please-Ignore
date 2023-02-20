main.out: main.cpp board.cpp bot.cpp node.cpp board.h bot.h constants.h node.h
		g++ -std=c++11 -g -o main.out main.cpp board.cpp bot.cpp node.cpp
