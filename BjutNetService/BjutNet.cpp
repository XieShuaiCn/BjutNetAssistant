#include "BjutNet.h"
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

namespace bna{

BjutNet::BjutNet():
    QThread()
{
    m_bRun = false;
    connect(&m_webLgn, &WebLgn::message, this, &BjutNet::message);
    connect(&m_webJfself, &WebJfself::message, this, &BjutNet::message);
    connect(&m_tmCheckLgn, &QTimer::timeout, this, &BjutNet::checkLgn);
    connect(&m_tmCheckOnline, &QTimer::timeout, this, &BjutNet::checkOnline);
    connect(this, &BjutNet::debug_info, &g_debugTool, &DebugTool::writeInfo);
}


bool BjutNet::loadAccount(const QString path)
{
    QFileInfo *fi;
    if(path.length() == 0)
    {
        QString conf_file_home = (QDir::homePath() + "/.bjutnet/account.json");
        fi = new QFileInfo(conf_file_home);
        if(!fi->exists())
        {
#ifndef Q_OS_WIN
            delete fi;
            QString conf_file_etc = "/etc/bjutnet.d/account.json";
            fi = new QFileInfo(conf_file_etc);
            if(!fi->exists())
            {
#endif
                delete fi;
                QString conf_file_model = (QDir::currentPath() + "/account.json");
                fi = new QFileInfo(conf_file_model);
                if(!fi->exists())
                {
                    emit message(QDateTime::currentDateTime(), "cannot find any configure files.\n");
                    delete fi;
                    return false;
                }
#ifndef Q_OS_WIN
            }
#endif
        }
    }
    else
    {
        fi = new QFileInfo(path);
    }
    if(!fi->exists())
    {
        emit message(QDateTime::currentDateTime(), "cannot find the configure file("+fi->fileName()+").\n");
        delete fi;
        return false;
    }
    if(fi->isReadable())
    {
        QFile f(fi->filePath());
        delete fi;
        f.open(QFile::ReadOnly);
        QJsonParseError jp_err;
        const QJsonDocument jd = QJsonDocument::fromJson(f.readAll(), &jp_err);
        f.close();
        if(jp_err.error == QJsonParseError::NoError)
        {
            const QJsonObject jo = jd.object();
            int version = 0;
            if(jo.contains("version")){
                version = jo["version"].toInt();
            }
            if(jo.contains("account"))
            {
                m_strAccount = jo["account"].toString();
            }
            if(jo.contains("password"))
            {
                m_strPassword = jo["password"].toString();
            }
            if(jo.contains("type")){
                if(version >= 2){
                    m_loginType = WebLgn::LoginType(jo["type"].toInt());
                }
                else{
                    m_loginType = WebLgn::LoginType(jo["type"].toInt()+1);
                }
            }
            if(jo.contains("ipv4"))
            {
                m_loginType = WebLgn::LoginType(m_loginType & ((jo["ipv4"].toInt() % 2) * int(WebLgn::IPv4)));
            }
            if(jo.contains("ipv6"))
            {
                m_loginType = WebLgn::LoginType(m_loginType & ((jo["ipv6"].toInt() % 2) * int(WebLgn::IPv6)));
            }
            if(jo.contains("autologin"))
            {
                m_bAutoLogin = jo["autologin"].toBool();
            }
            synchronizeAccount();
            if(m_bAutoLogin){
                start_monitor();
            }
            return true;
        }
        else{
            emit message(QDateTime::currentDateTime(), jp_err.errorString());
            if(g_bAppDebug)
            {
                emit debug_info(DebugTool::STATUS_FAIL, QString("Load account json:") + f.fileName(), true, true);
                emit debug_info(DebugTool::STATUS_FAIL, jp_err.errorString(), false, true);
            }
            return false;
        }
    }
    return false;
}

bool BjutNet::saveAccount(const QString path)
{
    QFile f(path);
    QDir d;
    if(path.length() == 0)
    {

        QString conf_file_home = (QDir::homePath() + "/.bjutnet/account.json");
        f.setFileName(conf_file_home);
        if(!f.exists())
        {
#ifndef Q_OS_WIN
            QString conf_file_etc = "/etc/bjutnet.d/account.json";
            f.setFileName(conf_file_etc);
            //etc没有写入权限
            if(!f.exists() || !f.open(QFile::WriteOnly) || !f.isWritable())
            {
#endif
                QString conf_file_model = (QDir::currentPath() + "/account.json");
                f.setFileName(conf_file_model);
                if(!f.exists() || !f.open(QFile::WriteOnly) || !f.isWritable())
                {
                    //不存在，则创建目录
                    d.setPath(QDir::homePath());
                    d.mkdir(".bjutnet");
                    f.setFileName(conf_file_home);
                }
#ifndef Q_OS_WIN
            }
#endif
        }
    }
    if(f.isOpen() || f.open(QFile::WriteOnly))//文件已打开或打开成功
    {
        QJsonObject jo;
        jo.insert("version", 2);
        jo.insert("account", m_strAccount);
        jo.insert("password", m_strPassword);
        jo.insert("type", int(m_loginType));
        jo.insert("autologin", m_bAutoLogin);
        QJsonDocument jd;
        jd.setObject(jo);
        QByteArray data= jd.toJson(QJsonDocument::Compact);
        f.write(data);
        f.close();
        if(g_bAppDebug)
        {
            emit debug_info(DebugTool::STATUS_SUCCESS, QString("Save account json:") + f.fileName(), true, true);
        }
        return true;
    }
    else if(g_bAppDebug)
    {
        emit debug_info(DebugTool::STATUS_FAIL, QString("Save account json:") + f.fileName(), true, true);
    }
    return false;
}

void BjutNet::synchronizeAccount()
{
    m_webLgn.setAccount(m_strAccount);
    m_webLgn.setPassword(m_strPassword);
    m_webLgn.setLoginType(m_loginType);
    m_webJfself.setAccount(m_strAccount);
    m_webJfself.setPassword(m_strPassword);
}

void BjutNet::checkLgn()
{
    static int sleepsec = 1;
    static QDateTime lastOfflined = QDateTime::currentDateTime();
    static QDateTime lastOnlined = QDateTime::currentDateTime();
    static int nOfflined = 0;
    bool suc = m_webLgn.checkLoginStatus();
    QDateTime time = QDateTime::currentDateTime();
    if (suc)
    {
        lastOnlined = time;
        //首次登录时或间隔一段时间输出信息
        if(m_tmCheckLgn.interval() != 1000*30)
            m_tmCheckLgn.start(1000*30);
    }
    else if(!m_webLgn.checkCampusNet())//非校园网
    {
        if(m_tmCheckLgn.interval() != 1000*30)
            m_tmCheckLgn.start(1000*30);
    }
    else//校园网未登录
    {
        if(lastOfflined < lastOnlined
                && lastOnlined.toSecsSinceEpoch() - lastOnlined.toSecsSinceEpoch() < 300)//5mins
        {
            ++nOfflined;
        }

        else if(nOfflined > 3)
        {
            nOfflined = 0;
            for(int i = 0; i< 3; ++i)
            {
                m_webJfself.refreshOnline();
                m_webJfself.toOfflineAll();
                sleep(1);
                m_webJfself.refreshAccount();
                if(!m_webJfself.gethasOnline())
                    break;
            }
        }
        else {
            nOfflined = 0;
        }
        lastOfflined = time;
        //登录
        suc = m_webLgn.login();
        time = QDateTime::currentDateTime();
        if (suc)
        {
            sleepsec = 1;
        }
        else
        {
            //this->sleep(sleepsec);
            if(sleepsec < 1024)//最大17分钟试一次
            {
                sleepsec *= 2;
            }
        }
        m_tmCheckLgn.start(sleepsec);
    }
}

void BjutNet::checkOnline()
{
    m_webJfself.refreshAccount();
    if(m_webJfself.refreshOnline())
    {
        if(m_tmCheckOnline.interval() != 1000*60*30)
            m_tmCheckOnline.start(1000*60*30);
    }
    else {
        if(m_tmCheckOnline.interval() != 1000*60)
            m_tmCheckOnline.start(1000*60);
    }
}

bool BjutNet::start_monitor()
{
    m_tmCheckLgn.start(100);//30sec
    m_tmCheckOnline.start(100);//30min
//    m_bRun = true;
//    if(!this->isRunning())
//    {    //启动线程
//        this->start(IdlePriority);
        connect(&m_netCfgMan, &QNetworkConfigurationManager::onlineStateChanged, &m_webLgn, &WebLgn::network_status_change);
//    }
    return true;
}

bool BjutNet::stop_monitor()
{
    m_tmCheckLgn.stop();
    m_tmCheckOnline.stop();
//    m_bRun = false;
//    if(this->isRunning())
//    {
//        //this->quit();
//        this->wait();
//        //this->terminate();
        disconnect(&m_netCfgMan, &QNetworkConfigurationManager::onlineStateChanged, &m_webLgn, &WebLgn::network_status_change);
//    }
    return true;
}


void BjutNet::run()
{
    for(int i = 0; i < 3 && !m_webJfself.checkLogin(); ++i)
        m_webJfself.login();
    if(m_webJfself.checkLogin())
    {
        m_webJfself.refreshAccount();
        m_webJfself.refreshOnline();
    }
    for(int i = 0; i < 3 && !m_webLgn.checkLoginStatus(); ++i)
        m_webLgn.login();
    m_webLgn.checkLoginStatus();
}
}
