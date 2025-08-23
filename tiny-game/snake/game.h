#ifndef __GAME_H__
#define __GAME_H__

#include "snake.h"
#include "food.h"
#include <ncurses.h>

enum class GameResult{
  RESTAR,
  QUIT
};

class Game {
private:
  Snake snake_;
  Food food_;
  int speed_;
  int score_;
  int length_;
  int width_;
  bool gameover_;
  
public:
  Game(int max_x, int max_y, int game_speed);

  void Init();
  void Run();
  void Update();
  void Render();
  void Delay(int ms);
  GameResult ShowGameOver();
  void HandleInput();
  void GenerateFood();
  bool IsEatFood();
  bool IsFoodOnSnake();
  bool IsCollision();
};

#endif  // __GAME_H__