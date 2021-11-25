//---------------------------------
//File Name    : windows.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-18 21:50:47
//Description  : 
//----------------------------------
#ifndef GLOBAL_WINDOWS_MANAGER_H
#define GLOBAL_WINDOWS_MANAGER_H
// 全局窗口管理器
#include <curses.h>
class GlobalWindowsManager final {
    public:
        void SetNormalWindows();
        static void RefreshWindows();
        static void CleanToBot();
        static WINDOW* CreateSubWin(WINDOW* father,int row,int col,int y,int x);
        void CloseWindows();
        void InitSubWin();
};
#endif //GLOBAL_WINDOWS_MANAGER_H
