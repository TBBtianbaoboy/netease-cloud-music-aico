//---------------------------------
//File Name    : utils.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-17 23:15:50
//Description  : 
//----------------------------------
#include "utils/utils.h"
#include <cctype>
#include <openssl/md5.h>
#include <cstring>
#include <regex>

string Utils::ConvertIntToTime(int seconds)
{
    int hour = seconds / 3600;
    int minute = seconds / 60 % 60;
    int second = seconds % 60;
    if (hour==0)
        return to_string(minute)+":"+to_string(second);
    else 
        return to_string(hour)+to_string(minute)+":"+to_string(second);
}

string Utils::MD5(const string& str)
{
    MD5_CTX ctx;

    string md5_string;
    unsigned char md[16] = { 0 };
    char tmp[33] = { 0 };

    MD5_Init( &ctx );
    MD5_Update( &ctx, str.c_str(), str.size() );
    MD5_Final( md, &ctx );

    for( int i = 0; i < 16; ++i )
    {   
        memset( tmp, 0x00, sizeof( tmp ) );
        sprintf( tmp, "%02X", md[i] );
        md5_string += tmp;
    }
    return Utils::StringToLower(md5_string);
}

bool Utils::CheckPhone(char *phone)
{
    std::regex rule("^1(\\d{10})$");
    return std::regex_match(string(phone),rule);
}

string Utils::StringToLower(string &str)
{
    for(int i=0;i<str.size();++i)
    {
        if(str[i]>='A' && str[i]<='Z')
            str[i] += 32;
    }
    return str;
}

string Utils::EncodeUri(string str)
{ 
    std::ostringstream oss;
    std::regex r("[!'\\(\\)*-.0-9A-Za-z_~]");

    for (char &c : str)
    {
        if (std::regex_match((std::string){c}, r))
        {
            oss << c;
        }
        else
        {
            oss << "%" << std::uppercase << std::hex << (0xff & c);
        }
    }
    return oss.str();
}
