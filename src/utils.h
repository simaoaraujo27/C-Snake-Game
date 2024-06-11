#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>

int initialize_window(SDL_Window **window, SDL_Renderer **renderer);
void destroy_window(SDL_Window *window, SDL_Renderer *renderer);

#endif 