#ifndef UNTITLED_MAP_H
#define UNTITLED_MAP_H
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
void generate_random(SDL_Renderer *renderer, double x, double y);
void makecolors ();
void find_center( double x, double y, uint32_t target);
void draw (SDL_Renderer *renderer, double x, double y);
void generate_random(SDL_Renderer *renderer, double x, double y);

#endif UNTITLED_MAP_H
