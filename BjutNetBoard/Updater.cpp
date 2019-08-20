#include "Updater.h"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QUrl>
#include <QDir>
#include <QProcess>
#include <QEventLoop>
#include <QTextCodec>
#include <QStandardPaths>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "../BjutNetService/Version.h"

namespace bna{
namespace gui{

Updater::Updater(QObject *parent) : QObject(parent),
    m_nNewVersion(0), m_nOldVersion(BNA_INNER_VERSION),
    m_strNewVersion("0.0.0"), m_strOldVersion(BNA_VERSION)
{
    connect(&m_http, &HttpClient::downloadProgress, this, &Updater::downloadProgress);
}

bool Updater::checkUpdate()
{
    bool success = false;
    QString content;
    int ret = m_http.getUrlHtml(QUrl(m_strHostName + "online/newest.php?os="
#ifdef Q_OS_WIN
                          "Windows"
#elif defined(Q_OS_LINUX)
                          "Linux64"
#endif
                     ), content);
    if(ret != 200){
        return false;
    }

    //{"success":true,"version":"1.4","inner_ver":"7","introduction":"1\u3001\u7a0b\u5e8f\u521d\u59cb\u5316\u5931\u8d25\u65f6\uff0c\u76f4\u63a5\u7ed3\u675f\r\n2\u3001\u6bcf\u9694\u534a\u5c0f\u65f6\u8f93\u51fa\u4e00\u6b21\u8d26\u53f7\u6d41\u91cf\u4fe1\u606f"}
    QJsonParseError jp_err;
    const QJsonDocument jdoc = QJsonDocument::fromJson(content.toLocal8Bit(), &jp_err);
    if(jp_err.error == QJsonParseError::NoError)
    {
        const QJsonObject jo = jdoc.object();
        if(jo.contains("success"))
        {
            success = jo["success"].toBool();
        }
        if(success){
            if(jo.contains("version")) {
                m_strNewVersion = (jo["version"].toString());
            }
            if(jo.contains("inner_ver")) {
                m_nNewVersion = (jo["inner_ver"].toInt());
            }
            if(jo.contains("introduction")){
                m_strNewIntroduction = (jo["introduction"].toString());
            }
        }
    }
    return success;
}

bool Updater::needUpdate()
{
    return m_nNewVersion > m_nOldVersion;
}

bool Updater::downloadNewPackage()
{
    if(!checkUpdate() || !needUpdate()) {
        return false;
    }
    //http://bnl.hrrcn.com/online/download.php?ver=13&os=Windows
    //{"success":true,"version":"3.0.0","platform":"Windows","file":"offline\/BjutNetLogin_3.0.0_Win32.exe"}
    QString content;
    int ret = m_http.getUrlHtml(QUrl(QString(m_strHostName + "online/download.php?ver=%1&os="
#if defined(Q_OS_WIN)
                          "Windows"
#elif defined(Q_OS_LINUX)
                          "Linux64"
#else
                          "Any"
#endif
                          ).arg(m_nNewVersion)), content);
    if(ret != 200){
        return false;
    }
    QJsonParseError jp_err;
    const QJsonDocument jdoc = QJsonDocument::fromJson(content.toLocal8Bit(), &jp_err);
    if(jp_err.error == QJsonParseError::NoError) {
        const QJsonObject jo = jdoc.object();
        if(jo.contains("file")) {
            m_strOnlineFileURL = (jo["file"].toString());
            if(m_strOnlineFileURL.isEmpty() || m_strOnlineFileURL.startsWith('#')){
                return false;
            }
            QFileInfo fi(m_strOnlineFileURL);
            //生成temp目录
            QString tempFile = QDir(g_strAppTempPath).absoluteFilePath(fi.fileName());
            QFile fileTemp(tempFile);
            if(fileTemp.exists())
            {
                fileTemp.remove();
            }
            fileTemp.close();
            ret = m_http.downloadFile(QUrl(m_strOnlineFileURL), QByteArray(), tempFile, false);

#ifdef QT_DEBUG
            bool suc =
#endif
            QFile(tempFile).setPermissions(QFile::Permission(0x7755));
#ifdef QT_DEBUG
            if (!suc) qDebug() << "Change permission faild." << endl;
#endif
            return 200 == ret && doInstall(tempFile);
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Updater::doDownload(const QString & online_path, const QString & local_path)
{
    if( 200 == m_http.downloadFile(QUrl(online_path), QByteArray(), local_path, false))
    {
        return true;
    }
    return false;
}

bool Updater::doInstall(const QString &local_path)
{
    return QProcess::startDetached(local_path);
}

bool Updater::runMaintainTool()
{
#ifdef Q_OS_WIN32
    return QProcess::startDetached("maintain.exe");
#else
    QString binFile = QDir(QApplication::applicationDirPath())
            .absoluteFilePath("maintain.sh");
    return QProcess::startDetached(binFile);
#endif
}
}}
