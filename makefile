all:
	-gcc -c parsing.c -g -o parsing.o
	-gcc -c engine.c -g -o engine.o
	-gcc script_player.c parsing.o engine.o -g -o script_player -ltoml -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
