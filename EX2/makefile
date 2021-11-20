egame: Person.o Gamer.o Manager.o encryption.o caesar.o main.o
	g++ -std=c++14 Person.o Gamer.o Manager.o encryption.o caesar.o main.o -o egame

Person.o: Person.cpp
	g++ -std=c++14 -c Person.cpp

Gamer.o: Gamer.cpp
	g++ -std=c++14 -c Gamer.cpp

Manager.o: Manager.cpp
	g++ -std=c++14 -c Manager.cpp

encryption.o: encryption.cpp
	g++ -std=c++14 -c encryption.cpp

caesar.o: caesar.cpp
	g++ -std=c++14 -c caesar.cpp

main.o: main.cpp
	g++ -std=c++14 -c main.cpp

clean:
	rm *.o egame