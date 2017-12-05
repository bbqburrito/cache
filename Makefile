project:cache.o  main.o
	g++  cache.o  main.o -o project
main.o: main.cpp
	g++ -c main.cpp
L1_cache.o: cache.cpp cache.h
	g++ -c cache.cpp
clean:
	rm *.o project 


