#ifndef __NCURSES_INPUT_H__
#define __NCURSES_INPUT_H__

#include "iinput.h"
#include <ncurses.h>
#include <unordered_map>

class NcursesInput : public IInput {
private:
  std::unordered_map<int, KeyEvent> key_map_;

public:
  NcursesInput();
  ~NcursesInput();

public:
  KeyEvent PollInput() override;
  void SetKeyMapping(int key, KeyEvent ev) override;
  KeyEvent GetKeyMapping(int key) const override;
};

#endif // __NCURSES_INPUT_H__