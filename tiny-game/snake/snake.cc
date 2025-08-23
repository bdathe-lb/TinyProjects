#include "snake.h"

Snake::Snake() {
  // 蛇头初始坐标
  body_.push_back(Pos(4,1));  
  // 蛇身初始坐标
  body_.push_back(Pos(3,1));
  body_.push_back(Pos(2,1));
  body_.push_back(Pos(1,1));

  // 方向初始向右
  dir_ = Direction::RIGHT;
}

void Snake::Move() {
  Pos new_head_pos = body_.front();

  // 根据方向修改蛇头新坐标
  switch (dir_) {
    case Direction::UP:    new_head_pos.y -= 1; break;
    case Direction::DOWN:  new_head_pos.y += 1; break;
    case Direction::LEFT:  new_head_pos.x -= 1; break;
    case Direction::RIGHT: new_head_pos.x += 1; break;
    default: break;
  }
  
  // 增加蛇头，删除蛇尾实现蛇坐标更新
  body_.push_front(new_head_pos);
  body_.pop_back();
}

void Snake::Grow() {
    Pos new_head_pos = body_.front();

  // 根据方向修改蛇头新坐标
  switch (dir_) {
    case Direction::UP:    new_head_pos.y += 1; break;
    case Direction::DOWN:  new_head_pos.y -= 1; break;
    case Direction::LEFT:  new_head_pos.x -= 1; break;
    case Direction::RIGHT: new_head_pos.x += 1; break;
    default: break;
  }
  
  // 增加蛇头实现增长
  body_.push_front(new_head_pos);
}

void Snake::SetDirection(Direction dir) {
  dir_ = dir;
}

Pos Snake::GetHeadPos() {
  return body_.front();
}

std::deque<Pos> Snake::GetBodyPos() {
  if (body_.size() <= 1) {
    return std::deque<Pos>();
  }

  return std::deque<Pos>(body_.begin() + 1, body_.end());
}