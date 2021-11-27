//---------------------------------
// File Name    : ncurses.cpp
// Author       : aico
// Mail         : 2237616014@qq.com
// Github       : https://github.com/TBBtianbaoboy
// Site         : https://www.lengyangyu520.cn
// Create Time  : 2021-11-14 20:45:03
// Description  :
//----------------------------------
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <memory>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <stack>
#include <locale.h>
#include "song/songs_controller.h"
#include "menu/menu.h"
#include "const/const.h"
#include "menu_windows/main_menu.h"
#include "item/items.h"
#include "windows/windows.h"
#include "controller/color.h"
#include "sub_win_print/sub_win_print.h"

using std::string;
using std::stack;

extern std::shared_ptr<SongController> songs_controller;
extern bool replaced;

extern Selected_Song_Comment_Menu *selected_song_comment_menu;
extern Selected_Playlists_Songs_MENU *selected_playlists_songs_menu;
extern Color color_controller;
SubWinPrint sub_win_print;
#define CTRLD 4

int main() {
    setlocale(LC_ALL,"");
    stack<ItemManager*> store_last_layer;
    int c;
    GlobalWindowsManager sysconfig;
    MenuWinManager mwmanager;

    sysconfig.SetNormalWindows();
    sysconfig.InitSubWin();
    color_controller.StartColor();
    color_controller.LoadAllColor();

    ItemManager* items = ITEM_UTILS::InitItems();
    mwmanager.IniMenu(items);

    mwmanager.InitWin();
    mwmanager.SetMenuWin();
    mwmanager.CondifWin();
    mwmanager.SetWinTitle("网易云音乐- Aico");

    mwmanager.PostMenu();
    mwmanager.RefreshWin();

    songs_controller = std::make_shared<SongController>();
    uint64_t last_song_id = 0;


    while ((c = wgetch(mwmanager.GetWin())) != 'q') {
        wrefresh(mwmanager.GetWin());
        switch (c) {
        case 'c':
            {
                ITEM_UTILS::UserLogout();
                break;
            }
        case 'G':
            {
                mwmanager.MenuDriver(REQ_LAST_ITEM);
                break;
            }
        case 'g':
            {
                char c{};
                if((c=getch())=='g')
                    mwmanager.MenuDriver(REQ_FIRST_ITEM);
                break;
            }
        case 'j':
            {
                mwmanager.MenuDriver(REQ_DOWN_ITEM);
                if(items->Type==2)
                    sub_win_print.PrintInCommentWin(selected_song_comment_menu[ITEM_UTILS::GetItenIndex(mwmanager.GetMenu())].comment);
                break;
            }
        case 'k':
            {
                mwmanager.MenuDriver(REQ_UP_ITEM);
                if(items->Type==2)
                    sub_win_print.PrintInCommentWin(selected_song_comment_menu[ITEM_UTILS::GetItenIndex(mwmanager.GetMenu())].comment);
                break;
            }
        case 'd':
            {
                mwmanager.MenuDriver(REQ_SCR_DPAGE);
                break;
            }
        case 'u':
            {
                mwmanager.MenuDriver(REQ_SCR_UPAGE);
                break;
            }
        case ' ':
            {
                //非歌曲项控制播停
                if (items->Type==0 )
                {
                    if (songs_controller->GetSongState()==2)
                    {
                        sub_win_print.PrintSongStateWin(SONG_START,GREEN);
                        songs_controller->ResumeSong();
                    }
                    else if (songs_controller->GetSongState()==1)
                    {
                        sub_win_print.PrintSongStateWin(SONG_STOP,RED);
                        songs_controller->PauseSong();
                    }
                    break;
                }
                ITEM_UTILS::SetItemMusic(ITEM_UTILS::GetItenIndex(mwmanager.GetMenu()));
                break;
            }
        case 'h':
            {
                GlobalWindowsManager::CleanToBot();
                if(store_last_layer.empty())
                    break;
                mwmanager.UnPostMenu();
                for (int i = 0; i < mwmanager.GetMenu()->arows; ++i)
                    free_item(items->Items[i]);
                delete items;
                mwmanager.FreeMenu();
                items = store_last_layer.top();
                store_last_layer.pop();
                mwmanager.IniMenu(items);
                mwmanager.SetMenuWin();
                mwmanager.PostMenu();
                break;
            }
        case 'l':
            {
                short index;
                short sub_index;
                if (items->RowId == -1)
                    index = current_item(mwmanager.GetMenu())->index;
                else 
                {
                    index = items->RowId;
                    sub_index = current_item(mwmanager.GetMenu())->index;
                }
                if (!ITEM_UTILS::CheckIsL(items,index,sub_index))
                    break;
                store_last_layer.push(items);
                mwmanager.UnPostMenu();
                mwmanager.FreeMenu();
                //进入成功后的菜单
                items = ITEM_UTILS::InToL(items,index,items->LayerId+1,sub_index);
                //检测items 有效性
                if(nullptr==items){
                    items = store_last_layer.top();
                    store_last_layer.pop();
                }
                //更换播放序列
                if(items->Type==1 && replaced)
                    songs_controller->ResetSongItem();
                //首次进入评论就输出
                else if (items->Type==2)
                    sub_win_print.PrintInCommentWin(selected_song_comment_menu[0].comment);
                mwmanager.IniMenu(items);
                mwmanager.SetMenuWin();
                mwmanager.PostMenu();
                break;
            }
        }
        mwmanager.RefreshWin();
    }
    //完美谢幕
    GlobalWindowsManager::CleanToBot();
    ITEM_UTILS::OutPutMessage(GREEN,19,COLS/2-20,false, "正在退出中...");
    songs_controller->PauseSong();
    sleep(1);

    mwmanager.UnPostMenu();
    for (int i = 0; i < mwmanager.GetMenu()->arows+1; ++i)
        free_item(items->Items[i]);
    delete items;
    mwmanager.FreeMenu();
    while(!store_last_layer.empty())
    {
        items = store_last_layer.top();
        store_last_layer.pop();
        mwmanager.IniMenu(items);
        for (int i = 0; i < mwmanager.GetMenu()->arows+1; ++i)
            free_item(items->Items[i]);
        delete items;
        mwmanager.FreeMenu();
    }

    ITEM_UTILS::Destory();
    sysconfig.CloseWindows();

    return 0;
}
