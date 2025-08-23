#include "game.h"
#include <thread>
#include <chrono>

Game::Game(int max_x, int max_y, int game_speed)
  : snake_()
  , food_(max_x, max_y)
  , speed_(game_speed)
  , score_(0)
  , length_(max_x)
  , width_(max_y)
  , gameover_(false)
{ }

void Game::Init() {
  food_ = Food(COLS, LINES);
  snake_ = Snake();

  GenerateFood();
}

void Game::Run() {
  Init();
  
  while (!gameover_) {
    Render();
    HandleInput();
    Update();
    Delay(speed_);
    clear();
  }
}

void Game::Update() {
  snake_.Move();
  gameover_ = IsCollision();

  if (IsEatFood()) {
    GenerateFood();
    snake_.Grow();
    score_ += 10;
  }
}

void Game::Delay(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

GameResult Game::ShowGameOver() {
  clear();

  int x = COLS / 2 - 8;
  int y = LINES / 2 - 2;

  mvprintw(y, x, "Total Score: %d", score_);
  mvprintw(y + 2, x - 10 , "Press R to restart, Press any key to exit");
  refresh();

  // 修改 grtch 为阻塞式
  nodelay(stdscr, FALSE);
  char key = getch();
  return (key == 'r' || key == 'R') ? GameResult::RESTAR : GameResult::QUIT; 
}

void Game::Render() {
  // 绘制蛇
  Pos snake_head_pos = snake_.GetHeadPos();
  mvprintw(snake_head_pos.y, snake_head_pos.x, "O");

  std::deque<Pos> snake_body_pos = snake_.GetBodyPos();
  for (auto pos : snake_body_pos) {
    mvprintw(pos.y, pos.x, "#");
  }

  // 绘制食物
  Pos food_pos = food_.GetFoodPos();
  mvprintw(food_pos.y, food_pos.x, "*");

  refresh();
}

void Game::HandleInput() {
  int key = getch();
  
  switch (key) {
    case KEY_UP:    snake_.SetDirection(Direction::UP);    break;
    case KEY_DOWN:  snake_.SetDirection(Direction::DOWN);  break;
    case KEY_LEFT:  snake_.SetDirection(Direction::LEFT);  break;
    case KEY_RIGHT: snake_.SetDirection(Direction::RIGHT); break;
    default: break;
  }
}

void Game::GenerateFood() {
  do {
    food_.GenerateRandomPos();
  } while (IsFoodOnSnake());
}

bool Game::IsEatFood() {
  Pos snake_head_pos = snake_.GetHeadPos();
  Pos food_pos = food_.GetFoodPos();
  if (snake_head_pos == food_pos) return true;

  return false;
}

bool Game::IsFoodOnSnake() {   
  Pos food_pos = food_.GetFoodPos();
  Pos snake_head_pos = snake_.GetHeadPos();
  std::deque<Pos> snake_body_pos = snake_.GetBodyPos();

  if (food_pos == snake_head_pos) return true;
  
  for (const auto pos : snake_body_pos) {
    if (food_pos == pos) return true;
  }

  return false;
}

bool Game::IsCollision() {
  Pos snake_head_pos = snake_.GetHeadPos();

  if (snake_head_pos.x >= length_ || snake_head_pos.x <= 0 ||
      snake_head_pos.y >= width_  || snake_head_pos.y <= 0) 
    return true;

  return false;
}