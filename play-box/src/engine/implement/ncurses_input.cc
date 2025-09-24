#include "ncurses_input.h"

NcursesInput::NcursesInput() {
  // ...
}

NcursesInput::~NcursesInput() {
  // ...
}

KeyEvent NcursesInput::PollInput() {
  int key = getch();
  return GetKeyMapping(key);
}

void NcursesInput::SetKeyMapping(int key, KeyEvent ev) {
  key_map_[key] = ev;
}

KeyEvent NcursesInput::GetKeyMapping(int key) const {
  auto it = key_map_.find(key);

  if (it != key_map_.end()) 
    return it->second;
  else 
    return KeyEvent::None;
}