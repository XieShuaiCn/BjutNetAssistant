#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "common.h"
#include "HNetworkCookieJar.h"
#include <QUrl>
#include <QNetworkAccessManager>

namespace bna{
namespace core{

class HttpClient : public QObject
{
    Q_OBJECT
public:
    HttpClient();
    //~HttpClient();
    QString getUrlHtml(const QUrl &url);
    int getUrlHtml(const QUrl &url, QString &content);
    QString postUrlHtml(const QUrl &url, const QMap<QString, QString> &data);
    int postUrlHtml(const QUrl &url, const QMap<QString, QString> &data, QString &content);
    QString postUrlHtml(const QUrl &url, const QString &arg);
    int postUrlHtml(const QUrl &url, const QString &arg, QString &content);
    int downloadFile(const QUrl &url, const QByteArray &arg, const QString &fileName, bool bPost=true);

    int downUrlData(const QUrl &url, QByteArray &content);
    int downUrlData(const QUrl &url, const QByteArray &arg, QByteArray &content, bool bPost=true);

    bool setCodec(const QByteArray &codeName);
    bool pop_message(QDateTime& time, QString& info);
    void clear_message();
signals:
    void uploadProgress(qint64 bytesSent,qint64 bytesTotal);
    void downloadProgress(qint64 bytesDown,qint64 bytesTotal);
protected:
    void push_message(const QDateTime& time, const QString& info);
    QList<QPair<QDateTime, QString> > m_arrMessage;
    //
    HNetworkCookieJar *m_netCookie;
    QNetworkAccessManager m_netMan;
    QTextCodec *m_pCodec = nullptr;
};


inline int HttpClient::downUrlData(const QUrl &url, QByteArray &content)
{
    return downUrlData(url, QByteArray(), content, false);
}
}}
#endif // HTTPCLIENT_H
