#include "HNetworkCookieJar.h"

namespace bna{
namespace core{

HNetworkCookieJar::~HNetworkCookieJar()
{

}

QList<QNetworkCookie> HNetworkCookieJar::getCookies() const
{
    return allCookies();
}

QList<QNetworkCookie> HNetworkCookieJar::getCookies(const QUrl &url) const
{
    return cookiesForUrl(url);
}

QByteArray HNetworkCookieJar::getCookieBytes() const
{
    QByteArray data;
    for(const auto &it : allCookies())
    {
        if(data.size())
        {
            data.push_back("; ");
        }
        data.push_back(it.name());
        data.push_back('=');
        data.push_back(it.value());
    }
    return data;
}

QByteArray HNetworkCookieJar::getCookieBytes(const QUrl &url) const
{
    QByteArray data;
    for(const auto &it : cookiesForUrl(url))
    {
        if(data.size())
        {
            data.push_back("; ");
        }
        data.push_back(it.name());
        data.push_back('=');
        data.push_back(it.value());
    }
    return data;
}
}
}
