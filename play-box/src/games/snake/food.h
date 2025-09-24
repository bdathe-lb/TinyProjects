#ifndef __FOOD_H__
#define __FOOD_H__

#include "position.h"
#include <random>

class Food {
private:
  std::mt19937 rng_;
  std::uniform_int_distribution<int> dist_x_;
  std::uniform_int_distribution<int> dist_y_;

private:
  Pos pos_;

public:
  Food(int max_x, int max_y);
  
public:
  // 给食物随机生成一个位置
  void GenerateRandomPos();
  // 获取食物的位置
  Pos GetFoodPos() const;
};

#endif  // __FOOD_H__