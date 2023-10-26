CXX=g++
CFLAGS=-g -Wall -std=c++17

main: main.cpp condition_parser.o date.o node.o token.o database.o test_runner.o condition_parser_test.cpp
	@$(CXX) $(CFLAGS) main.cpp -o main condition_parser.o date.o node.o token.o database.o test_runner.o condition_parser_test.cpp

condition_parser.o: condition_parser.cpp condition_parser.h
	@$(CXX) $(CFLAGS) -c condition_parser.cpp

date.o: date.cpp date.h
	@$(CXX) $(CFLAGS) -c date.cpp

node.o: node.cpp node.h
	@$(CXX) $(CFLAGS) -c node.cpp

token.o: token.cpp token.h
	@$(CXX) $(CFLAGS) -c token.cpp

database.o: database.cpp database.h
	@$(CXX) $(CFLAGS) -c database.cpp

test_runner.o: test_runner.cpp test_runner.h
	@$(CXX) $(CFLAGS) -c test_runner.cpp
