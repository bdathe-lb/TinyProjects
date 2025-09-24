#ifndef __GAME_ENGINE_H__
#define	__GAME_ENGINE_H__

#include "igame.h"
#include "iinput.h"
#include "irender.h"
#include <memory>

class GameEngine {
private:
	std::unique_ptr<IGame> game_;
	std::unique_ptr<IInput> input_;
	std::unique_ptr<IRender> render_;
	bool running_;
	int frame_delay_ms_; 

public:
	GameEngine(int frame_delay_ms = 16);

public:
	// 游戏初始化
	void Init(std::unique_ptr<IGame> game,
						std::unique_ptr<IInput> input,
						std::unique_ptr<IRender> render);
	// 游戏运行主循环
	void Run();
	// 游戏结束时的资源清理
	void Shutdown();
};

#endif //__GAME_ENGINE_H__ 
