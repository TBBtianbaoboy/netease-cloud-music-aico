//---------------------------------
//File Name    : json.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 23:14:05
//Description  : 
//----------------------------------
#include "json/json.h"
#include <fstream>
#include "file/file.h"

using std::fstream;
JSON_HANDLER::JSON_HANDLER(const string& filename){
    file_name = filename;
}

JSON_HANDLER::~JSON_HANDLER(){}

bool JSON_HANDLER::WriteJsonData()
{
    std::ifstream fin(file_name);
    if(fin.is_open()){
        fin>>data;
        fin.close();
        return true;
    }
    fin.close();
    return false;
}

json JSON_HANDLER::Json()
{
    return data;
}
