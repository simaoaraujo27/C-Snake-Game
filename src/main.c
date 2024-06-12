#include "game.h"
#include "utils.h"
#include <time.h>

int main() {
  srand(time(NULL));
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (initialize_window(&window, &renderer) != 0) {
    return 1;
  }

  TTF_Font *font = TTF_OpenFont("assets/fonts/font.ttf", 80);
  if (!font) {
    fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
    return 1;
  }

  Mix_Chunk *foodSound = Mix_LoadWAV("assets/audio/eating.wav");
  if (foodSound == NULL) {
    fprintf(stderr, "Failed to load food sound effect! SDL_mixer Error: %s\n",
            Mix_GetError());
    return 1;
  }

  // Carregar música de fundo
  Mix_Music *backgroundMusic = Mix_LoadMUS("assets/audio/background.mp3");
  if (backgroundMusic == NULL) {
    fprintf(stderr, "Failed to load background music! SDL_mixer Error: %s\n",
            Mix_GetError());
    return 1;
  }

  Game *game = malloc(sizeof(struct Game));
  setup(game);

  // Reproduzir música de fundo em loop
  if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
    fprintf(stderr, "SDL_mixer Error: %s\n", Mix_GetError());
  }

  Mix_VolumeMusic(30);

  while (!game->quit) {
    processInput(game);
    update(game, foodSound);
    render(renderer, game, font);
    SDL_Delay(125);
  }

  destroy_window(window, renderer, foodSound);
  free(game->snake->head);
  free(game->snake);
  free(game->food);
  Mix_FreeMusic(backgroundMusic);

  free(game);

  return 0;
}
