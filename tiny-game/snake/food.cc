#include "food.h"

Food::Food(int max_x, int max_y) 
  : rng_(std::random_device{}())
  , dist_x_(0, max_x - 1)
  , dist_y_(0, max_y - 1)
{}

void Food::GenerateRandomPos() {
  pos_.x = dist_x_(rng_);
  pos_.y = dist_y_(rng_);
}

Pos Food::GetFoodPos() {
  return pos_;
}