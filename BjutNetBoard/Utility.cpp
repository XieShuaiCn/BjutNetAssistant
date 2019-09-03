#include "Utility.h"

#include <QHostInfo>
#ifdef Q_OS_UNIX
#include <ifaddrs.h>
#include <arpa/inet.h>
#endif

namespace bna{
namespace gui {

bool ListLocalIpAddress(QVector<QHostAddress> &addrs)
{
#ifdef Q_OS_UNIX
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
            addrs.push_back(QHostAddress(QString::fromUtf8(addressBuffer)));
        } else if (ifap->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifap->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            addrs.push_back(QHostAddress(QString::fromUtf8(addressBuffer)));
        }
        ifap=ifap->ifa_next;
    }
    if (ifap0) {
        freeifaddrs(ifap0);
        ifap = ifap0 = nullptr;
    }
    return true;
#else
    QHostInfo host_info = QHostInfo::fromName(QHostInfo::localHostName());
    for(const auto &h : host_info.addresses()){
        if(h.isLoopback()) continue;
        addrs.append(h);
    }
    return true;
#endif
}

}
}
