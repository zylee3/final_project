all:
	-gcc demo_script.c -o demo_script -ltoml
	-gcc test.c -g -o test -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
