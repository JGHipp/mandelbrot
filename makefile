CC=gcc
CFLAGS=-g -lSDL2
FILES=*.c
EXEC=mandelbrot

mandelbrot: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)
