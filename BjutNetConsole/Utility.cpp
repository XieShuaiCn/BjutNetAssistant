
#if defined(_WIN32) || defined(WIN32) || defined(_WINDOWS) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
#include <windows.h>
#endif
#include "Utility.h"
#include <stdio.h>
#include <string.h>

#ifdef BNA_OS_WIN
#include <conio.h>
#endif
#ifdef BNA_OS_LINUX
#include <termio.h>
#endif
#if defined(BNA_OS_LINUX) || defined(BNA_OS_MAC)
#include <unistd.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif

#ifndef BNA_OS_LINUX
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/host_name.hpp>
#endif

namespace bna{

bool ConvertUtf8ToMultiBytes(const char *src, int srclen, char *&dst, int &dstlen)
{
#if defined(BNA_OS_WIN)
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
#else
    (void)(srclen);
    (void)(dstlen);
    strcpy(dst, src);
#endif
    return true;
}

#if defined(BNA_OS_WIN)

bool CheckUtf8ToMultiBytes(std::string &src)
{
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
        src.assign(mbstr);
        delete mbstr;
    }
    return true;
}

#endif

void ConsoleInputPasswd(std::string &passwd, char echo)
{
#ifdef BNA_OS_WIN
    // Windows system
    char ch;
    while((ch=getch())!='\r')
    {
        if(ch!=8)//不是回撤就录入
        {
            passwd.push_back(ch);
            putchar(echo);//并且输出*号
        }
        else if(passwd.size())
        {
            putchar('\b');//这里是删除一个，我们通过输出回撤符 /b，回撤一格，
            putchar(' ');//再显示空格符把刚才的*给盖住，
            putchar('\b');//然后再 回撤一格等待录入。
            passwd.pop_back();
        }
    }
    passwd.push_back('\0');
    putchar('\r');
    putchar('\n');

#elif defined(BNA_OS_LINUX)

#if 1
    // linux system, using getpass function in sytem library
    (void)echo;
    passwd.assign(getpass(""));
#else
    // linux system, by changing terminal attribute in sytem library
    #define TERM_ECHOFLAGS (ECHO|ECHOE|ECHOK|ECHONL)
    char ch;
    struct termios termios_buf;
    tcgetattr(STDIN_FILENO,&termios_buf);
    termios_buf.c_lflag &= ~TERM_ECHOFLAGS;
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&termios_buf);
    fflush(stdout);
    fflush(stdin);
    //TODO: 第一个字符总是\n，怎么办
    while((ch=getchar())!='\r' && ch!='\n')
    {
        tcgetattr(STDIN_FILENO,&termios_buf);
        termios_buf.c_lflag |= TERM_ECHOFLAGS;
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&termios_buf);
        if(ch!=8)//不是回撤就录入
        {
            passwd.push_back(ch);
            putchar(echo);//并且输出*号
        }
        else if(passwd.size())
        {
            putchar('\b');//这里是删除一个，我们通过输出回撤符 /b，回撤一格，
            putchar(' ');//再显示空格符把刚才的*给盖住，
            putchar('\b');//然后再 回撤一格等待录入。
            passwd.pop_back();
        }
        tcgetattr(STDIN_FILENO,&termios_buf);
        termios_buf.c_lflag &= ~TERM_ECHOFLAGS;
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&termios_buf);
    }
    tcgetattr(STDIN_FILENO,&termios_buf);
    termios_buf.c_lflag |= TERM_ECHOFLAGS;
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&termios_buf);
    passwd.push_back('\0');
    putchar('\r');
    putchar('\n');
    #undef TERM_ECHOFLAGS
#endif

#elif defined(BNA_OS_MAC)
    (void)echo;
    passwd.assign(getpass(""));
#endif
}

bool ListLocalIpAddress(std::vector<std::string> &ip)
{
#if defined(BNA_OS_LINUX) || defined(BNA_OS_MAC)
    struct ifaddrs *ifap0=nullptr, *ifap=nullptr;
    void * tmpAddrPtr=nullptr;
    getifaddrs(&ifap0);
    ifap=ifap0;
    while (ifap) {
        if(0==strncmp(ifap->ifa_name, "vmnet", 5) || 0==strncmp(ifap->ifa_name, "lo", 2)){
            ifap = ifap->ifa_next;
            continue;
        }
        if (ifap->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifap->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            ip.emplace_back(addressBuffer);
        } else if (ifap->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifap->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            ip.emplace_back(addressBuffer);
        }
        ifap=ifap->ifa_next;
    }
    if (ifap0) {
        freeifaddrs(ifap0);
        ifap = ifap0 = nullptr;
    }
    return true;
#else
    // other platforms. Using boost library.
    using namespace boost;
    using udp = boost::asio::ip::udp;
    boost::system::error_code err;
    boost::asio::io_context ip_context;
    udp::resolver ipresolver(ip_context);
    udp::resolver::query query(boost::asio::ip::host_name(err), "");
    udp::resolver::iterator it = ipresolver.resolve(query, err);
    udp::resolver::iterator end;
    for(; it != end; ++it){
        udp::endpoint endpoint = *it;
        ip.emplace_back(endpoint.address().to_string(err));
    }
    return true;
#endif
}

}
