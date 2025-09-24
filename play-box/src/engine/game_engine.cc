#include "game_engine.h"
#include <chrono>
#include <thread>

GameEngine::GameEngine(int frame_delay_ms)
	: running_(false),
		frame_delay_ms_(frame_delay_ms)
{ } 

void GameEngine::Init(std::unique_ptr<IGame> game,
					std::unique_ptr<IInput> input,
					std::unique_ptr<IRender> render) {
	game_ = std::move(game);
	input_ = std::move(input);
	render_ = std::move(render);

	game_->Init();
	game_->InitKeyMapping(*input_);
}

void GameEngine::Run() {
	using clock = std::chrono::steady_clock;
	auto last_time = clock::now();

	running_ = true;

	while (running_) {
		// 计算帧时间差 dt
		auto now = clock::now();
		auto dt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count();
		last_time = now;

		// 处理输入
		KeyEvent ev = input_->PollInput();
		if (ev == KeyEvent::Quit) {
			running_ = false;
			break;
		}
		game_->HandleInput(ev);

		// 更新游戏逻辑
		game_->Update(dt_ms);

		// 渲染
		game_->Render(*render_);

		// 判断游戏是否结束
		if (game_->IsGameOver()) {
			running_ = false;
		}

		// 控制帧率
		std::this_thread::sleep_for(std::chrono::milliseconds(frame_delay_ms_));
	}
}

void GameEngine::Shutdown() {
	game_.reset();
	input_.reset();
	render_.reset();

	running_ = false;
}


