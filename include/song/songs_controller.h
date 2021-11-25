//---------------------------------
//File Name    : thread_songs.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-16 22:51:10
//Description  : 
//----------------------------------
#ifndef SONG_CONTROLLER
#define SONG_CONTROLLER
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

using std::string;

class SongController
{
    private:
        enum class SongState
        {
            Stoped = 0,
            Running = 1,
            Paused = 2
        };
    public:
        SongController();
        ~SongController();

    public:
        void StartSong();
        void StartMonitor();
        void PauseSong();
        void ResumeSong();
        void CloseSong();
        void SetSongUrlAndItem(string&,short);
        void ResetSongItem();
        int GetSongItem();
        bool& GetPauseFlag();
        bool& GetStopFlag();
        bool& GetNextFlag();
        bool& GetCloseFlag();
        std::mutex& GetMutex();
        std::condition_variable& GetCV();
        int GetSongState() const;
        static size_t PlayAudio(void *buffer, size_t size, size_t nb, void *userp);
    private:
        void StopSong();
        void Run();
        void RunMonitor();

private:
    SongState m_Thread_State;
    std::shared_ptr<std::thread> m_pThread;
    std::shared_ptr<std::thread> m_Monitor;
    std::mutex m_Mutex;
    std::condition_variable m_Condition_Variable;
    bool m_Thread_Pause_Flag;
    bool m_Thread_Stop_Flag;
    bool is_Next_Song_Flag;
    bool close_song_flag;
    int m_song_item;
    string song_url;
};
#endif //SONG_CONTROLLER
