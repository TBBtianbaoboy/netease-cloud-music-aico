//---------------------------------
//File Name    : sub_win_print.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-23 16:38:53
//Description  : 
//----------------------------------
#ifndef SUB_WIN_PRINT_H
#define SUB_WIN_PRINT_H

#include <curses.h>
#include <string>

class SubWinPrint final {
    private:
        WINDOW* comment_win; //显示评论的界面
        WINDOW* lyrics_win;  //显示歌词的界面
        WINDOW* song_state_win; //显示当前歌曲状态的界面
        WINDOW* song_play_win;  //显示当前播放歌曲的信息
    public:
        SubWinPrint():comment_win(nullptr),lyrics_win(nullptr),song_state_win(nullptr),song_play_win(nullptr){
        }
        SubWinPrint(const SubWinPrint&)=delete;
        SubWinPrint& operator=(const SubWinPrint&)=delete;
        SubWinPrint(SubWinPrint&&)=delete;
        ~SubWinPrint(){
            delwin(comment_win);
            delwin(song_state_win);
            delwin(song_play_win);
        }

    public:
        void SetSubWin(WINDOW*,WINDOW*,WINDOW*,WINDOW*);
        WINDOW* GetCommentWin()const;
        WINDOW* GetLyricsWin()const;
        WINDOW* GetSongStateWin()const;
        WINDOW* GetSongPlayWin()const;
        void PrintInCommentWin(const std::string&);
        void PrintInLyricsWin(const std::string&);
        void PrintSongStateWin(const std::string&,int);
        void PrintInSongPlayWin(const std::string&);
};
#endif //SUB_WIN_PRINT_H
