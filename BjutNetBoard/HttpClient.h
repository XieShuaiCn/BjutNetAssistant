#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "common.h"
#include <QtCore/QUrl>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAccessManager>

class bna_gui_HttpClient : public QObject
{
    Q_OBJECT
public:
    bna_gui_HttpClient();
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
    QNetworkAccessManager m_netMan;
    QTextCodec *m_pCodec = nullptr;
};


inline int bna_gui_HttpClient::downUrlData(const QUrl &url, QByteArray &content)
{
    return downUrlData(url, QByteArray(), content, false);
}

namespace bna{
namespace gui{
class HttpClient : public bna_gui_HttpClient {};
}
}
#endif // HTTPCLIENT_H
