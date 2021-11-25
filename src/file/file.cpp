//---------------------------------
//File Name    : file.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 22:43:18
//Description  : 
//----------------------------------
#include "file/file.h"
#include <iostream>

using std::endl;

FILE_HANDLER::FILE_HANDLER(const string& filename )
{
    file_name = filename;
}

FILE_HANDLER::~FILE_HANDLER(){}

bool FILE_HANDLER::IsOpenFile()
{
    file.open(file_name,std::ios_base::out);
    if (file.is_open())
        return true;
    return false;
}

void FILE_HANDLER::WriteNewFile(const char* data) {
    file << data << endl;
}

void FILE_HANDLER::CloseFile()
{
    file.close();
}

string FILE_HANDLER::GetFileName()
{
    return file_name;
}

void FILE_HANDLER::SetFileName(const string &filename)
{
    file_name = filename;
}

bool FILE_HANDLER::RemoveFile(const string& filename)
{
    if(remove(filename.c_str()))
        return true;
    return false;
}
