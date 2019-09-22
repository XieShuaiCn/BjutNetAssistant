#include "HttpClient.h"
#include <QFile>
#include <QBuffer>
#include <QEventLoop>
#include <QTextCodec>
#include <QtAlgorithms>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace bna{
namespace core{

HttpClient::HttpClient()
{
    m_pCodec = QTextCodec::codecForName("UTF-8");
    m_netCookie = new HNetworkCookieJar();//不用释放，该内存由m_netMan管理
    m_netMan.setCookieJar(m_netCookie);
}

//HttpClient::~HttpClient()
//{
//    if (m_pCodec)
//    {
//        delete m_pCodec;
//    }
//}

bool HttpClient::setCodec(const QByteArray &codeName)
{
//    if (m_pCodec)
//    {
//        delete m_pCodec;//不需要析构，统一由QT管理
//    }
    if (codeName == m_pCodec->name())
    {
        return true;
    }
    static const QList<QByteArray> availableCodecs = QTextCodec::availableCodecs();
    bool avaiable = false;
    QByteArray lower_name = codeName.toLower();
    for (const auto it : availableCodecs)
    {
        if(lower_name.size() == it.size() && lower_name.startsWith(it.toLower()))
        {
            avaiable = true;
            break;
        }
    }
    if(avaiable)
    {
        m_pCodec = QTextCodec::codecForName(codeName);
        return true;
    }
    return false;
}

void HttpClient::push_message(const QDateTime &time, const QString &info)
{
    m_arrMessage.push_back(QPair<QDateTime, QString>(time, info));
}

bool HttpClient::pop_message(QDateTime& time, QString& info)
{
    if(m_arrMessage.size())
    {
        auto it = m_arrMessage.constBegin();
        time = it->first;
        info = it->second;
        m_arrMessage.pop_front();
        return true;
    }
    return false;
}

void HttpClient::clear_message()
{
    m_arrMessage.clear();
}

QString HttpClient::getUrlHtml(const QUrl &url)
{
    QString content;
    getUrlHtml(url, content);
    return content;
}

int HttpClient::getUrlHtml(const QUrl &url, QString &content)
{
    QByteArray data;
    content.clear();
    int status = downUrlData(url, QByteArray(), data, false);
    //数据转码
    content.append(m_pCodec->toUnicode(data));
    return status;
}

QString HttpClient::postUrlHtml(const QUrl &url, const QMap<QString, QString> &data)
{
    QString content;
    postUrlHtml(url, data, content);
    return content;
}

int HttpClient::postUrlHtml(const QUrl &url, const QMap<QString, QString> &data, QString &content)
{
    QString arg;
    auto end = data.cend();
    for(auto it = data.cbegin(); it != end; ++it)
    {
        arg.append(it.key());
        arg.append('=');
        arg.append(it.value());
        arg.append('&');
    }
    if(arg.endsWith('&'))
    {
        arg.remove(arg.size()-1, 1);
    }
    return postUrlHtml(url, arg, content);
}

QString HttpClient::postUrlHtml(const QUrl &url, const QString &arg)
{
    QString content;
    postUrlHtml(url, arg, content);
    return content;
}

int HttpClient::postUrlHtml(const QUrl &url, const QString &arg, QString &content)
{
    QByteArray postArray;
    QByteArray data;

    postArray.append(arg);
    content.clear();
    int status = downUrlData(url, postArray, data, true);
    //数据转码
    if(data.size())
    {
        content.append(m_pCodec->toUnicode(data));
    }
    return status;
}

int HttpClient::downloadFile(const QUrl &url, const QByteArray &arg, const QString &fileName, bool bPost)
{
    //建立http连接
    QNetworkRequest request;
    if(url.scheme() == "https"){
        //ssl连接
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);
    }
    QEventLoop loop;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.167 Safari/537.36");

    if(bPost)
    {
        request.setHeader(QNetworkRequest::ContentLengthHeader,
                          arg.size());
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");
    }
//    QBuffer bufArg(&arg);
//    bufArg.open(QIODevice::ReadOnly);
//    m_netMan.createRequest(QNetworkAccessManager::PostOperation,
//                           request, &bufArg);
//    bufArg.close();
    request.setRawHeader("Cookie", m_netCookie->getCookieBytes());
    QNetworkReply *pReply = bPost ? m_netMan.post(request, arg) :
                                    m_netMan.get(request);

    //设置请求回应的回调
    connect(pReply, SIGNAL(downloadProgress(qint64,qint64)) , this , SIGNAL(downloadProgress(qint64,qint64)));
    connect(pReply, SIGNAL(finished()) , &loop , SLOT(quit()));
    //等待回应
    loop.exec();

    if(pReply->error() != QNetworkReply::NoError)//error
    {
        push_message(QDateTime::currentDateTime(), QString("Down url error:%1").arg(pReply->errorString()));
        pReply->disconnect();
        pReply->deleteLater();
        return pReply->error();
    }
    int status = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //int status = pReply->rawHeader("status").toInt();
    //读取
    QFile file(fileName);
    if(status == 200 && file.open(QIODevice::ReadWrite))
    {
        int len = 1024*128;
        char *data = new char[1024*128];
        do{
            len = pReply->read(data, 1024*128);
            if(len > 0)
            {
                file.write(data, len);
            }
        }while(len > 0);
        file.close();
    }
    pReply->disconnect();
    pReply->close();
    //pReply->deleteLater();
    delete pReply;
    return status;
}

int HttpClient::downUrlData(const QUrl &url, const QByteArray &arg, QByteArray &content, bool bPost/*=true*/)
{
    //建立http连接
    QNetworkRequest request;
    if(url.scheme() == "https"){
        //ssl连接
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);
    }
    QEventLoop loop;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.167 Safari/537.36");

    if(bPost)
    {
        request.setHeader(QNetworkRequest::ContentLengthHeader,
                          arg.size());
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");
    }
//    QBuffer bufArg(&arg);
//    bufArg.open(QIODevice::ReadOnly);
//    m_netMan.createRequest(QNetworkAccessManager::PostOperation,
//                           request, &bufArg);
//    bufArg.close();
    request.setRawHeader("Cookie", m_netCookie->getCookieBytes());
    QNetworkReply *pReply = bPost ? m_netMan.post(request, arg) :
                                    m_netMan.get(request);

    //设置请求回应的回调
    connect(pReply , SIGNAL(finished()) , &loop , SLOT(quit()));
    //等待回应
    loop.exec();

    if(pReply->error() != QNetworkReply::NoError)//error
    {
        push_message(QDateTime::currentDateTime(), QString("Down url error:%1").arg(pReply->errorString()));
        return pReply->error();
    }
    //读取
    content = pReply->readAll();
#ifdef QT_DEBUG
    if(content.size() == 0)
        qDebug() << "Waring: downUrlData(data.size=0): " << pReply->errorString();
#endif
    int status = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //int status = pReply->rawHeader("status").toInt();
    pReply->close();
    //pReply->deleteLater();
    delete pReply;
    return status;
}
}}
