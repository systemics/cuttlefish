#include <ncursesw/ncurses.h>

int main() { 
  initscr();
  curs_set(0);
  /*
    mvprintw(y, x, string);
    getmaxyx(stdscr,row,col);
  */
  refresh();
  getch();
  endwin();
  return 0;
}
