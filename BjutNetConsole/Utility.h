#ifndef BNC_UTILITY_H
#define BNC_UTILITY_H

#include <string>

bool ConvertUtf8ToMultiBytes(const char *src, int srclen, char *&dst, int &dstlen);

bool CheckUtf8ToMultiBytes(const std::string &src, std::string &dst);

#endif // BNC_UTILITY_H
