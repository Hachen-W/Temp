PATHFLAGS=-Wl,-rpath,/home/simba/oop2024/3/lab/src
OBJECTS=main.o Game.o Aura.o Characteristics.o Construction.o Construction_Enemy.o Entity.o Landscape.o

all: main.o Game.o Entity.o Landscape.o Aura.o Characteristics.o Construction.o Construction_Enemy.o dynamic_enemy
	g++ -std=c++20 $(OBJECTS) -L. -lmyfunc -lsfml-graphics -lsfml-window -lsfml-system $(PATHFLAGS) -o game
	rm -f *.o

dynamic_enemy: Enemy.cpp
	g++ -std=c++20 -fPIC -c Enemy.cpp
	g++ -std=c++20 -shared -o libmyfunc.so Enemy.o

main.o: main.cpp
	g++ -std=c++20 -c main.cpp 

Game.o: Game.cpp
	g++ -std=c++20 -c Game.cpp 

Entity.o: Entity.cpp
	g++ -std=c++20 -c Entity.cpp 

Landscape.o : Landscape.cpp
	g++ -std=c++20 -c Landscape.cpp 

Aura.o: Aura.cpp
	g++ -std=c++20 -c Aura.cpp 

Characteristics.o: Characteristics.cpp
	g++ -std=c++20 -c Characteristics.cpp 

Construction.o: Construction.cpp
	g++ -std=c++20 -c Construction.cpp

Construction_Enemy.o: Construction_Enemy.cpp
	g++ -std=c++20 -c Construction_Enemy.cpp

clean:  
	rm -f *.so

old: *.cpp
	g++ -std=c++20 *.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system 

