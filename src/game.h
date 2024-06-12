#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define BLOCK_SIZE 40
#define GRID_WIDTH (800 / BLOCK_SIZE)
#define GRID_HEIGHT (600 / BLOCK_SIZE)

#define EMPTY 0
#define SNAKE 1
#define FOOD 2

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

typedef struct {
  int x;
  int y;
} Point;

typedef struct SnakeSegment {
  int x;
  int y;
  struct SnakeSegment *next;
} SnakeSegment;

typedef struct Snake {
  SnakeSegment *head;
  SnakeSegment *tail;
  int length;
  int direction;
} Snake;

typedef struct Food {
  int x;
  int y;
} Food;

typedef struct Game {
  int grid[GRID_HEIGHT][GRID_WIDTH];
  Snake *snake;
  Food *food;
  int score;
  int gameOver;
  int quit;
} Game;

int mod(int x, int y);
void processInput(Game *game);

void increaseSnake(Game *game);
void moveSnake(Game *game, Mix_Chunk *foodSound);
void clearGrid(Game *game);
void updateGrid(Game *game);
void update(Game *game, Mix_Chunk *foodSound);
void renderGrid(SDL_Renderer *renderer, Game *game);
void renderScore(SDL_Renderer *renderer, Game *game, TTF_Font *font);
void render(SDL_Renderer *renderer, Game *game, TTF_Font *font);
void setup(Game *game);

#endif
