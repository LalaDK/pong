build:
	gcc -Wall -I/opt/homebrew/include -lSDL2 -std=c99 ./src/*.c -o game

run:
	./game

clean:
	rm game
