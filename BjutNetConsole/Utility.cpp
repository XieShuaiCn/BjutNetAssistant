#ifdef _WIN32
#include <windows.h>
#endif
#include "Utility.h"

bool ConvertUtf8ToMultiBytes(const char *src, int srclen, char *&dst, int &dstlen)
{
    int wcslen = ::MultiByteToWideChar(CP_UTF8, 0, src, srclen, nullptr, 0);
    wchar_t *wstr = new wchar_t[wcslen+1];
    wcslen = ::MultiByteToWideChar(CP_UTF8, 0, src, srclen, wstr, wcslen);
    wstr[wcslen] = 0;
    if(wcslen > 0){
        dstlen = ::WideCharToMultiByte(CP_ACP, 0, wstr, wcslen, nullptr, 0, nullptr, nullptr);
        dst = new char[dstlen+1];
        dstlen = ::WideCharToMultiByte(CP_ACP, 0, wstr, wcslen, dst, dstlen, nullptr, nullptr);
        dst[dstlen] = 0;
        delete wstr;
        return dstlen > 0;
    }
    delete wstr;
    return false;
}

bool CheckUtf8ToMultiBytes(const std::string &src, std::string &dst)
{
#ifdef _WIN32
    bool bNeedConvert = false;
    for(const char &ch : src){
        if(ch < 0){
            bNeedConvert = true;
            break;
        }
    }
    if(bNeedConvert){
        char *mbstr = nullptr;
        int mblen = 0;
        if(!ConvertUtf8ToMultiBytes(src.data(), src.size(), mbstr, mblen)) {
            return false;
        }
        dst.assign(mbstr);
        delete mbstr;
    }
    else{
        dst.assign(src);
    }
#else
    dst.assign(src);
#endif
    return true;
}
