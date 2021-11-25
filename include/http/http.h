//---------------------------------
//File Name    : http.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 21:51:06
//Description  : 
//----------------------------------
#ifndef HTTP_H
#define HTTP_H
#include <iostream>
#include <mpg123.h>
#include <ao/ao.h>

#define BITS 8


struct CURL_DATA {
    size_t size;
    char* data;
};

class CURL_HANDLER final {
    private:
        CURL_DATA curl_data;
    public:
        CURL_HANDLER();
        ~CURL_HANDLER();
        CURL_HANDLER(const CURL_HANDLER&)=delete;
        CURL_HANDLER& operator=(const CURL_HANDLER&)=delete;
        static size_t WriteData(void *ptr, size_t size, size_t nb, CURL_DATA *data); 
        bool HandleUrl(const char* url);

        static size_t PlayAudio(void *buffer, size_t size, size_t nb, void *userp);
        void HandleMusicUrl(const char* music_url);
        char* GetData()const;
};
#endif //HTTP_H
