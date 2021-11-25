//---------------------------------
//File Name    : http.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 22:00:33
//Description  : 
//----------------------------------
#include "http/http.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <mpg123.h>

extern mpg123_handle *mh;
extern ao_device *dev;
extern long int musicTime;

CURL_HANDLER::CURL_HANDLER()
{
    curl_data.size = 0;
    curl_data.data = nullptr;
}

CURL_HANDLER::~CURL_HANDLER()
{
    if(curl_data.data){
        free(curl_data.data);
    }
}

size_t CURL_HANDLER::WriteData(void *ptr, size_t size, size_t nb, CURL_DATA *data)
{
    size_t index = data->size;
    size_t n = (size * nb);
    char* tmp;

    data->size += (size * nb);

    tmp = (char*)realloc(data->data, data->size + 1);

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        // @log
        // fprintf(stderr, "fail to allocate memory.\n");
        return 0;
    }
    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';
    return size * nb;
}

size_t CURL_HANDLER::PlayAudio(void *buffer, size_t size, size_t nb, void *userp)
{
    int err;
    off_t frame_offset;
    unsigned char *audio;
    size_t done;
    ao_sample_format format;
    int channels, encoding;
    long rate;

    mpg123_feed(mh, (const unsigned char*) buffer, size * nb);
    do {
        err = mpg123_decode_frame(mh, &frame_offset, &audio, &done);

        // printf("offset=%ld,done=%ld\n",frame_offset,done);
        switch(err) {
            case MPG123_NEW_FORMAT:
                mpg123_getformat(mh, &rate, &channels, &encoding);
                format.bits = mpg123_encsize(encoding) * BITS;
                format.rate = rate;
                format.channels = channels;
                format.byte_format = AO_FMT_NATIVE;
                format.matrix = 0;
                musicTime = mpg123_length(mh)/rate;
                //TODO
                dev = ao_open_live(ao_default_driver_id(), &format, NULL);
                break;
            case MPG123_OK:
                ao_play(dev, (char*)audio, done);
                break;
            case MPG123_NEED_MORE:
                break;
            default:
                break;
        }
    } while(done > 0);

    return size * nb;

}

void CURL_HANDLER::HandleMusicUrl(const char *music_url)
{
    ao_initialize();
    
    mpg123_init();
    mh = mpg123_new(NULL, NULL);
    mpg123_open_feed(mh);

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, PlayAudio);
    curl_easy_setopt(curl, CURLOPT_URL, music_url);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    ao_close(dev);
    ao_shutdown();
}

bool CURL_HANDLER::HandleUrl(const char *url)
{
    CURL *curl;

    curl_data.size = 0;
    curl_data.data = (char*)malloc(4096*4*4); /* reasonable size initial buffer */
    if(nullptr == curl_data.data)
    {
        //@log
        return false;
    }

    curl_data.data[0] = '\0';

    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_data);
        res = curl_easy_perform(curl);
               if(res != CURLE_OK) {
                   curl_easy_cleanup(curl);
                   //@log
                   // fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
                   return false;
               }
        curl_easy_cleanup(curl);
    }
    return true;
}

char* CURL_HANDLER::GetData()const{
    return curl_data.data;
}
