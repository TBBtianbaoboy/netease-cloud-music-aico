//---------------------------------
// File Name    : menu.h
// Author       : aico
// Mail         : 2237616014@qq.com
// Github       : https://github.com/TBBtianbaoboy
// Site         : https://www.lengyangyu520.cn
// Create Time  : 2021-11-15 10:12:28
// Description  :
//----------------------------------
#ifndef MENU_WIN_MANAGER_H
#define MENU_WIN_MANAGER_H
#include <curses.h>
#include <string>
#include "../item/items.h"

using std::string;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4


// 菜单界面管理器
class MenuWinManager final{
    private:
        MENU* current_menu;
        WINDOW* window;
        void PrintInWin(WINDOW *win, int starty, int startx, int width,
                     const char *string, chtype color);
    public:
        MenuWinManager():current_menu(nullptr),window(nullptr){}
        void InitWin();
        void IniMenu(ItemManager* items);
        void SetMenuWin();
        void CondifWin();
        void SetWinTitle(string title);

        void PostMenu();
        void UnPostMenu();
        void FreeMenu();

        void RefreshWin();

        MENU* GetMenu()const;
        WINDOW* GetWin()const;

        void MenuDriver(int action);
};
#endif //MENU_WIN_MANAGER_H
