//---------------------------------
//File Name    : utils.cpp
//Author       : aico
//Mail         : 2237616014@qq.com
//Github       : https://github.com/TBBtianbaoboy
//Site         : https://www.lengyangyu520.cn
//Create Time  : 2021-11-16 16:31:40
//Description  : 
//----------------------------------
#ifndef UTILS_H
#define UTILS_H
#include <string>

using std::to_string;
using std::string;

class Utils {
    public:
        template<typename Type>
        static string ToString(Type);
        
        static string ConvertIntToTime(int seconds);
        static string MD5(const string&);
        static bool CheckPhone(char*);
        static string StringToLower(string&);
        static string EncodeUri(string);
};

template<typename Type>
string Utils::ToString(Type value){
    return to_string(value);
}
#endif //UTILS_H
