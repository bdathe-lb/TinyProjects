#ifndef __RENDER_H__
#define __RENDER_H__

#include <string>

class IRender {
public:
	virtual ~IRender() {}

public:
	// 清空画布
	virtual void Clear() = 0;
	// 在坐标 (x,y) 绘制一个字符
	virtual void DrawPoint(int x, int y, char c) = 0;
	// 在坐标 (x,y) 绘制一段文本
	virtual void DrawText(int x, int y, const std::string &text) = 0;
	// 提交绘制到画布
	virtual void Present() = 0;
	// 更新画布大小
	virtual void UpdateSize() = 0;
	// 获取画布的宽度
	virtual int GetWidth() const = 0;
	// 获取画布的高度
	virtual int GetHeight() const = 0;
};

#endif //__RENDER_H__
