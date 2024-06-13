#include "game.h"

// Basic window structure in SDL2 
int initialize_window(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!*window) {
    fprintf(stderr, "Error creating SDL Window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (!*renderer) {
    fprintf(stderr, "Error creating SDL Renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(*window);
    SDL_Quit();
    return 1;
  }

  if (TTF_Init() == -1) {
    fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
    return 1;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
            Mix_GetError());
    return 1;
  }


  return 0;
}

// Clears everything
void destroy_window(SDL_Window *window, SDL_Renderer *renderer,
                    Mix_Chunk *foodSound) {
  Mix_FreeChunk(foodSound);
  Mix_CloseAudio();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
}
