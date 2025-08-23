#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "position.h"
#include <deque>

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Snake {
private:
  std::deque<Pos> body_;
  Direction dir_;

public:
  Snake();
  void Move();
  void Grow();
  void SetDirection(Direction dir);
  Pos GetHeadPos();
  std::deque<Pos> GetBodyPos();
};

#endif  // __SNAKE_H__