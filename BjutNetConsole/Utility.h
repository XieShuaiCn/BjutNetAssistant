#ifndef BNC_UTILITY_H
#define BNC_UTILITY_H

#include <string>
#include <vector>

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#define BNA_OS_WIN64
#endif
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WINDOWS)
#define BNA_OS_WIN32
#endif
#if defined(BNA_OS_WIN32) || defined(BNA_OS_WIN64)
#define BNA_OS_WIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define BNA_OS_LINUX
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#define BNA_OS_MAC
#endif

namespace bna{

bool ConvertUtf8ToMultiBytes(const char *src, int srclen, char *&dst, int &dstlen);

bool CheckUtf8ToMultiBytes(std::string &src_dst);

void ConsoleInputPasswd(std::string &passwd, char echo);

bool ListLocalIpAddress(std::vector<std::string> &ip);

////////////////////////////implements/////////////////////////////////

#ifndef BNA_OS_WIN
inline bool CheckUtf8ToMultiBytes(std::string &src_dst) {(void)src_dst; return true;}
#endif
}
#endif // BNC_UTILITY_H
