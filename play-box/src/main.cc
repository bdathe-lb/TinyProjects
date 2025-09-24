#include "game_engine.h"
#include "snake_game.h"
#include "ncurses_input.h"
#include "ncurses_render.h"
#include <memory>

int main() {
  // 创建游戏引擎对象
  GameEngine engine;

  // 创建游戏、输入、渲染对象
  auto input = std::make_unique<NcursesInput>();
  auto render = std::make_unique<NcursesRender>();
  auto game = std::make_unique<SnakeGame>(render->GetWidth(), render->GetHeight());

  // 初始化引擎
  engine.Init(std::move(game), std::move(input), std::move(render));

  // 运行游戏主循环
  engine.Run();

  // 游戏结束，清理资源
  engine.Shutdown();

  return 0;
}
