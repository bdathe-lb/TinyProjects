#include <getopt.h>
#include <string.h>
#include "game.h"

int main(int argc, char *argv[]) {
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

  struct option long_options[] = {
    { "level",   required_argument, 0, 'l' },
    { "help",    no_argument,       0, 'h' },
    { "version", no_argument,       0, 'v' },
    {  0,        0,                 0,  0  }
  };
  const char *optstring = "l:hv";
  
  int opt;
  int opt_idx = 0;
  int speed = 500;
  while ((opt = getopt_long(argc, argv, optstring, long_options, &opt_idx)) != -1) {
    switch (opt) {
      case 'l': 
        if (strcmp(optarg, "simple") == 0) {
          speed = 1000;
        } else if (strcmp(optarg, "medium") == 0) {
          speed = 500;
        } else if (strcmp(optarg, "hard") == 0) {
          speed = 50;
        } else {
          fprintf(stderr, "The argument is error, please use 'simple', 'medium' or 'hard'\n");
          return 1;
        }
      break;
      case 'h':
        printf("Usage: ./snake [-l/--level] <simple/medium/hard>\n");
        break;
      case 'v':
        printf("The Snake written by Bdathe\n");
        break;
      default:
        printf("Usage: ./snake [-l/--level] <simple/medium/hard>\n");
        break;
    }
  } 

  while (true) {
    Game game(COLS, LINES, speed);
    game.Run();

    GameResult res = game.ShowGameOver();
    if (res == GameResult::QUIT) break;

    // 非阻塞调用 getch()
    nodelay(stdscr, TRUE);
  }

  // 退出 ncurses
  endwin();

  return 0;
}