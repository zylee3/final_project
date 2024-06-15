all:
	-gcc demo_script.c ../parsing.o -g -o demo_script -ltoml
	-gcc demo_engine.c ../engine.o -g -o demo_engine -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

clean:
	rm *.o