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

  Game *game = malloc(sizeof(struct Game));
  setup(game);

  while (!game->quit) {
    processInput(game);
    update(game);
    printCoords(game);
    render(renderer, game, font);
    SDL_Delay(90);
  }

  destroy_window(window, renderer);
  free(game->snake->head);
  free(game->snake);
  free(game->food);

  free(game);

  return 0;
}
