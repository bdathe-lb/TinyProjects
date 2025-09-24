#ifndef	__IGAME_H__
#define	__IGAME_H__

#include "iinput.h"
#include "irender.h"

enum class KeyEvent;

class IGame {
public:
	virtual ~IGame() {  }

	// 初始化游戏
	virtual void Init() = 0;
	// 输入处理
	virtual void HandleInput(KeyEvent key) = 0;
	// 更新逻辑
	virtual void Update(int dt_ms) = 0;
	// 渲染输出
	virtual void Render(IRender& render) = 0;
	// 判断游戏是否结束
	virtual bool IsGameOver() const = 0;
	// 初始化键盘映射
	virtual void InitKeyMapping(IInput &input) = 0;
};

#endif  // __IGAME_H__
