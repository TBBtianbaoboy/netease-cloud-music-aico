//---------------------------------
//File Name    : file.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 22:34:27
//Description  : 
//----------------------------------
#ifndef FILE_H
#define FILE_H
#include <fstream>

using std::fstream;
using std::string;

class FILE_HANDLER{
    private:
        fstream file;
        string file_name;
    public:
        FILE_HANDLER(const string& filename);
        ~FILE_HANDLER();
        FILE_HANDLER(const FILE_HANDLER&)=delete;
        FILE_HANDLER& operator=(const FILE_HANDLER&)=delete;
        bool IsOpenFile();
        void WriteNewFile(const char* data);
        void CloseFile();
        string GetFileName();
        void SetFileName(const string& filename);
        static bool RemoveFile(const string& filename);
};

#endif //FILE_H
