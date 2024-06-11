all:
	-gcc -c parsing.c -g -o parsing.o
	-gcc demo_script.c parsing.o -g -o demo_script -ltoml
	-gcc -c engine.c -g -o engine.o
	-gcc demo_engine.c engine.o -g -o demo_engine -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	-gcc script_player.c parsing.o engine.o -g -o script_player -ltoml -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
