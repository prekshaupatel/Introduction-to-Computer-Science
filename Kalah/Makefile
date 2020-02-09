
kalah: main.o Game.o Player.o Board.o Side.h
	g++ -o kalah main.o Game.o Player.o Board.o

main.o: main.cpp Game.h Player.h Board.h Side.h
	g++ -c main.cpp -o main.o

Game.o: Game.cpp Game.h
	g++ -c Game.cpp -o Game.o

Player.o: Player.cpp Player.h
	g++ -c Player.cpp -o Player.o

Board.o: Board.cpp Board.h
	g++ -c Board.cpp -o Board.o

.PHONY: clean

clean:
	rm *.o
