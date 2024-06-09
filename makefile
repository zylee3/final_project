all:
	-gcc demo_script.c -o demo_script -ltoml
	-gcc engine.c -g -o engine -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
