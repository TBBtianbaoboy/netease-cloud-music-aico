//---------------------------------
//File Name    : main_menu.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 13:45:05
//Description  : 
//----------------------------------
#ifndef MENU_WINDOWS_H
#define MENU_WINDOWS_H
#include "menu_windows/main_menu.h"
#include <curses.h>
#include <cstring>

void MenuWinManager::InitWin()
{
    //15 行，最大列
    this->window = newwin(14, COLS/2, 5, 0);
    keypad(this->window, TRUE);

}

void MenuWinManager::IniMenu(ItemManager* items)
{
    current_menu = new_menu((ITEM**)items->Items);
}

void MenuWinManager::SetMenuWin()
{
    set_menu_win(current_menu,window);
    set_menu_sub(current_menu, derwin(window, 10, COLS/2-2, 3, 1));
    set_menu_format(current_menu, 10, 1);
    set_menu_mark(current_menu, " -> ");
}

void MenuWinManager::CondifWin()
{
    //add box to window
    // box(window, 0, 0);
    // mvwaddch(window, 2, 0, ACS_LTEE);
    // mvwhline(window, 2, 1, ACS_HLINE, COLS/2-2);
    // mvwaddch(window, 2, COLS/2-1, ACS_RTEE);
}

void MenuWinManager::PrintInWin(WINDOW *win, int starty, int startx, int width,
                     const char *string, chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}


void MenuWinManager::SetWinTitle(string title)
{
    PrintInWin(window, 1, 0, COLS/2, title.c_str(), COLOR_PAIR(1));
}

void MenuWinManager::PostMenu()
{
    post_menu(current_menu);
}

void MenuWinManager::UnPostMenu()
{
    unpost_menu(current_menu);
}

void MenuWinManager::FreeMenu()
{
    free_menu(current_menu);
}

void MenuWinManager::RefreshWin()
{
    wrefresh(window);
}

MENU* MenuWinManager::GetMenu()const
{
    return current_menu;
}

WINDOW* MenuWinManager::GetWin()const
{
    return window;
}

void MenuWinManager::MenuDriver(int action)
{
    menu_driver(current_menu,action);
}

#endif //MENU_WINDOWS_H
