all:
	-gcc -c parsing.c -o parsing.o
	-gcc -c engine.c -o engine.o
	-gcc script_player.c parsing.o engine.o -o script_player -ltoml -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

debug:
	-gcc -c parsing.c -o parsing.o
	-gcc -c engine.c -o engine.o
	-gcc script_player.c parsing.o engine.o -Ddebug -o script_player -ltoml -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

clean:
	-rm *.o script_player