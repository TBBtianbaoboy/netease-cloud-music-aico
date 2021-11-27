//---------------------------------
//File Name    : items.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-18 20:47:59
//Description  : 
//----------------------------------
#ifndef ITEM_H
#define ITEM_H
#include <menu.h>
#include <string>

using std::string;

//argsAnnotation :rowid default -1 , layerId default 0
//en_US:items manager in all
//zh_CN:所有的项目管理器
struct ItemManager{
    ITEM** Items;
    bool IsNext;
    short RowId;
    short LayerId;
    short Type; // 0 others | 1 songs | 2 comment
    ItemManager():Items(nullptr),IsNext(false),RowId(-1),LayerId(0),Type(0){}
    void SetData(short type,short row_id,short layer_id,bool is_next);
};

class ITEM_UTILS final {
    public:
        static bool         CheckIsL(ItemManager* last_items,short& index,short sub_index);
        static ItemManager* InToL(ItemManager* last_items,short row_id,short layer_id,short sub_index);
        static ItemManager* InitItems();
        //called by SetItemMusic
        static string       GetItemMusicUrl(short item_num);
        //called by SetItemMusic
        static uint64_t     GetItemMusicId(short item_num);
        static void         SetItemMusic(short item_num);
        static void         OutPutMessage(int color,int raw,int col,bool isLine,const string &message);
        static void         UserLogout();
        static bool         UserLogin();
        static void         SearchInput(const string&,char* input);
        static short        GetItenIndex(MENU*);
        static void         Destory();
};
#endif //ITEM_H
