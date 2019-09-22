#ifndef BJUTNET_H
#define BJUTNET_H

#include "common.h"
#include "WebLgn.h"
#include "WebJfself.h"
#include <QThread>
#include <QTimer>

namespace bna{
namespace core {

class BjutNet : public QThread
{
    Q_OBJECT
public:
    BjutNet();
    //加载账号信息
    bool loadAccount(const QString path = QString());
    //保存账号信息
    bool saveAccount(const QString path = QString());
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
signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
    // 调试信息
    void debug_info(DebugTool::DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);
protected:
    void run();
private:
    QNetworkConfigurationManager m_netCfgMan;
    std::atomic_bool m_bRun;
    QTimer m_tmCheckLgn;
    QTimer m_tmCheckOnline;
    //用户配置信息
    QString m_strAccount;
    QString m_strPassword;
    WebLgn::LoginType m_loginType = WebLgn::AutoLoginType;
    bool m_bAutoLogin = true;
    WebLgn m_webLgn;
    WebJfself m_webJfself;
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
}

inline WebLgn::LoginType BjutNet::getLoginType() const
{
    return m_loginType;
}

inline void BjutNet::setLoginType(const WebLgn::LoginType type)
{
    m_loginType = type;
    m_webLgn.setLoginType(type);
}

inline void BjutNet::setLoginType(const int type)
{
    if(type > 0 && type <= 3)
    {
        m_loginType = WebLgn::LoginType(type);
        m_webLgn.setLoginType(m_loginType);
    }
}

inline bool BjutNet::getAutoLogin()
{
    return m_bAutoLogin;
}

inline void BjutNet::setAutoLogin(bool autologin/* = true*/)
{
    m_bAutoLogin = autologin;
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
