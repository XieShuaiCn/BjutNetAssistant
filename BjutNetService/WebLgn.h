#ifndef WEBLGN_H
#define WEBLGN_H

#include "common.h"
#include "HttpClient.h"
#include <atomic>
#include <QTime>
#include <QNetworkConfigurationManager>

namespace bna{

class WebLgn : public QObject
{
    Q_OBJECT

public:
    //登录类型
    enum LoginType{
        AutoLoginType = 0, IPv4 = 1, IPv6 = 2, IPv4_6 = 3
    };
    enum NetType{
        UnknownNet = 0, BJUT_LAN = 1, BJUT_WIFI = 2, FREE_NET = 3
    };
    //构造函数
    WebLgn();
    //登录网关
    bool login();
    bool loginOnLAN(LoginType type = AutoLoginType);
    bool loginOnWIFI(LoginType type = AutoLoginType);
    //注销网关
    bool logout();
    bool logoutOnLAN(LoginType type = AutoLoginType);
    bool logoutOnWIFI(LoginType type = AutoLoginType);
    //检测网关状态
    bool checkLoginStatus(LoginType type = AutoLoginType);
    //检测网络是否接入校园网
    bool checkCampusNet();
    // 已用时间
    int getTime() const;
    // 已用流量
    int getFlow() const;
    // 剩余金额
    int getFee() const;
    // 获取账号名
    QString getAccount() const;
    // 设置账号
    void setAccount(const QString &account);
    // 获取密码
    QString getPassword() const;
    // 设置密码
    void setPassword(const QString &password);
    // 获取登录类型(IPv4,IPv6)
    LoginType getLoginType() const;
    // 设置登录类型(IPv4,IPv6,IPv46)
    void setLoginType(const LoginType type);
    void setLoginType(const int type);
    // 获取网络类型(WIFI,LAN)
    NetType getNetType() const;
    //获取登录状态，true：在线；false：离线
    bool getLoginStatus() const;

signals:
    //监视消息
    void message(const QDateTime& time, const QString& info);
    //更新状态（是否登录，已用时间（分钟），已用流量（千字节），剩余金额（分））
    void status_update(bool login, int time, int flow, int fee);
    // 调试信息
    void debug_info(DebugTool::DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);

public slots:
    // 网卡状态改变(是否联网)
    void network_status_change(bool online);

protected:
    // 转换登录返回代码为消息
    QString convertMsg(int msg, QString msga = "");

private:
    //账号信息
    QString m_strAccount;
    QString m_strPassword;
    LoginType m_loginType;
    NetType m_netType;
    HttpClient m_http;
    bool m_isOnline = false;
    int m_nTime;//分钟
    int m_nFlow;//KB
    int m_nFee;//分
};

/////////////////////// inline implementation /////////////////////////

inline int WebLgn::getTime() const
{
    return m_nTime;
}
inline int WebLgn::getFlow() const
{
    return m_nFlow;
}
inline int WebLgn::getFee() const
{
    return m_nFee;
}
inline QString WebLgn::getAccount() const
{
    return m_strAccount;
}
inline void WebLgn::setAccount(const QString &account)
{
    m_strAccount = account;
}
inline QString WebLgn::getPassword() const
{
    return m_strPassword;
}
inline void WebLgn::setPassword(const QString &password)
{
    m_strPassword = password;
}
inline WebLgn::LoginType WebLgn::getLoginType() const
{
    return m_loginType;
}
inline void WebLgn::setLoginType(const WebLgn::LoginType type)
{
    m_loginType = type;
}
inline void WebLgn::setLoginType(const int type)
{
    if(type > 0 && type <= 3)
        m_loginType = LoginType(type);
}
inline WebLgn::NetType WebLgn::getNetType() const
{
    return m_netType;
}
//获取登录状态，true：在线；false：离线
inline bool WebLgn::getLoginStatus() const
{
    return m_isOnline;
}

}
#endif // BJUTNET_H
