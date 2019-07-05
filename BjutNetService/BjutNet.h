#ifndef BJUTNET_H
#define BJUTNET_H

#include "common.h"
#include "WebLgn.h"
#include "WebJfself.h"
#include <QThread>
#include <QTimer>

namespace bna{

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
    QString getAccount() const
    {
        return m_strAccount;
    }
    void setAccount(const QString &account)
    {
        m_strAccount = account;
        m_webLgn.setAccount(account);
        m_webJfself.setAccount(account);
    }
    QString getPassword() const
    {
        return m_strPassword;
    }
    void setPassword(const QString &password)
    {
        m_strPassword = password;
        m_webLgn.setPassword(password);
        m_webJfself.setPassword(password);
    }
    WebLgn::LoginType getLoginType() const
    {
        return m_loginType;
    }
    void setLoginType(const WebLgn::LoginType type)
    {
        m_loginType = type;
        m_webLgn.setLoginType(type);
    }
    void setLoginType(const int type)
    {
        if(type > 0 && type <= 3)
        {
            m_loginType = WebLgn::LoginType(type);
            m_webLgn.setLoginType(m_loginType);
        }
    }
    WebLgn &getWebLgn()
    {
        return m_webLgn;
    }
    WebJfself &getWebJfself()
    {
        return m_webJfself;
    }
signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
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
    WebLgn::LoginType m_loginType;
    WebLgn m_webLgn;
    WebJfself m_webJfself;
};
}
#endif // BJUTNET_H
