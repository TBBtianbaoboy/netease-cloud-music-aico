//---------------------------------
//File Name    : json.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-15 23:06:08
//Description  : 
//----------------------------------
#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

using nlohmann::json;
using std::string;

class JSON_HANDLER{
    private:
        json data;
        string file_name;
    public:
        JSON_HANDLER(const string& filename);
        ~JSON_HANDLER();
        JSON_HANDLER(const JSON_HANDLER&)=delete;
        JSON_HANDLER& operator=(const JSON_HANDLER&)=delete;
        JSON_HANDLER(JSON_HANDLER&&)=delete;

        bool WriteJsonData();
        json Json();
};

