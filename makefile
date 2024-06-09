all:
	-gcc -c parsing.c -o parsing.o
	-gcc demo_script.c parsing.o -o demo_script -ltoml
	-gcc engine.c -g -o engine -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
