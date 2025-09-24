#ifndef __IINPUT_H__
#define __IINPUT_H__

enum class KeyEvent {
	None,
	Up, Down, Left, Right,
	Confirm, Cancel,
	Quit
};

class IInput {
public:
	virtual ~IInput() {}

public:
	// 获取输入事件
	virtual KeyEvent PollInput() = 0;
	// 设置按键映射
	virtual void SetKeyMapping(int key, KeyEvent ev) = 0;
	// 获取按键映射
	virtual KeyEvent GetKeyMapping(int key) const = 0;
};

#endif //__IINPUT_H__
