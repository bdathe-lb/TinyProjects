#ifndef __SNAKE_GAME_H__
#define __SNAKE_GAME_H__

#include "igame.h"
#include "irender.h"
#include "iinput.h"
#include "position.h"
#include "food.h"
#include "snake.h"
#include <deque>

class SnakeGame : public IGame {
private:
  Snake snake_;
  Food food_;
  int score_;
  bool game_over_;
  int width_;
  int height_;
  int elapsed_time_ms_;
  int update_delay_ms_;

public:
  SnakeGame(int max_x, int max_y);
  ~SnakeGame();

public:
  void Init() override;
  void HandleInput(KeyEvent ev) override;
  void Update(int dt_ms) override;
  void Render(IRender& render) override;
  bool IsGameOver() const override;
  void InitKeyMapping(IInput &input) override;

private:
  bool IsFoodEaten();
  bool IsFoodOnSnake();
};

#endif  // __SNAKE_GAME_H__