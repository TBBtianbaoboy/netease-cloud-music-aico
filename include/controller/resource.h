//---------------------------------
//File Name    : route.h
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-16 15:42:59
//Description  : 
//----------------------------------
#ifndef RESOURCE_H
#define RESOURCE_H
#include "../file/file.h"
#include "../http/http.h"
#include "../json/json.h"

class ResourceHandler{
    private:
        FILE_HANDLER file_handler;
        CURL_HANDLER curl_handler;
        JSON_HANDLER json_handler;
        string resourcePath;
    public:
        ResourceHandler(const string&,const string&);
        //是否移除文件数据
        bool Run(bool);
        JSON_HANDLER& GetData();
};

#endif //RESOURCE_H
