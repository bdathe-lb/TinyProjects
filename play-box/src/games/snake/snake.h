#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "position.h"
#include <deque>

enum class Direction {
  Up,
  Down,
  Left,
  Right
};

class Snake {
private:
  std::deque<Pos> body_;
  Direction dir_;

public:
  Snake();

public:
  // 蛇初始化
  void Init();
  // 蛇移动
  void Move();
  // 蛇成长
  void Grow();
  // 设置蛇的移动方向
  void SetDirection(Direction dir);
  // 获取蛇的移动方向
  Direction GetDirection() const;
  // 获取蛇头位置
  Pos GetHeadPos() const;
  // 获取蛇身子位置
  std::deque<Pos> GetBodyPos() const;
};

#endif  // __SNAKE_H__