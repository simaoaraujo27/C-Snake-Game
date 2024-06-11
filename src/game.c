#include "game.h"

int mod(int x, int y) {
  if (x < 0)
    return x + y;
  return x % y;
}

void processInput(Game *game) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      game->quit = 1;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        if (game->snake->direction != DOWN)
          game->snake->direction = UP;
        break;
      case SDLK_DOWN:
        if (game->snake->direction != UP)
          game->snake->direction = DOWN;
        break;
      case SDLK_LEFT:
        if (game->snake->direction != RIGHT)
          game->snake->direction = LEFT;
        break;
      case SDLK_RIGHT:
        if (game->snake->direction != LEFT)
          game->snake->direction = RIGHT;
        break;
      case SDLK_ESCAPE:
        game->quit = 1;
        break;
      }
    }
  }
}

void printCoords(Game *game) {
  SnakeSegment *head = game->snake->head;
  while (head != NULL) {
    printf("(%d, %d)\n", head->x, head->y);
    head = head->next;
  }
  printf("--------------\n");
}

void increaseSnake(Game *game) {
  SnakeSegment *new = malloc(sizeof(struct SnakeSegment));

  switch (game->snake->direction) {
  case UP:
    new->y = game->snake->tail->y + 1;
    new->x = game->snake->tail->x;
    break;
  case RIGHT:
    new->y = game->snake->tail->y;
    new->x = game->snake->tail->x - 1;
    break;
  case DOWN:
    new->y = game->snake->tail->y - 1;
    new->x = game->snake->tail->x;
    break;
  case LEFT:
    new->y = game->snake->tail->y;
    new->x = game->snake->tail->x - 1;
    break;
  }

  new->next = NULL;
  game->snake->tail->next = new;
  game->snake->tail = new;

  return;
}

void moveSnake(Game *game) {
  int prev_x = game->snake->head->x;
  int prev_y = game->snake->head->y;

  SnakeSegment *current = game->snake->head;
  switch (game->snake->direction) {
  case UP:
    current->y = mod(current->y - 1, GRID_HEIGHT);
    break;
  case RIGHT:
    current->x = mod(current->x + 1, GRID_WIDTH);
    break;
  case DOWN:
    current->y = mod(current->y + 1, GRID_HEIGHT);
    break;
  case LEFT:
    current->x = mod(current->x - 1, GRID_WIDTH);
    break;
  }

  if (current->next != NULL) {
    current = current->next;

    while (current != NULL) {
      int new_x = current->x;
      int new_y = current->y;

      current->x = prev_x;
      current->y = prev_y;

      current = current->next;

      prev_x = new_x;
      prev_y = new_y;
    }
  }

  // Verifica colisão com o próprio corpo
  SnakeSegment *body = game->snake->head->next;
  while (body != NULL) {
    if (game->snake->head->y == body->y && game->snake->head->x == body->x) {
      game->gameOver = 1;
      setup(game);
      return;
    }
    body = body->next;
  }

  // Verifica colisão com a maçã e, em caso afirmativo, gera outra e cresce a
  // cobra
  if (game->snake->head->y == game->food->y &&
      game->snake->head->x == game->food->x) {
    game->score++;
    game->snake->length++;
    increaseSnake(game);
    do {
      game->food->x = rand() % GRID_WIDTH;
      game->food->y = rand() % GRID_HEIGHT;
    } while (game->grid[game->food->y][game->food->x] !=
             EMPTY); // Garante que a comida não aparece sobre a cobra
  }
}

void clearGrid(Game *game) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      game->grid[y][x] = EMPTY;
    }
  }
}

void updateGrid(Game *game) {
  clearGrid(game);
  game->grid[game->food->y][game->food->x] = FOOD;
  SnakeSegment *snake = game->snake->head;
  while (snake != NULL) {
    game->grid[snake->y][snake->x] = SNAKE;
    snake = snake->next;
  }
}

void update(Game *game) {
  moveSnake(game);
  updateGrid(game);
}

void renderScore(SDL_Renderer *renderer, Game *game, TTF_Font *font) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Limpar com cor preta
  SDL_RenderClear(renderer);                      // Limpar a tela

  SDL_Color textColor = {255, 255, 255, 255}; // Cor branca

  char scoreText[50];
  sprintf(scoreText, "Score: %d", game->score);

  SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
  if (!textSurface) {
    fprintf(stderr, "Error creating text surface: %s\n", TTF_GetError());
    return;
  }

  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (!textTexture) {
    fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
    return;
  }

  SDL_Rect textRect;
  textRect.x = 250; // Posição X na tela
  textRect.y = 650; // Posição Y na tela
  textRect.w = textSurface->w;
  textRect.h = textSurface->h;

  SDL_FreeSurface(textSurface); // Não precisamos mais da surface

  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
  SDL_DestroyTexture(textTexture);
}

void renderGrid(SDL_Renderer *renderer, Game *game) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      SDL_Rect rect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
      if (game->grid[y][x] == SNAKE) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      } else if (game->grid[y][x] == FOOD) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }
      SDL_RenderFillRect(renderer, &rect);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawRect(renderer, &rect); // Desenha as bordas das células
    }
  }
  SDL_RenderPresent(renderer);
}

void render(SDL_Renderer *renderer, Game *game, TTF_Font *font) {
  renderGrid(renderer, game);
  renderScore(renderer, game, font);
}

void setup(Game *game) {
  // Inicializa a cobra
  Snake *snake = malloc(sizeof(struct Snake));
  if (snake == NULL) {
    fprintf(stderr, "Failed to allocate memory for Snake struct.\n");
    exit(1);
  }
  SnakeSegment *head = malloc(sizeof(struct SnakeSegment));
  if (head == NULL) {
    fprintf(stderr, "Failed to allocate memory for SnakeSegment struct.\n");
    exit(1);
  }
  head->x = GRID_WIDTH / 2;
  head->y = GRID_HEIGHT / 2;
  head->next = NULL;
  snake->head = head;
  snake->tail = head;
  snake->length = 1;
  snake->direction = RIGHT;
  game->snake = snake;
  increaseSnake(game);
  increaseSnake(game);

  // Inicializa a comida
  Food *food = malloc(sizeof(struct Food));
  if (food == NULL) {
    fprintf(stderr, "Failed to allocate memory for Food struct.\n");
    exit(1);
  }
  food->x = rand() % GRID_WIDTH;
  food->y = rand() % GRID_HEIGHT;
  game->food = food;

  // Inicializa o grid
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      game->grid[i][j] = EMPTY;
    }
  }

  game->grid[head->y][head->x] = SNAKE;
  game->grid[food->y][food->x] = FOOD;

  // Inicializa o score, o gameover e o quit
  game->score = 0;
  game->gameOver = 0;
  game->quit = 0;
}