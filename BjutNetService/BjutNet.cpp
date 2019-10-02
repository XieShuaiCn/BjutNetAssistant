#include "BjutNet.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QJsonObject>
#include <QJsonDocument>

namespace bna{
namespace core{

BjutNet::BjutNet():
    m_nTimerCheckLgn(-1), m_nTimerCheckOnline(-1)
{
    connect(&m_webLgn, &WebLgn::message, this, &BjutNet::message);
    connect(&m_webJfself, &WebJfself::message, this, &BjutNet::message);
    connect(this, &BjutNet::debug_info, DebugTool::getPointer(), &DebugTool::writeString);
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

#define RESTART_TIMER_LGN   restartTimer(m_nTimerCheckLgn, m_nTimerCheckLgnInterval, Qt::VeryCoarseTimer)

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
        if(m_nTimerCheckLgnInterval != 1000*30){//30s
            m_nTimerCheckLgnInterval = (1000*30);
            RESTART_TIMER_LGN;
        }
    }
    else if(!m_webLgn.checkCampusNet())//非校园网
    {
        if(m_nTimerCheckLgnInterval != 1000*30){//30s
            m_nTimerCheckLgnInterval = 1000*30;
            RESTART_TIMER_LGN;
        }
    }
    else//校园网未登录
    {
        if(lastOfflined < lastOnlined
                && lastOnlined.toSecsSinceEpoch() - lastOnlined.toSecsSinceEpoch() < 300)//5mins
        {
            ++nOfflined;
#ifdef BUILD_DEVELOP
            message(QDateTime::currentDateTime(), QString("offline in short time. %1").arg(nOfflined));
#endif
        }

        else if(nOfflined > 3)
        {
            nOfflined = 0;
            for(int i = 0; i< 5; ++i)
            {
                m_webJfself.refreshOnline();
                m_webJfself.toOfflineAll();
#ifdef BUILD_DEVELOP
                message(QDateTime::currentDateTime(), QString("CheckLgn: offline all."));
#endif
                QElapsedTimer t;
                t.start();
                while(t.elapsed()<1000){// wait a second
                    QCoreApplication::processEvents(QEventLoop::DialogExec);
                }
                m_webJfself.refreshAccount();
                if(!m_webJfself.gethasOnline()){
#ifdef BUILD_DEVELOP
                    message(QDateTime::currentDateTime(), QString("CheckLgn: no online."));
#endif
                    break;
                }
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
        m_nTimerCheckLgnInterval = (sleepsec * 1000);
        RESTART_TIMER_LGN;
    }
}
#undef RESTART_TIMER_LGN
#define RESTART_TIMER_ONLINE restartTimer(m_nTimerCheckOnline, m_nTimerCheckOnlineInterval, Qt::VeryCoarseTimer)

void BjutNet::checkOnline()
{
    m_webJfself.refreshAccount();
    if(m_webJfself.refreshOnline())
    {
        int interval = std::min(1000*60*30, m_nTimerCheckOnlineInterval*2);//max 30min
        if(m_nTimerCheckOnlineInterval != interval){
            m_nTimerCheckOnlineInterval = interval;
            RESTART_TIMER_ONLINE;
        }
    }
    else {
        if(m_nTimerCheckOnlineInterval != 1000*30) {//30s
            m_nTimerCheckOnlineInterval = 1000*30;
            RESTART_TIMER_ONLINE;
        }
    }
}
#undef RESTART_TIMER_ONLINE

bool BjutNet::start_monitor()
{
    // timer
    m_nTimerCheckLgnInterval = 100;
    m_nTimerCheckOnlineInterval = 100;
    restartTimer(m_nTimerCheckOnline, m_nTimerCheckOnlineInterval, Qt::VeryCoarseTimer);
    restartTimer(m_nTimerCheckLgn, m_nTimerCheckLgnInterval, Qt::VeryCoarseTimer);
    // The moniter of physical net line
    connect(&m_netCfgMan, &QNetworkConfigurationManager::onlineStateChanged, &m_webLgn, &WebLgn::network_status_change);
    return true;
}

bool BjutNet::stop_monitor()
{
    disconnect(&m_netCfgMan, &QNetworkConfigurationManager::onlineStateChanged, &m_webLgn, &WebLgn::network_status_change);
    if(m_nTimerCheckLgn>0){ killTimer(m_nTimerCheckLgn);}
    if(m_nTimerCheckOnline>0){ killTimer(m_nTimerCheckOnline);}
    return true;
}

int BjutNet::restartTimer(int &id, int interval, Qt::TimerType timerType)
{
    if(id > 0){
        killTimer(id);
    }
    id = startTimer(interval, timerType);
    return id;
}

void BjutNet::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerCheckLgn){
        checkLgn();
    }
    else if (event->timerId() == m_nTimerCheckOnline){
        checkOnline();
    }
}

}
}
