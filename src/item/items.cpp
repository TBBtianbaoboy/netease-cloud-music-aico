//---------------------------------
//File Name    : items.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-18 22:05:10
//Description  : 
//----------------------------------
#include "item/items.h"
#include "const/const.h"
#include "menu/menu.h"
#include "file/file.h"
#include "json/json.h"
#include "utils/utils.h"
#include "const/const.h"
#include "song/songs_controller.h"
#include "controller/resource.h"
#include "controller/color.h"
#include "windows/windows.h"
#include "sub_win_print/sub_win_print.h"
#include <cstdlib>
#include <curses.h>
#include <menu.h>
#include <string>
#include <unistd.h>
#include <memory>
//global control
MAIN_MENU *main_menu = nullptr;
Selected_Playlists_MENU *selected_playlists_menu = nullptr;
Selected_Playlists_Songs_MENU *selected_playlists_songs_menu = nullptr;
Selected_Song_Comment_Menu *selected_song_comment_menu = nullptr;
Artist_Menu *artist_menu = nullptr;
Artist_Sub_Menu *artist_sub_menu = nullptr;
Artist_Album_Menu *artist_album_menu = nullptr;
Search_Menu *search_menu = nullptr;

int song_count = 0;
int playlist_count = 0;
int comment_count = 0;
int artist_count = 0;
int artist_album_count = 0;
short last_sub_index = -1;
bool replaced = false;
extern SubWinPrint sub_win_print;

extern long int musicTime;
extern uint64_t last_song_id;
extern Color color_controller;
extern std::shared_ptr<SongController> songs_controller;

void ItemManager::SetData(short type, short row_id, short layer_id, bool is_next)
{
    Type = type;
    RowId = row_id;
    LayerId = layer_id;
    IsNext = is_next;
}
//判断是否还有下一级菜单
bool ITEM_UTILS::CheckIsL(ItemManager *last_items,short& index,short sub_index)
{
    //歌手菜单分级
    if(index == 4 && last_items->LayerId == 2)
    {
        if(sub_index==0)
            index = 10;
        else if(sub_index==1)
            index = 11;
    }
    //搜索菜单分级
    if(index == 8 && last_items->LayerId == 1)
    {
        if(sub_index == 0)
            index = 12;
        else if(sub_index == 1)
            index = 13;
        else if(sub_index == 2)
            index = 14;
        else if (sub_index == 3)
            index = 15;
    }
    //搜索菜单2分级
    if(index == 13 && last_items->LayerId == 3)
    {
        if(sub_index == 0)
            index = 16;
        else if(sub_index == 1)
            index = 17;
    }

    if (MenuDataPosition[index][last_items->LayerId+1]==-1)
        last_items->IsNext = false;
    else 
        last_items->IsNext = true;
    return last_items->IsNext;
}

//进入菜单后的执行环节
ItemManager* ITEM_UTILS::InToL(ItemManager* last_items,short row_id,short layer_id,short item_number)
{
    ItemManager* items = nullptr;
    switch(MenuDataPosition[row_id][layer_id])
    {
        //0行1层
        case 0:
            {
                string resource_path = url + main_menu[0].route;
                string file_name = "selected_playlists.json";
                ResourceHandler selected_playlist_handler(resource_path,file_name);
                if(!selected_playlist_handler.Run(true))
                    return nullptr;

                playlist_count = selected_playlist_handler.GetData().Json()["result"].size();

                if(selected_playlists_menu)
                    delete [] selected_playlists_menu;
                selected_playlists_menu = new Selected_Playlists_MENU[playlist_count];

                for(int i=0;i<playlist_count;++i)
                {
                    selected_playlists_menu[i].title = Utils::ToString(i)+". "
                        + string(selected_playlist_handler.GetData().Json()["result"][i]["name"]);
                    selected_playlists_menu[i].id = selected_playlist_handler.GetData().Json()["result"][i]["id"];
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(playlist_count+1,sizeof(ITEM*));
                for(int i=0;i<playlist_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_menu[i].title.c_str());
                }

                //0表示无法执行任何动作
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
            //获取歌单歌曲
        case 1:
            {
                string cookie;
                JSON_HANDLER json(".userinfo.json");
                if(json.WriteJsonData())
                    cookie = json.Json()["cookie"];
                string resourcePath = url+uri_playlist_datails+std::to_string(selected_playlists_menu[item_number].id)+"&cookie=" + Utils::EncodeUri(cookie);
                string file_name = "playlist_detail.json";
                ResourceHandler playlist_detail_handler(resourcePath,file_name);
                //获取歌单详情
                if(!playlist_detail_handler.Run(true))
                    return nullptr;
                //判断数据是否有效
                if(playlist_detail_handler.GetData().Json()["code"]!=200)
                    return nullptr;

                //歌单歌曲数量限制与有效性检验
                song_count = playlist_detail_handler.GetData().Json()["playlist"]["trackCount"];
                if (song_count > 50)
                    song_count = 50;
                if(song_count==0)
                    return nullptr;
                //判断播放歌单是否更变
                if(last_sub_index != item_number)
                    replaced = true;
                else 
                    replaced = false;
                last_sub_index = item_number;

                if(selected_playlists_songs_menu)
                    delete [] selected_playlists_songs_menu;
                selected_playlists_songs_menu = new Selected_Playlists_Songs_MENU[song_count];
                resourcePath = url + uri_songs_details;
                //获取歌曲详情路径
                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_songs_menu[i].id = playlist_detail_handler.GetData().Json()["playlist"]["trackIds"][i]["id"];
                    if (i==0)
                        resourcePath += std::to_string(selected_playlists_songs_menu[i].id);
                    else 
                        resourcePath += (","+std::to_string(selected_playlists_songs_menu[i].id));
                }
                //获取歌曲详情
                file_name = "song_detail.json";
                ResourceHandler song_detail_handler(resourcePath,file_name);
                if(!song_detail_handler.Run(true))
                    return nullptr;

                for(int i=0; i<song_count;++i){
                    selected_playlists_songs_menu[i].author = string(song_detail_handler.GetData().Json()["songs"][i]["ar"][0]["name"]);
                    selected_playlists_songs_menu[i].author_id = song_detail_handler.GetData().Json()["songs"][i]["ar"][0]["id"];
                    selected_playlists_songs_menu[i].al = string(song_detail_handler.GetData().Json()["songs"][i]["al"]["name"]);
                    selected_playlists_songs_menu[i].al_id = song_detail_handler.GetData().Json()["songs"][i]["al"]["id"];
                    selected_playlists_songs_menu[i].title = std::to_string(i)+". "
                        + string(song_detail_handler.GetData().Json()["songs"][i]["name"]) + " 🎶 "
                        + selected_playlists_songs_menu[i].author + " 🔖 "
                        + selected_playlists_songs_menu[i].al;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_songs_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(1,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 2:            /*获取评论操作----------no bug*/
            {
                string resourcePath = url + uri_song_comment + Utils::ToString(selected_playlists_songs_menu[item_number].id);
                string file_name = "song_comment_menu.json";
                ResourceHandler resource_handler(resourcePath,file_name);
                if(!resource_handler.Run(true))
                    return nullptr;

                comment_count = resource_handler.GetData().Json()["hotComments"].size();
                if (comment_count == 0)
                    return nullptr;

                if(selected_song_comment_menu)
                    delete [] selected_song_comment_menu;
                selected_song_comment_menu = new Selected_Song_Comment_Menu[comment_count];

                for(int i=0;i<comment_count;++i)
                {
                    selected_song_comment_menu[i].comment = resource_handler.GetData().Json()["hotComments"][i]["content"];
                    selected_song_comment_menu[i].number = resource_handler.GetData().Json()["hotComments"][i]["likedCount"];
                    selected_song_comment_menu[i].user = Utils::ToString(i)+"."
                        +string(resource_handler.GetData().Json()["hotComments"][i]["user"]["nickname"])+": ";
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(comment_count+1,sizeof(ITEM*));
                for(int i=0;i<comment_count;++i){
                    items->Items[i] = new_item(" ",selected_song_comment_menu[i].user.c_str());
                }

                //2表示动作为可显示
                items->SetData(2,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 3:
            {
                //首先判断用户登陆状态
                uint64_t user_id;
                string resourcePath;
                string user_info_file = ".userinfo.json";
                JSON_HANDLER userInfo(user_info_file);
                if(userInfo.WriteJsonData())
                    user_id = userInfo.Json()["account"]["id"];
                else
                {
                    if(!ITEM_UTILS::UserLogin())
                        return nullptr;
                    JSON_HANDLER ui(user_info_file);
                    if(!ui.WriteJsonData())
                        return nullptr;
                    user_id = ui.Json()["account"]["id"];
                }

                resourcePath = url + main_menu[1].route + Utils::ToString(user_id);
                string playlist_file = "playlists.json";
                ResourceHandler playlist_handler(resourcePath,playlist_file);
                if(!playlist_handler.Run(true)){
                    return nullptr;
                }

                playlist_count = playlist_handler.GetData().Json()["playlist"].size();
                if(playlist_count==0)
                    return nullptr;
                //获取用户歌单
                if(selected_playlists_menu)
                    delete [] selected_playlists_menu;
                selected_playlists_menu = new Selected_Playlists_MENU[playlist_count];

                for(int i=0;i<playlist_count;++i)
                {
                    selected_playlists_menu[i].title = Utils::ToString(i)+". "+string(playlist_handler.GetData().Json()["playlist"][i]["name"]);
                    selected_playlists_menu[i].id = playlist_handler.GetData().Json()["playlist"][i]["id"];
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(playlist_count+1,sizeof(ITEM*));
                for(int i=0;i<playlist_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_menu[i].title.c_str());
                }

                //0表示无法执行任何动作
                items->SetData(0,row_id,layer_id,last_items->IsNext);

                //清除
                GlobalWindowsManager::CleanToBot();
                break;
            }
        case 4:
            {
                string user_info_file = ".userinfo.json";
                string cookie;
                JSON_HANDLER userInfo(user_info_file);
                if(userInfo.WriteJsonData())
                    cookie = userInfo.Json()["cookie"];
                else
                {
                    if(!ITEM_UTILS::UserLogin())
                        return nullptr;
                    JSON_HANDLER ui(user_info_file);
                    if(!ui.WriteJsonData())
                        return nullptr;
                    cookie = ui.Json()["cookie"];
                }

                string resourcePath = url + main_menu[2].route
                    + Utils::EncodeUri(cookie);
                string daily_playlist_file = "daily_playlist.json";

                ResourceHandler daily_playlist_handler(resourcePath,daily_playlist_file);
                if(!daily_playlist_handler.Run(true))
                    return nullptr;

                song_count = daily_playlist_handler.GetData().Json()["recommend"].size();
                if(song_count==0)
                    return nullptr;
                //获取用户歌单
                if(selected_playlists_menu)
                    delete [] selected_playlists_menu;
                selected_playlists_menu = new Selected_Playlists_MENU[song_count];

                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_menu[i].title = Utils::ToString(i)+". "+string(daily_playlist_handler.GetData().Json()["recommend"][i]["name"]);
                    selected_playlists_menu[i].id = daily_playlist_handler.GetData().Json()["recommend"][i]["id"];
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_menu[i].title.c_str());
                }

                //0表示无法执行任何动作
                items->SetData(0,row_id,layer_id,last_items->IsNext);

                //清除
                GlobalWindowsManager::CleanToBot();
                break;
            }
        case 5:
            {
                string user_info_file = ".userinfo.json";
                string cookie;
                JSON_HANDLER userInfo(user_info_file);
                if(userInfo.WriteJsonData())
                    cookie = userInfo.Json()["cookie"];
                else
                {
                    if(!ITEM_UTILS::UserLogin())
                        return nullptr;
                    JSON_HANDLER ui(user_info_file);
                    if(!ui.WriteJsonData())
                        return nullptr;
                    cookie = ui.Json()["cookie"];
                }

                string resourcePath = url + main_menu[3].route
                    + Utils::EncodeUri(cookie);
                string daily_songs_file = "daily_songs.json";

                ResourceHandler daily_songs_handler(resourcePath,daily_songs_file);
                if(!daily_songs_handler.Run(true))
                    return nullptr;

                song_count = daily_songs_handler.GetData().Json()["data"]["dailySongs"].size();
                if(song_count>50)
                    song_count = 50;
                if(song_count==0)
                    return nullptr;
                //获取用户歌单
                if(selected_playlists_songs_menu)
                    delete [] selected_playlists_songs_menu;
                selected_playlists_songs_menu = new Selected_Playlists_Songs_MENU[song_count];

                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_songs_menu[i].id = daily_songs_handler.GetData().Json()["data"]["dailySongs"][i]["id"];
                    selected_playlists_songs_menu[i].author = string(daily_songs_handler.GetData().Json()["data"]["dailySongs"][i]["ar"][0]["name"]);
                    selected_playlists_songs_menu[i].author_id = daily_songs_handler.GetData().Json()["data"]["dailySongs"][i]["ar"][0]["id"];
                    selected_playlists_songs_menu[i].al = daily_songs_handler.GetData().Json()["data"]["dailySongs"][i]["al"]["name"];
                    selected_playlists_songs_menu[i].al_id = daily_songs_handler.GetData().Json()["data"]["dailySongs"][i]["al"]["id"];
                    selected_playlists_songs_menu[i].title = Utils::ToString(i) + ". "
                        + string(daily_songs_handler.GetData().Json()["data"]["dailySongs"][i]["name"]) + " 🎶 "
                        + selected_playlists_songs_menu[i].author + " 🔖 "
                        + selected_playlists_songs_menu[i].al;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_songs_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(1,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 6:
            {
                string resourcePath = url + main_menu[4].route;
                string hot_artist_file = "hot_artist.json";
                ResourceHandler hot_artist_handler(resourcePath,hot_artist_file);
                if(!hot_artist_handler.Run(true))
                    return nullptr;
                if(hot_artist_handler.GetData().Json()["code"]!=200)
                    return nullptr;

                artist_count = hot_artist_handler.GetData().Json()["artists"].size();
                if(artist_count==0)
                    return nullptr;

                if(artist_menu)
                    delete [] artist_menu;
                artist_menu = new Artist_Menu[artist_count];

                for(int i=0;i<artist_count;++i)
                {
                    artist_menu[i].id = hot_artist_handler.GetData().Json()["artists"][i]["id"];
                    artist_menu[i].name = hot_artist_handler.GetData().Json()["artists"][i]["name"];
                    if(hot_artist_handler.GetData().Json()["artists"][i]["alias"].size()!=0)
                        artist_menu[i].alias = hot_artist_handler.GetData().Json()["artists"][i]["alias"][0];
                    artist_menu[i].title = Utils::ToString(i) + ". "
                        + artist_menu[i].name + " - " + artist_menu[i].alias;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(artist_count+1,sizeof(ITEM*));
                for(int i=0;i<artist_count;++i){
                    items->Items[i] = new_item(" ",artist_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 7:
            {
                if(artist_sub_menu)
                    delete artist_sub_menu;
                artist_sub_menu = new Artist_Sub_Menu;
                items = new ItemManager;
                items->Items = (ITEM**)calloc(3,sizeof(ITEM*));
                artist_sub_menu->artist_id = artist_menu[item_number].id;
                artist_sub_menu->artist_hot_songs = "0. " + artist_menu[item_number].name + "的热门歌曲";
                artist_sub_menu->artist_all_al = "1. " + artist_menu[item_number].name + "的所有专辑";
                items->Items[0] = new_item(" ",artist_sub_menu->artist_hot_songs.c_str());
                items->Items[1] = new_item(" ",artist_sub_menu->artist_all_al.c_str());
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 40:
            {
                //进入歌手热门歌曲
                string resourcePath = url + uri_artist_hot_song + Utils::ToString(artist_sub_menu->artist_id);
                string artist_hot_song_file = "artist_hot_song.json";
                ResourceHandler artist_hot_song_handler(resourcePath,artist_hot_song_file);
                if(!artist_hot_song_handler.Run(true))
                    return nullptr;
                if(artist_hot_song_handler.GetData().Json()["code"]!=200)
                    return nullptr;
                song_count = artist_hot_song_handler.GetData().Json()["songs"].size();
                if(song_count==0)
                    return nullptr;
                if(selected_playlists_songs_menu)
                    delete [] selected_playlists_songs_menu;
                selected_playlists_songs_menu = new Selected_Playlists_Songs_MENU[song_count];
                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_songs_menu[i].id = artist_hot_song_handler.GetData().Json()["songs"][i]["id"];
                    selected_playlists_songs_menu[i].author = string(artist_hot_song_handler.GetData().Json()["songs"][i]["ar"][0]["name"]);
                    selected_playlists_songs_menu[i].author_id = artist_hot_song_handler.GetData().Json()["songs"][i]["ar"][0]["id"];
                    selected_playlists_songs_menu[i].al = string(artist_hot_song_handler.GetData().Json()["songs"][i]["al"]["name"]);
                    selected_playlists_songs_menu[i].al_id = artist_hot_song_handler.GetData().Json()["songs"][i]["al"]["id"];
                    selected_playlists_songs_menu[i].title = std::to_string(i)+". "
                        + string(artist_hot_song_handler.GetData().Json()["songs"][i]["name"]) + " 🎶 "
                        + selected_playlists_songs_menu[i].author + " 🔖 "
                        + selected_playlists_songs_menu[i].al;
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_songs_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(1,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 9:
            {
                string resourcePath = url + main_menu[5].route;
                string top_list_file = "top_list.json";
                ResourceHandler top_list_handler(resourcePath,top_list_file);
                if(!top_list_handler.Run(true))
                    return nullptr;
                if(top_list_handler.GetData().Json()["code"]!=200)
                    return nullptr;
                playlist_count = top_list_handler.GetData().Json()["list"].size();
                if(playlist_count==0)
                    return nullptr;
                if(playlist_count>50)
                    playlist_count = 50;

                if(selected_playlists_menu)
                    delete [] selected_playlists_menu;
                selected_playlists_menu = new Selected_Playlists_MENU[playlist_count];

                for(int i=0;i<playlist_count;++i)
                {
                    selected_playlists_menu[i].title = Utils::ToString(i)+". "
                        + string(top_list_handler.GetData().Json()["list"][i]["name"]);
                    selected_playlists_menu[i].id = top_list_handler.GetData().Json()["list"][i]["id"];
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(playlist_count+1,sizeof(ITEM*));
                for(int i=0;i<playlist_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_menu[i].title.c_str());
                }

                //0表示无法执行任何动作
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 10:
            {
                string user_info_file = ".userinfo.json";
                string cookie;
                JSON_HANDLER userInfo(user_info_file);
                if(userInfo.WriteJsonData())
                    cookie = userInfo.Json()["cookie"];
                else
                {
                    if(!ITEM_UTILS::UserLogin())
                        return nullptr;
                    JSON_HANDLER ui(user_info_file);
                    if(!ui.WriteJsonData())
                        return nullptr;
                    cookie = ui.Json()["cookie"];
                }

                string resourcePath = url + main_menu[6].route
                    + Utils::EncodeUri(cookie);
                string daily_songs_file = "daily_songs.json";

                ResourceHandler daily_songs_handler(resourcePath,daily_songs_file);
                if(!daily_songs_handler.Run(true))
                    return nullptr;

                song_count = daily_songs_handler.GetData().Json()["data"].size();
                if(song_count>50)
                    song_count = 50;
                if(song_count==0)
                    return nullptr;
                //获取用户歌单
                if(selected_playlists_songs_menu)
                    delete [] selected_playlists_songs_menu;
                selected_playlists_songs_menu = new Selected_Playlists_Songs_MENU[song_count];

                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_songs_menu[i].id = daily_songs_handler.GetData().Json()["data"][i]["id"];
                    selected_playlists_songs_menu[i].author = string(daily_songs_handler.GetData().Json()["data"][i]["artists"][0]["name"]);
                    selected_playlists_songs_menu[i].author_id = daily_songs_handler.GetData().Json()["data"][i]["artists"][0]["id"];
                    selected_playlists_songs_menu[i].al = daily_songs_handler.GetData().Json()["data"][i]["album"]["name"];
                    selected_playlists_songs_menu[i].al_id = daily_songs_handler.GetData().Json()["data"][i]["album"]["id"];
                    selected_playlists_songs_menu[i].title = Utils::ToString(i) + ". "
                        + string(daily_songs_handler.GetData().Json()["data"][i]["name"]) + " 🎶 "
                        + selected_playlists_songs_menu[i].author + " 🔖 "
                        + selected_playlists_songs_menu[i].al;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_songs_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(1,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 41:
            {
                string resourcePath = url + uri_artist_album + Utils::ToString(artist_sub_menu->artist_id);
                string artist_album_file = "artist_album.json";
                ResourceHandler artist_album_handler(resourcePath,artist_album_file);
                if(!artist_album_handler.Run(true))
                    return nullptr;
                if(artist_album_handler.GetData().Json()["code"]!=200)
                    return nullptr;
                artist_album_count = artist_album_handler.GetData().Json()["hotAlbums"].size();
                if(artist_album_count==0)
                    return nullptr;
                if(artist_album_menu)
                    delete [] artist_album_menu;
                artist_album_menu = new Artist_Album_Menu[artist_album_count];

                for(int i=0;i<artist_album_count;++i)
                {
                    artist_album_menu[i].al_artist = artist_album_handler.GetData().Json()["hotAlbums"][i]["artists"][0]["name"];
                    artist_album_menu[i].al_name   = artist_album_handler.GetData().Json()["hotAlbums"][i]["name"];
                    artist_album_menu[i].al_id     = artist_album_handler.GetData().Json()["hotAlbums"][i]["id"];
                    artist_album_menu[i].title     = Utils::ToString(i) + ". "
                        + artist_album_menu[i].al_name + " - " + artist_album_menu[i].al_artist;
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(artist_album_count+1,sizeof(ITEM*));
                for(int i=0;i<artist_album_count;++i){
                    items->Items[i] = new_item(" ",artist_album_menu[i].title.c_str());
                }
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 42:
            {
                string resourcePath = url + uri_album_details + Utils::ToString(artist_album_menu[item_number].al_id);
                string album_details_file = "album_details.json";
                ResourceHandler album_details_handler(resourcePath,album_details_file);
                if(!album_details_handler.Run(true))
                    return nullptr;
                if(album_details_handler.GetData().Json()["code"]!=200)
                    return nullptr;
                song_count = album_details_handler.GetData().Json()["songs"].size();
                if(song_count==0)
                    return nullptr;
                if(song_count>50)
                    song_count = 50;

                if(selected_playlists_songs_menu)
                    delete [] selected_playlists_songs_menu;
                selected_playlists_songs_menu = new Selected_Playlists_Songs_MENU[song_count];
                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_songs_menu[i].id = album_details_handler.GetData().Json()["songs"][i]["id"];
                    selected_playlists_songs_menu[i].author = string(album_details_handler.GetData().Json()["songs"][i]["ar"][0]["name"]);
                    selected_playlists_songs_menu[i].author_id = album_details_handler.GetData().Json()["songs"][i]["ar"][0]["id"];
                    selected_playlists_songs_menu[i].al = album_details_handler.GetData().Json()["songs"][i]["al"]["name"];
                    selected_playlists_songs_menu[i].al_id = album_details_handler.GetData().Json()["songs"][i]["al"]["id"];
                    selected_playlists_songs_menu[i].title = Utils::ToString(i) + ". "
                        + string(album_details_handler.GetData().Json()["songs"][i]["name"]) + " 🎶 "
                        + selected_playlists_songs_menu[i].author + " 🔖 "
                        + selected_playlists_songs_menu[i].al;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_songs_menu[i].title.c_str());
                }
                //1 表示可播放
                items->SetData(1,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 11:
            {
                string resourcePath = url + main_menu[7].route;
                string newest_album_file = "newest_album.json";
                ResourceHandler newest_album_handler(resourcePath,newest_album_file);
                if(!newest_album_handler.Run(true))
                    return nullptr;
                if(newest_album_handler.GetData().Json()["code"]!=200)
                    return nullptr;
                artist_album_count = newest_album_handler.GetData().Json()["albums"].size();
                if(artist_album_count==0)
                    return nullptr;
                if(artist_album_count>50)
                    artist_album_count = 50;

                if(artist_album_menu)
                    delete [] artist_album_menu;
                artist_album_menu = new Artist_Album_Menu[artist_album_count];
                for(int i=0;i<artist_album_count;++i)
                {
                    artist_album_menu[i].al_artist = newest_album_handler.GetData().Json()["albums"][i]["artist"]["name"];
                    artist_album_menu[i].al_name   = newest_album_handler.GetData().Json()["albums"][i]["name"];
                    artist_album_menu[i].al_id     = newest_album_handler.GetData().Json()["albums"][i]["id"];
                    artist_album_menu[i].title     = Utils::ToString(i) + ". "
                        + artist_album_menu[i].al_name + " - " + artist_album_menu[i].al_artist;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(artist_album_count+1,sizeof(ITEM*));
                for(int i=0;i<artist_album_count;++i){
                    items->Items[i] = new_item(" ",artist_album_menu[i].title.c_str());
                }
                //1 表示可播放
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 12:
            {
                if(search_menu)
                    delete search_menu;
                search_menu = new Search_Menu;
                items = new ItemManager;
                items->Items = (ITEM**)calloc(5,sizeof(ITEM*));
                search_menu->song_title = "0. 搜索歌曲";
                search_menu->artist_title = "1. 搜索歌手";
                search_menu->album_title = "2. 搜索专辑";
                search_menu->playlist_title = "3. 搜索歌单";
                items->Items[0] = new_item(" ",search_menu->song_title.c_str());
                items->Items[1] = new_item(" ",search_menu->artist_title.c_str());
                items->Items[2] = new_item(" ",search_menu->album_title.c_str());
                items->Items[3] = new_item(" ",search_menu->playlist_title.c_str());
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                break;
            }
        case 80:
            {
                //搜索前暂停歌曲，以免闪跳
                bool isStop = false;
                if(songs_controller->GetSongState()==1)
                {
                    isStop = true;
                    songs_controller->PauseSong();
                    ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在加载搜索模块...");
                    sleep(1);
                }
                char words[100]{};
                ITEM_UTILS::SearchInput("请输入歌曲名: ", words);
                if(isStop)
                {
                    //搜索结束继续播放
                    songs_controller->ResumeSong();
                }
                ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在搜索中...");

                string resourcePath = url + main_menu[8].route + Utils::ToString(1)
                    + "&keywords=" + Utils::EncodeUri(string(words));
                string search_song_file = "search_song.json";
                ResourceHandler search_song_handler(resourcePath,search_song_file);
                if(!search_song_handler.Run(true))
                    return nullptr;
                if(search_song_handler.GetData().Json()["code"]!=200)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }
                song_count = search_song_handler.GetData().Json()["result"]["songs"].size();
                if(song_count==0)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }

                if(selected_playlists_songs_menu)
                    delete [] selected_playlists_songs_menu;
                selected_playlists_songs_menu = new Selected_Playlists_Songs_MENU[song_count];
                for(int i=0;i<song_count;++i)
                {
                    selected_playlists_songs_menu[i].id = search_song_handler.GetData().Json()["result"]["songs"][i]["id"];
                    selected_playlists_songs_menu[i].author = string(search_song_handler.GetData().Json()["result"]["songs"][i]["ar"][0]["name"]);
                    selected_playlists_songs_menu[i].author_id = search_song_handler.GetData().Json()["result"]["songs"][i]["ar"][0]["id"];
                    selected_playlists_songs_menu[i].al = search_song_handler.GetData().Json()["result"]["songs"][i]["al"]["name"];
                    selected_playlists_songs_menu[i].al_id = search_song_handler.GetData().Json()["result"]["songs"][i]["al"]["id"];
                    selected_playlists_songs_menu[i].title = Utils::ToString(i) + ". "
                        + string(search_song_handler.GetData().Json()["result"]["songs"][i]["name"]) + " 🎶 "
                        + selected_playlists_songs_menu[i].author + " 🔖 "
                        + selected_playlists_songs_menu[i].al;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(song_count+1,sizeof(ITEM*));
                for(int i=0;i<song_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_songs_menu[i].title.c_str());
                }
                //1 表示可播放
                items->SetData(1,row_id,layer_id,last_items->IsNext);
                GlobalWindowsManager::CleanToBot();
                break;
            }
        case 81:
            {
                bool isStop = false;
                if(songs_controller->GetSongState()==1)
                {
                    isStop = true;
                    songs_controller->PauseSong();
                    ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在加载搜索模块...");
                    sleep(1);
                }
                char words[100]{};
                ITEM_UTILS::SearchInput("请输入歌手名: ", words);
                if(isStop)
                {
                    //搜索结束继续播放
                    songs_controller->ResumeSong();
                }
                ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在搜索中...");

                string resourcePath = url + main_menu[8].route + Utils::ToString(100)
                    + "&keywords=" + Utils::EncodeUri(string(words));
                string search_artist_file = "search_artist.json";
                ResourceHandler search_artist_handler(resourcePath,search_artist_file);
                if(!search_artist_handler.Run(true))
                    return nullptr;
                if(search_artist_handler.GetData().Json()["code"]!=200)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }

                artist_count = search_artist_handler.GetData().Json()["result"]["artists"].size();
                if(artist_count==0)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }

                if(artist_menu)
                    delete [] artist_menu;
                artist_menu = new Artist_Menu[artist_count];

                for(int i=0;i<artist_count;++i)
                {
                    artist_menu[i].id = search_artist_handler.GetData().Json()["result"]["artists"][i]["id"];
                    artist_menu[i].name = search_artist_handler.GetData().Json()["result"]["artists"][i]["name"];
                    if(search_artist_handler.GetData().Json()["result"]["artists"][i]["alias"].size()!=0)
                        artist_menu[i].alias = search_artist_handler.GetData().Json()["result"]["artists"][i]["alias"][0];
                    artist_menu[i].title = Utils::ToString(i) + ". "
                        + artist_menu[i].name + " - " + artist_menu[i].alias;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(artist_count+1,sizeof(ITEM*));
                for(int i=0;i<artist_count;++i){
                    items->Items[i] = new_item(" ",artist_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                GlobalWindowsManager::CleanToBot();
                break;
            }
        case 82:
            {
                bool isStop = false;
                if(songs_controller->GetSongState()==1)
                {
                    isStop = true;
                    songs_controller->PauseSong();
                    ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在加载搜索模块...");
                    sleep(1);
                }
                char words[100]{};
                ITEM_UTILS::SearchInput("请输入专辑名: ", words);
                if(isStop)
                {
                    //搜索结束继续播放
                    songs_controller->ResumeSong();
                }
                ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在搜索中...");

                string resourcePath = url + main_menu[8].route + Utils::ToString(10)
                    + "&keywords=" + Utils::EncodeUri(string(words));
                string search_album_file = "search_album.json";
                ResourceHandler search_album_handler(resourcePath,search_album_file);
                if(!search_album_handler.Run(true))
                    return nullptr;
                if(search_album_handler.GetData().Json()["code"]!=200)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }

                artist_album_count = search_album_handler.GetData().Json()["result"]["albums"].size();
                if(artist_album_count==0)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }

                if(artist_album_menu)
                    delete [] artist_album_menu;
                artist_album_menu = new Artist_Album_Menu[artist_album_count];

                for(int i=0;i<artist_album_count;++i)
                {
                    artist_album_menu[i].al_artist = search_album_handler.GetData().Json()["result"]["albums"][i]["artist"]["name"];
                    artist_album_menu[i].al_name   = search_album_handler.GetData().Json()["result"]["albums"][i]["name"];
                    artist_album_menu[i].al_id     = search_album_handler.GetData().Json()["result"]["albums"][i]["id"];
                    artist_album_menu[i].title     = Utils::ToString(i) + ". "
                        + artist_album_menu[i].al_name + " - " + artist_album_menu[i].al_artist;
                }

                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(artist_album_count+1,sizeof(ITEM*));
                for(int i=0;i<artist_album_count;++i){
                    items->Items[i] = new_item(" ",artist_album_menu[i].title.c_str());
                }
                //1 表示动作为可播放
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                GlobalWindowsManager::CleanToBot();
                break;
            }
        case 83:
            {
                bool isStop = false;
                if(songs_controller->GetSongState()==1)
                {
                    isStop = true;
                    songs_controller->PauseSong();
                    ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在加载搜索模块...");
                    sleep(1);
                }
                char words[100]{};
                ITEM_UTILS::SearchInput("请输入专辑名: ", words);
                if(isStop)
                {
                    //搜索结束继续播放
                    songs_controller->ResumeSong();
                }
                ITEM_UTILS::OutPutMessage(GREEN,19,0,false,"正在搜索中...");

                string resourcePath = url + main_menu[8].route + Utils::ToString(1000)
                    + "&keywords=" + Utils::EncodeUri(string(words));
                string search_playlist_file = "search_playlist.json";
                ResourceHandler search_playlist_handler(resourcePath,search_playlist_file);
                if(!search_playlist_handler.Run(true))
                    return nullptr;
                if(search_playlist_handler.GetData().Json()["code"]!=200)
                {
                    ITEM_UTILS::OutPutMessage(RED,19,0,false, "没有搜索结果");
                    return nullptr;
                }
                playlist_count = search_playlist_handler.GetData().Json()["result"]["playlists"].size();

                if(selected_playlists_menu)
                    delete [] selected_playlists_menu;
                selected_playlists_menu = new Selected_Playlists_MENU[playlist_count];

                for(int i=0;i<playlist_count;++i)
                {
                    selected_playlists_menu[i].title = Utils::ToString(i)+". "
                        + string(search_playlist_handler.GetData().Json()["result"]["playlists"][i]["name"]);
                    selected_playlists_menu[i].id = search_playlist_handler.GetData().Json()["result"]["playlists"][i]["id"];
                }
                items = new ItemManager;
                //@beCare +1
                items->Items = (ITEM**)calloc(playlist_count+1,sizeof(ITEM*));
                for(int i=0;i<playlist_count;++i){
                    items->Items[i] = new_item(" ",selected_playlists_menu[i].title.c_str());
                }

                //0表示无法执行任何动作
                items->SetData(0,row_id,layer_id,last_items->IsNext);
                GlobalWindowsManager::CleanToBot();
                break;
            }
        default:
            break;
    }
    return items; 
}

ItemManager* ITEM_UTILS::InitItems()
{
    int main_menu_num = 9;
    main_menu = new MAIN_MENU[main_menu_num];
    main_menu[0].title = "0. 精品歌单";//"Selected Playlists";
    main_menu[0].route = "/personalized";

    main_menu[1].title = "1. 我的歌单";
    main_menu[1].route = "/user/playlist?limit=50&uid=";

    main_menu[2].title = "2. 每日推荐歌单";
    main_menu[2].route = "/recommend/resource?cookie=";

    main_menu[3].title = "3. 每日推荐歌曲";
    main_menu[3].route = "/recommend/songs?cookie=";

    main_menu[4].title = "4. 热门歌手";
    main_menu[4].route = "/top/artists?limit=100";

    main_menu[5].title = "5. 排行榜单";
    main_menu[5].route = "/toplist";

    main_menu[6].title = "6. 私人FM";
    main_menu[6].route = "/personal_fm?cookie=";

    main_menu[7].title = "7. 最新专辑";
    main_menu[7].route = "/album/newest";

    main_menu[8].title = "8. 搜索";
    main_menu[8].route = "/cloudsearch?limit=50&type=";

    ItemManager* items = new ItemManager;
    items->Items = (ITEM**)calloc(main_menu_num+1,sizeof(ITEM*));
    for(int i=0;i<main_menu_num;++i){
        items->Items[i] = new_item(" ",main_menu[i].title.c_str());
    }
    refresh();
    return items;
}

string ITEM_UTILS::GetItemMusicUrl(short item_num)
{
    //检测歌曲是否可用
    string resource_path = url + uri_check_song + Utils::ToString(selected_playlists_songs_menu[item_num].id);
    string file_name = "song_status.json";
    ResourceHandler song_status_handler(resource_path,file_name);
    if(!song_status_handler.Run(true))
        return "x";
    if(!song_status_handler.GetData().Json()["success"])
        return "x";
    //获取歌曲的url
    resource_path = url + uri_song_url + Utils::ToString(selected_playlists_songs_menu[item_num].id);
    file_name = "song_data.json";
    ResourceHandler resource_handler(resource_path,file_name);
    if(!resource_handler.Run(true))
        return "x";
    resource_path = resource_handler.GetData().Json()["data"][0]["url"];
    selected_playlists_songs_menu[item_num].br = resource_handler.GetData().Json()["data"][0]["br"];
    return resource_path;
}

uint64_t ITEM_UTILS::GetItemMusicId(short item_num)
{
    return selected_playlists_songs_menu[item_num].id;
}

void ITEM_UTILS::OutPutMessage(int color,int raw, int col, bool isLine,const string& message)
{
    move(raw,col);
    if (isLine)
        clrtoeol();
    else
        clrtobot();
    refresh();
    color_controller.OpenColor(color);
    mvprintw(raw,col,message.c_str());
    color_controller.CloseColor(color);
    refresh();
}

void ITEM_UTILS::SetItemMusic(short item_num)
{
    // SubWinPrint sub_win_print;
    //初次播放
    if (songs_controller->GetSongState()==0){
        string songUrl = ITEM_UTILS::GetItemMusicUrl(item_num);
        songs_controller->SetSongUrlAndItem(songUrl,item_num);
        songs_controller->StartSong();
        songs_controller->StartMonitor();
        sub_win_print.PrintSongStateWin(SONG_LOAD,BLUE);
        // ITEM_UTILS::OutPutMessage(YELLOW,0,15,true,selected_playlists_songs_menu[item_num].title);
        sub_win_print.PrintInSongPlayWin(selected_playlists_songs_menu[item_num].title);
    //id切歌与停止
    }else if (songs_controller->GetSongState()==1){
        if(last_song_id==ITEM_UTILS::GetItemMusicId(item_num))
        {
            songs_controller->PauseSong();
            sub_win_print.PrintSongStateWin(SONG_STOP,RED);
        }
        else{
            songs_controller->CloseSong();
            string songUrl = ITEM_UTILS::GetItemMusicUrl(item_num);
            songs_controller->SetSongUrlAndItem(songUrl,item_num);
            songs_controller->StartSong();
            sub_win_print.PrintSongStateWin(SONG_LOAD,BLUE);
            // ITEM_UTILS::OutPutMessage(YELLOW,0,15,true,selected_playlists_songs_menu[item_num].title);
            sub_win_print.PrintInSongPlayWin(selected_playlists_songs_menu[item_num].title);
        }
    //id切歌与恢复
    }else if (songs_controller->GetSongState()==2){
        if(last_song_id==ITEM_UTILS::GetItemMusicId(item_num))
        {
            sub_win_print.PrintSongStateWin(SONG_START,GREEN);
            songs_controller->ResumeSong();
        }
        else{
            songs_controller->CloseSong();
            string songUrl = ITEM_UTILS::GetItemMusicUrl(item_num);
            songs_controller->SetSongUrlAndItem(songUrl,item_num);
            songs_controller->StartSong();
            sub_win_print.PrintSongStateWin(SONG_LOAD,BLUE);
            sub_win_print.PrintInSongPlayWin(selected_playlists_songs_menu[item_num].title);
        }
    }
}

void ITEM_UTILS::UserLogout()
{
    GlobalWindowsManager::CleanToBot();
    color_controller.OpenColor(RED);
    mvprintw(19,10,"确定退出当前用户?(y): ");
    refresh();
    color_controller.CloseColor(RED);
    if(getch()=='y')
    {
        GlobalWindowsManager::CleanToBot();
        color_controller.OpenColor(GREEN);
        mvprintw(19,10,"退出成功");
        refresh();
        color_controller.CloseColor(GREEN);
        FILE_HANDLER::RemoveFile(".userinfo.json");
    }
    return;
}

bool ITEM_UTILS::UserLogin()
{
    char phone_number[50];
    char password[50];
    //清空下端
    GlobalWindowsManager::CleanToBot();
    //手机号输入与有效性校验
    echo();
    color_controller.OpenColor(BLUE);
    mvprintw(19,10,"请输入手机号: ");
    refresh();
    color_controller.CloseColor(BLUE);
    getstr(phone_number);
    if(!Utils::CheckPhone(phone_number)){
        GlobalWindowsManager::CleanToBot();
        color_controller.OpenColor(RED);
        mvprintw(19,COLS/2-10,"手机号码格式错误");
        refresh();
        color_controller.CloseColor(RED);
        return false;
    }

    //密码输入与加密
    noecho();
    color_controller.OpenColor(BLUE);
    mvprintw(20,10,"请输入密码: ");
    refresh();
    color_controller.CloseColor(BLUE);
    getstr(password);

    string resourcePath = url + uri_phone_login
        + string(phone_number) + "&md5_password="
        + Utils::MD5(string(password));
    string user_info_file = ".userinfo.json";
    ResourceHandler user_detail_handler(resourcePath,user_info_file);
    if(!user_detail_handler.Run(false))
        return false;

    //加载提示
    GlobalWindowsManager::CleanToBot();
    color_controller.OpenColor(GREEN);
    mvprintw(19,COLS/2-10,"正在登陆中......");
    refresh();
    color_controller.CloseColor(GREEN);

    if(user_detail_handler.GetData().Json()["code"]==200)
    {
        GlobalWindowsManager::CleanToBot();
        return true;
    }
    else{
        GlobalWindowsManager::CleanToBot();
        color_controller.OpenColor(RED);
        mvprintw(19,COLS/2-10,"手机号码未注册或密码错误");
        refresh();
        color_controller.CloseColor(RED);
        FILE_HANDLER::RemoveFile(user_info_file);
        return false;
    }
}

void ITEM_UTILS::SearchInput(const string &anno, char* input)
{
    GlobalWindowsManager::CleanToBot();
    color_controller.OpenColor(CYAN);
    mvprintw(19,0,anno.c_str());
    refresh();
    color_controller.CloseColor(CYAN);
    echo();
    getstr(input);
    noecho();
}

short ITEM_UTILS::GetItenIndex(MENU *menu){
    return current_item(menu)->index;
}

void ITEM_UTILS::Destory(){
    if(main_menu)
        delete [] main_menu;
    if(selected_playlists_songs_menu)
        delete [] selected_playlists_songs_menu;
    if(selected_playlists_menu)
        delete [] selected_playlists_menu;
    if(selected_song_comment_menu)
        delete [] selected_song_comment_menu;
    if(artist_menu)
        delete [] artist_menu;
    if(artist_sub_menu)
        delete artist_sub_menu;
    if(artist_album_menu)
        delete [] artist_album_menu;
    if(search_menu)
        delete search_menu;
}
