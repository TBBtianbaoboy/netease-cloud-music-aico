//---------------------------------
//File Name    : const.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-18 21:41:45
//Description  : 
//----------------------------------
#ifndef CONST_H
#define CONST_H
#include <string>
using std::string;

const short MenuDataPosition[18][8]{
    {-1,0,1,2,-1,-1,-1,-1},       //0--推荐歌单
    {-1,3,1,2,-1,-1,-1,-1},         //1--我的歌单
    {-1,4,1,2,-1,-1,-1,-1},    //2--每日推荐歌单
    {-1,5,2,-1,-1,-1,-1,-1},    //3--每日推荐歌曲
    {-1,6,7,-1,-1,-1,-1,-1},    //4--热门歌手
    {-1,9,1,2,-1,-1,-1,-1},    //5--所有榜单
    {-1,10,2,-1,-1,-1,-1,-1},    //6--私人FM
    {-1,11,42,2,-1,-1,-1,-1},    //7--最新专辑
    {-1,12,-1,-1,-1,-1,-1,-1},    //8--搜索
    {-1,45,46,47,48,49,-1,-1},     //9--电台

    {-1,-1,-1,40,2,-1,-1,-1},      //10--热门歌手次级菜单0
    {-1,-1,-1,41,42,2,-1,-1},     //11--热门歌手次级菜单1

    {-1,-1,80,2,-1,-1,-1,-1},    //12--搜索歌曲0
    {-1,-1,81,7,-1,-1,-1,-1},    //13--搜索歌手1
    {-1,-1,82,42,2,-1,-1,-1},    //14--搜索专辑2
    {-1,-1,83,1,2,-1,-1,-1},     //15--搜索歌单3

    {-1,-1,-1,-1,40,2,-1,-1},      //16--搜索歌手次级菜单0
    {-1,-1,-1,-1,41,42,2,-1},     //17--搜索歌手次级菜单1
};

const string SONG_LOAD="正加载🕑";
const string SONG_START="正播放🎧";
const string SONG_STOP="已暂停🔇";


//全局URL 地址
const string url="http://www.lengyangyu520.cn:3333";
//歌单详情地址
const string uri_playlist_datails="/playlist/detail?id=";
//歌曲详情地址
const string uri_songs_details="/song/detail?ids=";
//歌曲播放地址
const string uri_song_url="/song/url?id=";
//歌曲热门评论地址
const string uri_song_comment="/comment/hot?type=0&limit=50&id=";
//手机号码验证有效性地址
const string uri_phone_check="/cellphone/existence/check?phone=";
//手机号码与密码登陆地址
const string uri_phone_login="/login/cellphone?phone=";//md5_password
//检测登陆状态地址
const string uri_login_status="/login/status";
//检测音乐是否可用
const string uri_check_song="/check/music?id=";
//获取歌手热门歌曲的地址
const string uri_artist_hot_song="/artist/top/song?id=";
//获取歌手专辑地址
const string uri_artist_album="/artist/album?id=";
//获取专辑详情地址
const string uri_album_details="/album?id=";

#endif //CONST_H
