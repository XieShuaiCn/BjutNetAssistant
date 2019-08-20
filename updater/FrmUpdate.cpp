#include "FrmUpdate.h"
#include <tuple>
#include <QLabel>
#include <QProgressBar>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QDirIterator>
#include <QCryptographicHash>
#include "HttpClient.h"

extern QString g_strDistPath;

FrmUpdate::FrmUpdate(QWidget* parent)
    : FrmStep(parent)
{
    m_lblInfo = new QLabel(this);
    m_lblInfo->setGeometry(20, 15, this->width()-40, 30);
    m_lblInfo->setText("Updating from server...");
    m_barUpdate = new QProgressBar(this);
    m_barUpdate->setGeometry(20, 45, this->width()-40, 30);
    m_barUpdate->setRange(0, 100);
}

bool FrmUpdate::canCancel()
{
    return m_bCanCancel;
}

bool FrmUpdate::canGoLast()
{
    return m_bCanLast;
}

bool FrmUpdate::canGoNext()
{
    return m_bCanNext;
}

bool FrmUpdate::doProcess()
{
    using file_tuple = std::tuple<QString, int, QString, int>;
    m_bCanCancel = true;
    m_bCanLast = true;
    m_bCanNext = false;

    QVector<QString> vecHost;
    QVector<file_tuple> vecServerFiles;
    QVector<file_tuple> vecClientFiles;
    QString strUrlPlatform(
#ifdef Q_OS_WIN
                "windows"
#elif defined(Q_OS_LINUX)
                "linux64"
#endif
                );
    m_barUpdate->setValue(0);
    QDir dirBin(g_strDistPath);
    // compute md5
    QDirIterator iterDir(dirBin, QDirIterator::Subdirectories);
    m_lblInfo->setText("Go through local files...");
    while(iterDir.hasNext())
    {
        iterDir.next();
        auto fi = iterDir.fileInfo();
        if(!fi.isFile()){
            continue;
        }
        auto nm = fi.filePath();
        if(nm.endsWith("/..") || nm.endsWith("/.")){
            continue;
        }
        if(nm.startsWith("./")){
            nm = nm.right(nm.size()-2);
        }
        else{
            if(nm.startsWith(g_strDistPath)){
                if(g_strDistPath.endsWith('/')){
                    nm = nm.right(nm.size()-g_strDistPath.size());
                }
                else {
                    nm = nm.right(nm.size()-g_strDistPath.size()-1);
                }
            }
        }
        auto size = fi.size();
        QCryptographicHash hash(QCryptographicHash::Md5);
        QFile fd(fi.absoluteFilePath());
        if(fd.open(QFile::ReadOnly)){
            hash.addData(fd.readAll());
        }
        vecClientFiles.push_back(file_tuple(nm, size, QString(hash.result().toHex()), 0));
    }
    m_barUpdate->setValue(20);

    HttpClient http;
    QString content;
    int http_code, host_index;
    m_lblInfo->setText("Downloading mainfest...");
    http_code = http.getUrlHtml(QUrl("http://bnl.hrrcn.com/online/host.php"), content);
    if(200 != http_code){
        m_lblInfo->setText("Fail to get host list.");
        return false;
    }
    QJsonParseError jp_err;
    const QJsonDocument jdoc = QJsonDocument::fromJson(content.toLocal8Bit(), &jp_err);
    if(jp_err.error == QJsonParseError::NoError && jdoc.isArray())
    {
        const QJsonArray ja = jdoc.array();
        for(const auto &a : ja){
            vecHost.push_back(a.toString());
        }
        if(vecHost.empty()){
            m_lblInfo->setText("Empty host list.");
            return false;
        }
    }
    else{
        m_lblInfo->setText("Fail to parse host list.");
        return false;
    }
    for(host_index=0, http_code=0, content.clear(); host_index < vecHost.size() && http_code != 200; ++host_index){
        m_lblInfo->setText(QString("Downloading mainfest... (#%1)").arg(host_index));
        http_code = http.getUrlHtml(QUrl(vecHost[host_index] + "online/" + strUrlPlatform + "/content.json"), content);
    }
    --host_index;
    m_barUpdate->setValue(30);
    if(200 == http_code)
    {
        const QJsonDocument jdoc = QJsonDocument::fromJson(content.toLocal8Bit(), &jp_err);
        if(jp_err.error == QJsonParseError::NoError)
        {
            const QJsonObject jo = jdoc.object();
            if(jo.contains("platform"))
            {
                if(jo["platform"].toString() !=
#ifdef Q_OS_WIN
                        "Windows"
#elif defined(Q_OS_LINUX)
                        "Linux64"
#endif
                        ){
                    m_lblInfo->setText("Error platform mainfest.");
                    return false;
                }
            }
            if(jo.contains("files")) {
                if(jo["files"].isArray()){
                    QJsonArray json_files = jo["files"].toArray();
                    for(auto f : json_files)
                    {
                        auto jf = f.toObject();
                        if(jf.contains("name") && jf.contains("size") && jf.contains("sum"))
                            vecServerFiles.push_back(file_tuple(
                                            jf["name"].toString(),
                                            jf["size"].toInt(),
                                            jf["sum"].toString(),
                                            jf["permission"].toInt()
                                    ));
                    }
                    m_barUpdate->setValue(50);
                    QDir distDir(g_strDistPath);
                    m_lblInfo->setText("Downloading files...");
                    for(int i = 0; i < vecServerFiles.size(); ++i)
                    {
                        file_tuple &ft_s = vecServerFiles[i];
                        file_tuple ft_c;
                        QString &fname = std::get<0>(ft_s);
                        for(auto &fc : vecClientFiles)
                        {
                            if(std::get<0>(fc) == fname)
                            {
                                ft_c = fc;
                                break;
                            }
                        }
                        if(fname.contains('/') || fname.contains('\\')){
                            int iii = std::get<0>(ft_s).lastIndexOf('/');
                            int jjj = std::get<0>(ft_s).lastIndexOf('\\');
                            int kkk = std::max(iii, jjj);
                            if(kkk > 0){
                                distDir.mkpath(fname.left(kkk));
                            }
                        }
                        if(std::get<2>(ft_s).size() == 0 || std::get<2>(ft_s)!=std::get<2>(ft_c)){
                            QString tempFile = distDir.absoluteFilePath(fname);
                            QString urlFile = vecHost[host_index] + "online/" + strUrlPlatform + "/files/" + fname;
                            http_code = http.downloadFile(
                                        QUrl(urlFile),
                                        QByteArray(), tempFile, false);
                            if(http_code != 200)
                            {
                                m_lblInfo->setText("Fail to download file: " + std::get<0>(ft_s));
                                return false;
                            }
                            QFile::setPermissions(fname, QFile::Permissions(std::get<3>(ft_s)));
                        }
                        m_barUpdate->setValue(50.0 + 40.0 * i / vecServerFiles.size());
                    }
                    m_barUpdate->setValue(100);
                    m_bCanNext = true;
                    m_bCanLast = false;
                    m_bCanCancel = false;
                    return true;
                }
                else{
                    m_lblInfo->setText("Can not parse file list");

                }
            }
            else
            {
                m_lblInfo->setText("Can not get file list");
            }
        }
        else
        {
            m_lblInfo->setText("Can not parse json.");
        }
    }
    else {
        m_lblInfo->setText("Can not connect to server.");
    }
    return false;
}
