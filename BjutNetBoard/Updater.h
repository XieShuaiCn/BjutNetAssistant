#ifndef UPDATER_H
#define UPDATER_H

#include "../BjutNetService/common.h"
#include "../BjutNetService/HttpClient.h"

namespace bna {
namespace gui {

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr);

    bool checkUpdate();
    bool needUpdate();

    const QString &getOldVersion() { return m_strOldVersion; }
    const QString &getNewVersion() { return m_strNewVersion; }
    int getOldVersionNum() { return m_nOldVersion; }
    int getNewVersionNum() { return m_nNewVersion; }
    QString getNewIntroduction() { return m_strNewIntroduction; }
    bool downloadNewPackage();
    bool doDownload(const QString &online_path, const QString &local_path);
    bool doInstall(const QString &local_path);
    bool runMaintainTool();
signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
protected:
    int getUrl(QUrl url, QString &content);
    int m_nNewVersion = 0;
    int m_nOldVersion = 0;
    QString m_strNewVersion;
    QString m_strOldVersion;
    QString m_strNewIntroduction;
    QString m_strOnlineFileURL;
    bna::core::HttpClient m_http;
    const QString m_strHostName = "http://bnl.hrrcn.com/";
    const QString m_strOssHostName = "http://bjutnetlogin.oss-cn-beijing.aliyuncs.com/";
};
}}
#endif // UPDATER_H
