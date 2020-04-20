/*
	Renders the Mandelbrot set
	Written by John Hippisley
*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// SDL stuff
const int WIDTH = 200;
const int HEIGHT = 200;
const int SCALE = 4;
SDL_Window* window;
SDL_Surface* imgSurface;
SDL_Surface* windowSurface;
Uint32* pixels;

int init()
{
   	SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Mandelbrot Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * SCALE, HEIGHT * SCALE, 0);
    windowSurface = SDL_GetWindowSurface(window);
	imgSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    pixels = (Uint32*) imgSurface->pixels;
}

int term()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
	exit(0);
}

int update()
{
	SDL_BlitScaled(imgSurface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);
}

// Boundaries
const double yMin = -1.0;
const double yMax = +1.0;
const double xMin = -2.0;
const double xMax = +2.0;
const int max_iter = 350;

int calc(double ca, double cb);
int init();
int renderpixel(int x, int y);

int main(int argc, char* argv[])
{
	init();
	SDL_Event ev;
	// Current point to calculate
	int x = 0, y = 0, render = 1;
	while(1)
	{
		while(SDL_PollEvent(&ev)) if(ev.type == SDL_QUIT) term();
		if(render)
		{
			renderpixel(x, y);
			++x;
			if(x >= WIDTH)
			{
				x = 0;
				++y;
			}
			if(y >= HEIGHT) render = 0;
		}
	}
	return 0;
}

int renderpixel(int x, int y)
{
	// Calculate the courtesian x and y
	int corx = x - WIDTH/2;
	int cory = HEIGHT-y -HEIGHT/2;

	// Calculate complex number from (x,y)
	double ca = corx * ((xMax-xMin) / WIDTH);
	double cb = cory * ((yMax-yMin) / HEIGHT);

	// Render pixel
	printf("Calculating pixel (%i, %i)...", x, y);
	int gray = calc(ca, cb);
	pixels[x + y * WIDTH] = (gray << 16) + (gray << 8) + gray;
	update();
  	return 0;
}

int calc(double ca, double cb)
{
	// Complex number 'x'
	double xa = 0;
	double xb = 0; // (i)

	int iterations = 0;
	for(;;)
	{
		// Result when squaring x
		double x2a = (xa * xa - xb * xb);
		double x2b = (xa * xb + xb * xa); //i

		// Result when adding squrae to c
		double ra = ca + x2a;
		double rb = cb + x2b;

		// Check if >= 2 and set next input to this output
		if(ra >= 2) break;
		xa = ra;
		xb = rb;
		++iterations;
		if(iterations >= max_iter)
		{
			iterations = 0;
			break;
		}
	}
	printf(" on complex plane is (%f + %f", ca, cb);
	printf("i) --> %i iterations\n", iterations);
	return iterations;
}
