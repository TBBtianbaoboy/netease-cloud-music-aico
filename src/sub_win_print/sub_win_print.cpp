//---------------------------------
//File Name    : sub_win_print.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-23 16:48:59
//Description  : 
//----------------------------------
#include "sub_win_print/sub_win_print.h"
#include <curses.h>
#include "controller/color.h"

extern Color color_controller;

WINDOW* SubWinPrint::GetCommentWin()const{
    return comment_win;
}

WINDOW* SubWinPrint::GetLyricsWin()const{
    return lyrics_win;
}

WINDOW* SubWinPrint::GetSongStateWin()const{
    return song_state_win;
}

WINDOW* SubWinPrint::GetSongPlayWin()const{
    return song_play_win;
}

void SubWinPrint::PrintInCommentWin(const std::string &comment){
    wclear(comment_win);
    color_controller.WOpenColor(comment_win,CYAN);
    wprintw(comment_win,"%s",comment.c_str());
    color_controller.WCloseColor(comment_win,CYAN);
    wrefresh(comment_win);
}

void SubWinPrint::PrintInLyricsWin(const std::string &lyrics){

}

void SubWinPrint::PrintSongStateWin(const std::string &song_state,int type){
    wclear(song_state_win);
    color_controller.WOpenColor(song_state_win,type);
    wprintw(song_state_win,"%s",song_state.c_str());
    color_controller.WCloseColor(song_state_win,type);
    wrefresh(song_state_win);
}

void SubWinPrint::PrintInSongPlayWin(const std::string &song_play){
    wclear(song_play_win);
    color_controller.WOpenColor(song_play_win,YELLOW);
    wprintw(song_play_win,"%s",song_play.c_str());
    color_controller.WCloseColor(song_play_win,YELLOW);
    wrefresh(song_play_win);
}

void SubWinPrint::SetSubWin(WINDOW *comment, WINDOW *lyrics, WINDOW *state, WINDOW *play)
{
    comment_win = comment;
    lyrics_win = lyrics;
    song_state_win = state;
    song_play_win = play;
}
