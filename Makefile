all:
	gcc -o iv iv.c stb_image.h -I/usr/local/include/SDL3 -L/usr/local/lib -lSDL3 -lm
install:
	cp iv /usr/local/bin/
