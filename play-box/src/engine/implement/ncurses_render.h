#ifndef __NCURSES_RENDER_H__
#define __NCURSES_RENDER_H__

#include "irender.h"
#include <ncurses.h>
#include <string>

class NcursesRender : public IRender {
private:
  int width_;
  int heigth_;

public:
  NcursesRender();
  ~NcursesRender();

public:
  void Clear() override;
  void DrawPoint(int x, int y, char c) override;
  void DrawText(int x, int y, const std::string &text) override;
  void Present() override;
  void UpdateSize() override;
  int GetWidth() const override;
  int GetHeight() const override;
};

#endif  // __NCURSES_RENDER_H__