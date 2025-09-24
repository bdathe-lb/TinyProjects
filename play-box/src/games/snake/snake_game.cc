#include "snake_game.h"

SnakeGame::SnakeGame(int max_x, int max_y)
  : snake_(),
    food_(max_x, max_y),
    score_(0),
    game_over_(false),
    width_(max_x),
    height_(max_y),
    elapsed_time_ms_(0),
    update_delay_ms_(50)
{ }

SnakeGame::~SnakeGame() {
  // ...
};

void SnakeGame::Init() {
  snake_.Init();
  food_.GenerateRandomPos();
}

void SnakeGame::HandleInput(KeyEvent ev) {
  Direction cur_dir = snake_.GetDirection();
  
  switch (ev) {
    case KeyEvent::Up:
      if (cur_dir != Direction::Down) snake_.SetDirection(Direction::Up); 
      break;
    case KeyEvent::Down:
      if (cur_dir != Direction::Up) snake_.SetDirection(Direction::Down);
      break;
    case KeyEvent::Left:
      if (cur_dir != Direction::Right) snake_.SetDirection(Direction::Left); 
      break;
    case KeyEvent::Right:
      if (cur_dir != Direction::Left) snake_.SetDirection(Direction::Right);
      break;
    default:
      break;
  }
}

void SnakeGame::Update(int dt_ms) {
  elapsed_time_ms_ += dt_ms;

  while (elapsed_time_ms_ >= update_delay_ms_) {
    snake_.Move();

    game_over_ = IsGameOver();
    if (game_over_) return;
    
    if (IsFoodEaten()) {
      snake_.Grow();
      score_ += 10;
      
      // 重新生成食物位置
      do {
        food_.GenerateRandomPos();
      } while (IsFoodOnSnake()); // 如果蛇占满了整个地图，会卡住，设置最大循环次数来解决
    }

    elapsed_time_ms_ -= update_delay_ms_;
  }
}

void SnakeGame::Render(IRender& render) {
  render.Clear();

  // 渲染蛇头
  Pos head_pos = snake_.GetHeadPos();
  render.DrawPoint(head_pos.x, head_pos.y, '@');

  // 渲染蛇身体
  std::deque<Pos> body_pos = snake_.GetBodyPos();
  for (auto pos : body_pos) {
    render.DrawPoint(pos.x, pos.y, 'O');
  }

  // 渲染食物
  Pos food_pos = food_.GetFoodPos();
  render.DrawPoint(food_pos.x, food_pos.y, '*');

  // 渲染边框 ...

  // 游戏结束提示
  if (game_over_) {
    render.DrawText(width_/2 -5, height_/2, "GAME OVER");
  }
}

bool SnakeGame::IsGameOver() const {
  // 判断是否撞墙
  Pos head_pos = snake_.GetHeadPos();
  if (head_pos.x < 0 || head_pos.x > width_ ||
      head_pos.y < 0 || head_pos.y > height_)
    return true;

  // 判断是否撞倒身子
  std::deque<Pos> body_pos = snake_.GetBodyPos();
  for (auto pos : body_pos) {
    if (pos == head_pos)
      return true;
  }

  return false;
}

void SnakeGame::InitKeyMapping(IInput &input) {
  input.SetKeyMapping('w', KeyEvent::Up);
  input.SetKeyMapping('s', KeyEvent::Down);
  input.SetKeyMapping('a', KeyEvent::Left);
  input.SetKeyMapping('d', KeyEvent::Right);
  input.SetKeyMapping('q', KeyEvent::Quit);
}

bool SnakeGame::IsFoodEaten() {
  Pos head_pos = snake_.GetHeadPos();
  Pos food_pos = food_.GetFoodPos();
  if (head_pos == food_pos) 
    return true;

  return false;
}

bool SnakeGame::IsFoodOnSnake() {
  Pos food_pos = food_.GetFoodPos();
  Pos head_pos = snake_.GetHeadPos();
  std::deque<Pos> body_pos = snake_.GetBodyPos();

  if (food_pos == head_pos) 
    return true;
  
  for (auto pos : body_pos) {
    if (food_pos == pos) 
      return true;
  }

  return false;
}