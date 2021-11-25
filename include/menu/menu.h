//---------------------------------
//File Name    : menu.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-18 21:40:54
//Description  : 
//----------------------------------

#include <cstdint>
#include <string>

using std::string;

//-1-0菜单
//主菜单
struct MAIN_MENU {
    string title;
    string route;
};
//------------------------- 精选歌单    0
//0-1菜单
//精选歌单主菜单
struct Selected_Playlists_MENU {
    string title;
    uint64_t id; //歌单ID
};

//0-2菜单
//精选歌单歌曲菜单
//歌曲通用
struct Selected_Playlists_Songs_MENU{
    string title;
    uint64_t id; //歌曲ID
    uint64_t br; //歌曲码率
    string author;//歌曲作者
    uint64_t author_id;//歌曲作者id
    string al;//歌曲专辑
    uint64_t al_id;//歌曲专辑id
};

//0-3菜单
//歌曲热评
struct Selected_Song_Comment_Menu{
    string user; //评论用户
    string comment; //评论
    uint64_t number; //点赞次数
};

//歌手菜单
//通用
struct Artist_Menu{
    string title;
    string name;
    uint64_t id;
    string alias;
};

//歌手次级菜单
struct Artist_Sub_Menu{
    uint64_t artist_id;
    string artist_hot_songs;
    string artist_all_al;
};

//歌手专辑菜单
struct Artist_Album_Menu{
    uint64_t al_id; //专辑id
    string al_name; //专辑名称
    string al_artist; //专辑作者
    string title;     //菜单标题
};

//搜索菜单
struct Search_Menu{
    string song_title;
    string artist_title;
    string album_title;
    string playlist_title;
};
