#ifndef __POSITION_H__
#define __POSITION_H__

struct Pos {
  int x;
  int y;

  Pos()
    : x(0)
    , y(0)
  { }

  Pos(int pos_x, int pos_y)
    : x(pos_x)
    , y(pos_y)
  { }

  bool operator==(const Pos &other) const { return x == other.x && y == other.y; }
};

#endif  // __POSITION_H__