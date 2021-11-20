game: Person.o Gamer.o Manager.o main.o
	g++ -std=c++14 Person.o Gamer.o Manager.o main.o -o game

Person.o: Person.cpp
	g++ -std=c++14 -c Person.cpp

Gamer.o: Gamer.cpp
	g++ -std=c++14 -c Gamer.cpp

Manager.o: Manager.cpp
	g++ -std=c++14 -c Manager.cpp

main.o: main.cpp
	g++ -std=c++14 -c main.cpp

clean:
	rm *.o game
