//---------------------------------
// File Name    : resource.cpp
// Author       : aico
// Mail         : 2237616014@qq.com
// Github       : https://github.com/TBBtianbaoboy
// Site         : https://www.lengyangyu520.cn
// Create Time  : 2021-11-16 15:56:03
// Description  :
//----------------------------------
#include "controller/resource.h"

ResourceHandler::ResourceHandler(const string& resource_path,const string& filename) : file_handler(filename),json_handler(filename),resourcePath(resource_path) {}

bool ResourceHandler::Run(bool isRemove) {
  if (curl_handler.HandleUrl(resourcePath.c_str())&&
      file_handler.IsOpenFile()) {
    file_handler.WriteNewFile(curl_handler.GetData());
    file_handler.CloseFile();

    if(!json_handler.WriteJsonData())
        return false;
    if(isRemove)
        FILE_HANDLER::RemoveFile(file_handler.GetFileName());
    return true;
  }
  return false;
}

JSON_HANDLER& ResourceHandler::GetData()
{
    return json_handler;
}
