//---------------------------------
//File Name    : songs_controller.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-16 23:10:41
//Description  : 
//----------------------------------
#include "song/songs_controller.h"
#include "utils/utils.h"
#include "item/items.h"
#include "menu/menu.h"
#include "controller/color.h"
#include "const/const.h"
#include "sub_win_print/sub_win_print.h"
#include <mpg123.h>
#include <ao/ao.h>
#include <curl/curl.h>
#include <mutex>
#include <curses.h>

#define BITS 8

mpg123_handle *mh = nullptr;
ao_device *dev = nullptr;
long int musicTime = 0;
int progressControl = 1;

std::shared_ptr<SongController> songs_controller;
uint64_t last_song_id;

extern int song_count;
extern Selected_Playlists_Songs_MENU* selected_playlists_songs_menu;
extern SubWinPrint sub_win_print;

//初始化为停止状态
SongController::SongController()
    :m_pThread(nullptr),
    m_Monitor(nullptr),
    m_Thread_Pause_Flag(false),
    m_Thread_Stop_Flag(false),
    m_Thread_State(SongState::Stoped),
    is_Next_Song_Flag(false),
    close_song_flag(false),
    m_song_item(0){}

SongController::~SongController()
{
    StopSong();
}

void SongController::SetSongUrlAndItem(string &song,short item)
{
    if(song == "x")
    {
        string songUrl = ITEM_UTILS::GetItemMusicUrl(item+1);
        SetSongUrlAndItem(songUrl,item+1);
        return;
    }
    this->song_url = song;
    this->m_song_item = item;
}

void SongController::ResetSongItem()
{
    m_song_item = -1;
}

int SongController::GetSongItem()
{
    return m_song_item;
}


//开始播放
void SongController::StartSong()
{
    last_song_id = ITEM_UTILS::GetItemMusicId(m_song_item);
    if (m_pThread == nullptr)
    {
        m_pThread = std::make_shared<std::thread>(&SongController::Run, this);

        if (m_pThread != nullptr)
        {
            m_Thread_Pause_Flag = false;
            m_Thread_Stop_Flag = false;
            is_Next_Song_Flag = false;
            close_song_flag = false;
            m_Thread_State = SongState::Running;
        }
    }
}

void SongController::StartMonitor()
{
    if (m_pThread != nullptr)
    {
        m_Monitor = std::make_shared<std::thread>(&SongController::RunMonitor, this);

    }
}

//暂停播放
void SongController::PauseSong()
{
    if (m_pThread != nullptr)
    {
        if (m_Thread_State == SongState::Running)
        {
            m_Thread_Pause_Flag = true;
            m_Thread_State = SongState::Paused;
        }
    }
}

//恢复播放
void SongController::ResumeSong()
{
    if (m_pThread != nullptr)
    {
        if (m_Thread_State == SongState::Paused)
        {
            m_Thread_Pause_Flag = false;
            m_Condition_Variable.notify_all();
            m_Thread_State = SongState::Running;
        }
    }
}

//停止播放并退出
void SongController::StopSong()
{
    if (m_pThread != nullptr)
    {
        m_Thread_Pause_Flag = false;

        //解除阻塞
        m_Condition_Variable.notify_all();
        //开启停止退出标志,退出所有线程
        m_Thread_Stop_Flag = true;

        if (m_pThread->joinable())
        {
            m_pThread->join();
        }
        if (m_Monitor->joinable())
        {
            m_Monitor->join();
        }

        //释放
        m_pThread.reset();
        m_Monitor.reset();

        if (m_pThread == nullptr)
        {
            m_Thread_State = SongState::Stoped;
        }
    }
}

void SongController::CloseSong()
{
    if(GetSongState()==2){
        m_Thread_Pause_Flag = false;

        //解除阻塞
        m_Condition_Variable.notify_all();
    }
    close_song_flag = true;
    if (m_pThread != nullptr ){
        if (m_pThread->joinable())
        {
            m_pThread->join();
        }

        //释放
        m_pThread.reset();
    }
}

//获取歌曲播放状态
int SongController::GetSongState() const
{
    return (int)m_Thread_State;
}

bool& SongController::GetPauseFlag()
{
    return m_Thread_Pause_Flag;
}

bool& SongController::GetStopFlag()
{
    return m_Thread_Stop_Flag;
}

bool& SongController::GetNextFlag()
{
    return is_Next_Song_Flag;
}

bool& SongController::GetCloseFlag()
{
    return close_song_flag;
}

std::condition_variable& SongController::GetCV()
{
    return this->m_Condition_Variable;
}

std::mutex& SongController::GetMutex()
{
    return this->m_Mutex;
}

//实际运行
size_t SongController::PlayAudio(void *buffer, size_t size, size_t nb, void *userp)
{
    int err;
    off_t frame_offset;
    unsigned char *audio;
    size_t done;
    ao_sample_format format;
    int channels, encoding;
    long rate;
    int runtime;

    mpg123_feed(mh, (const unsigned char*) buffer, size * nb);
    // while is no use in thread
    do {
        err = mpg123_decode_frame(mh, &frame_offset, &audio, &done);
        //暂停
        if (songs_controller->GetPauseFlag())
        {
            std::unique_lock<std::mutex> thread_locker(songs_controller->GetMutex());
            if (songs_controller->GetPauseFlag())
            {
                // 等待互斥锁
                songs_controller->GetCV().wait(thread_locker);
            }
            thread_locker.unlock();
        }
        //终结退出
        if (songs_controller->GetStopFlag())
            return -1;
        //切换关闭
        if (songs_controller->GetCloseFlag())
            return -1;
        songs_controller->GetPauseFlag() = false;

        switch(err) {
            case MPG123_NEW_FORMAT:
                mpg123_getformat(mh, &rate, &channels, &encoding);
                format.bits = mpg123_encsize(encoding) * BITS;
                format.rate = rate;
                format.channels = channels;
                format.byte_format = AO_FMT_NATIVE;
                format.matrix = 0;
                //计算总时长
                musicTime = mpg123_length(mh)/rate;
                mvprintw(1,33,"%s",Utils::ConvertIntToTime(musicTime).c_str());
                refresh();
                ITEM_UTILS::OutPutMessage(GREEN,0,3,true, SONG_START);
                ITEM_UTILS::OutPutMessage(YELLOW,0,15,true,selected_playlists_songs_menu[songs_controller->GetSongItem()].title);
                //开启设备
                dev = ao_open_live(ao_default_driver_id(), &format, NULL);
                break;
            case MPG123_OK:
                ao_play(dev, (char*)audio, done);
                //计算播放时长
                runtime = (int)((double)frame_offset/38.28687496);
                mvprintw(1,4,"     ");
                mvprintw(1,4,"%s",Utils::ConvertIntToTime(runtime).c_str());
                refresh();
                //计算进度条
                if((int)((double)runtime/((double)musicTime/20))==progressControl){
                    move(1,9+progressControl);
                    addch('=');
                    addch('>');
                    refresh();
                    ++progressControl;
                }
                break;
            case MPG123_NEED_MORE:
                break;
            default:
                break;
        }
    } while(done > 0);
    return size * nb;
}
void SongController::Run()
{
    ao_initialize();
    
    mpg123_init();
    mh = mpg123_new(NULL, NULL);
    mpg123_open_feed(mh);

    //播放前清空计时槽并重置进度条标志
    progressControl = 1;
    move(1,0);
    clrtoeol();
    mvprintw(1,9,"[>");
    mvprintw(1,31,"]");
    refresh();

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, PlayAudio);
    curl_easy_setopt(curl, CURLOPT_URL,song_url.c_str());
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    //监控同步
    if(m_Thread_Stop_Flag || close_song_flag)
        is_Next_Song_Flag = false;
    else
        is_Next_Song_Flag = true;

    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    ao_close(dev);
    ao_shutdown();
}

void SongController::RunMonitor()
{
    while(!m_Thread_Stop_Flag){
        if(is_Next_Song_Flag){
            //默认开启循环播放
            if(m_song_item+1 >= song_count)
                m_song_item = -1;
            //默认开启顺序播放
            //清除旧障
            if (m_pThread->joinable())
            {
                m_pThread->join();
            }
            m_pThread.reset();

            ++m_song_item;
            string songUrl = ITEM_UTILS::GetItemMusicUrl(m_song_item);
            SetSongUrlAndItem(songUrl,m_song_item);
            StartSong();
            sub_win_print.PrintSongStateWin(SONG_LOAD,BLUE);
            sub_win_print.PrintInSongPlayWin(selected_playlists_songs_menu[m_song_item].title);
            is_Next_Song_Flag = false;
        }
    }
}
