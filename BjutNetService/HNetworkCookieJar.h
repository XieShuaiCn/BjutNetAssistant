#ifndef HNETWORKCOOKIEJAR_H
#define HNETWORKCOOKIEJAR_H

#include <QList>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

namespace bna{
namespace core{

class HNetworkCookieJar : public QNetworkCookieJar
{
public:
    HNetworkCookieJar() = default;
    // TODO::析构有问题
    ~HNetworkCookieJar();
    QList<QNetworkCookie> getCookies() const;
    QList<QNetworkCookie> getCookies(const QUrl &url) const;
    QByteArray getCookieBytes() const;
    QByteArray getCookieBytes(const QUrl &url) const;
};

}}
#endif // HNETWORKCOOKIEJAR_H
