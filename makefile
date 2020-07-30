main:main.o lru.o
	g++ main.o lru.o -o main
main.o: main.cpp
	g++ -c main.cpp -o main.o
lru.o: lru.cpp	
	g++ -c lru.cpp -o lru.o


clean:
	del *.o