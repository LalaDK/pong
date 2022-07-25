build:
	gcc -Wall -std=c99 ./src/*.c -o game -lSDL2

run:
	./game

clean:
	rm game
