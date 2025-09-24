#include "ncurses_render.h"

NcursesRender::NcursesRender() {
  // 初始化 ncurses
  initscr();
  // 关闭行缓冲，按键立即响应
  cbreak();
  // 不现实按键字符
  noecho();
  // 隐藏光标
  curs_set(0);
  // 启用方向键
  keypad(stdscr, TRUE);
  // 非阻塞调用 getch()
  nodelay(stdscr, TRUE);
  // 获取终端大小
  getmaxyx(stdscr, heigth_, width_);
}

NcursesRender::~NcursesRender() {
  // 恢复终端状态
  endwin();
}

void NcursesRender::Clear() {
  clear();
}

void NcursesRender::DrawPoint(int x, int y, char c) {
  if (x < 0 || x >= COLS || y < 0 || y >= LINES) return;
  
  mvaddch(y, x, c);
}

void NcursesRender::DrawText(int x, int y, const std::string &text) {
  if (x < 0 || x >= COLS || y < 0 || y >= LINES) return;

  mvaddstr(y, x, text.c_str());
}

void NcursesRender::Present() {
  refresh();
}

void NcursesRender::UpdateSize() {
  getmaxyx(stdscr, heigth_, width_);
}

int NcursesRender::GetWidth() const {
  return COLS;
}

int NcursesRender::GetHeight() const {
  return LINES;
}