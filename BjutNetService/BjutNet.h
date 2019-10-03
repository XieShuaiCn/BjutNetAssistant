#ifndef BJUTNET_H
#define BJUTNET_H

#include "common.h"
#include "WebLgn.h"
#include "WebJfself.h"
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QMutex>

namespace bna{
namespace core {

class BjutNet : public QObject
{
    Q_OBJECT
public:
    BjutNet();
    ~BjutNet();
    //加载账号信息
    bool loadAccount();
    //保存账号信息
    bool saveAccount();
    //同步账号信息
    void synchronizeAccount();
    //检查登录状态
    void checkLgn();
    //检查在线列表
    void checkOnline();
    //启动监视器
    bool start_monitor();
    //停止监视器
    bool stop_monitor();
    //获取账号
    QString getAccount() const;
    //设置账号
    void setAccount(const QString &account);

    QString getPassword() const;

    void setPassword(const QString &password);

    WebLgn::LoginType getLoginType() const;

    void setLoginType(const WebLgn::LoginType type);

    void setLoginType(const int type);

    bool getAutoLogin();

    void setAutoLogin(bool autologin = true);

    WebLgn &getWebLgn();

    WebJfself &getWebJfself();

    // restart timer,(stop and then start)
    int restartTimer(int &id, int interval, Qt::TimerType timerType = Qt::CoarseTimer);

signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
    // 调试信息
    void debug_info(DebugTool::DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);

protected slots:
    void online_status_changed(const QVariant &info);
    void login_status_changed(bool login, int time, int flow, int fee);

protected:
    //
    void timerEvent(QTimerEvent *event);

private:
    QNetworkConfigurationManager m_netCfgMan;
    int m_nTimerCheckLgn;
    int m_nTimerCheckLgnInterval;
    QDateTime m_dtLastCheckLgn;
    QMutex m_mtxCheckLgn;
    int m_nTimerCheckOnline;
    int m_nTimerCheckOnlineInterval;
    QDateTime m_dtLastCheckOnline;
    QMutex m_mtxCheckOnline;
    //用户配置信息
    QString m_strAccount;
    QString m_strPassword;
    WebLgn::LoginType m_loginType = WebLgn::AutoLoginType;
    bool m_bAutoLogin = true;
    WebLgn m_webLgn;
    WebJfself m_webJfself;
    bool m_bNeedSaveAccount = false;
};

//////////////////////////// inline implementation ////////////////

//获取账号
inline QString BjutNet::getAccount() const
{
    return m_strAccount;
}

inline void BjutNet::setAccount(const QString &account)
{
    m_strAccount = account;
    m_webLgn.setAccount(account);
    m_webJfself.setAccount(account);
    m_bNeedSaveAccount = true;
}

inline QString BjutNet::getPassword() const
{
    return m_strPassword;
}

inline void BjutNet::setPassword(const QString &password)
{
    m_strPassword = password;
    m_webLgn.setPassword(password);
    m_webJfself.setPassword(password);
    m_bNeedSaveAccount = true;
}

inline WebLgn::LoginType BjutNet::getLoginType() const
{
    return m_loginType;
}

inline void BjutNet::setLoginType(const WebLgn::LoginType type)
{
    m_loginType = type;
    m_webLgn.setLoginType(type);
    m_bNeedSaveAccount = true;
}

inline void BjutNet::setLoginType(const int type)
{
    if(type > 0 && type <= 3)
    {
        m_loginType = WebLgn::LoginType(type);
        m_webLgn.setLoginType(m_loginType);
        m_bNeedSaveAccount = true;
    }
}

inline bool BjutNet::getAutoLogin()
{
    return m_bAutoLogin;
}

inline void BjutNet::setAutoLogin(bool autologin/* = true*/)
{
    m_bAutoLogin = autologin;
    m_bNeedSaveAccount = true;
}

inline WebLgn &BjutNet::getWebLgn()
{
    return m_webLgn;
}

inline WebJfself &BjutNet::getWebJfself()
{
    return m_webJfself;
}

}}
#endif // BJUTNET_H
