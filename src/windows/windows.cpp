//---------------------------------
//File Name    : windows.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-18 22:06:06
//Description  : 
//----------------------------------
#include "windows/windows.h"
#include "sub_win_print/sub_win_print.h"
#include "song/songs_controller.h"
#include "item/items.h"
#include "controller/color.h"
#include <curses.h>
#include <unistd.h>

extern SubWinPrint sub_win_print;
extern SongController songs_controller;
void GlobalWindowsManager::SetNormalWindows()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    //使光标失效
    curs_set(0);
}

//全局可用,用来刷新窗口
void GlobalWindowsManager::RefreshWindows()
{
    refresh();
}

void GlobalWindowsManager::CleanToBot()
{
    move(19,0);
    clrtobot();
    refresh();
}

void GlobalWindowsManager::CloseWindows()
{
    endwin();
}

WINDOW* GlobalWindowsManager::CreateSubWin(WINDOW* father,int row, int col, int y, int x)
{
    return subwin(father,row, col, y, x);
}

void GlobalWindowsManager::InitSubWin()
{
    WINDOW* comment_win = subwin(stdscr,LINES-19, COLS/2,19,0);
    WINDOW* song_state_win = subwin(stdscr,1,10,0,3);
    WINDOW* song_play_win = subwin(stdscr,1,COLS-15,0,15);

    sub_win_print.SetSubWin(comment_win,nullptr,song_state_win,song_play_win);
}
