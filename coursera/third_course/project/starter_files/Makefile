CXX=g++
CFLAGS=-g -Wall -std=c++17

main: main.cpp parse.o search_server.o
	@$(CXX) $(CFLAGS) main.cpp *.o
	rm *.o

parse.o: parse.cpp parse.h
	@$(CXX) $(CFLAGS) -c parse.cpp

search_server.o: search_server.cpp search_server.h
	@$(CXX) $(CFLAGS) -c search_server.cpp

clean:
	rm *.o

zip:
	rm solution.zip
	zip solution.zip search_server.* parse.* iterator_range.h
