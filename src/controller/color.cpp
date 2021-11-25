#include "controller/color.h"
#include <curses.h>
Color color_controller;

//开启全局颜色支持
void Color::StartColor()
{
    start_color();
}

void Color::LoadAllColor()
{
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
}

void Color::OpenColor(int type)
{
    attron(COLOR_PAIR(type));
}

void Color::CloseColor(int type)
{
    attroff(COLOR_PAIR(type));
}

void Color::WOpenColor(WINDOW *win, int type)
{
    wattron(win,COLOR_PAIR(type));
}

void Color::WCloseColor(WINDOW *win, int type)
{
    wattroff(win,COLOR_PAIR(type));
}
