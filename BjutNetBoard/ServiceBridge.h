#ifndef SERVICEBRIDGE_H
#define SERVICEBRIDGE_H

#include <vector>
#include <tuple>
#include <array>
#include <QString>
#include <QJsonValue>
#include <QHostAddress>
#include <QUdpSocket>
#include "../BjutNetService/MessageValue.h"

namespace bna {

namespace gui {

class ServiceBridge : public QObject
{
    Q_OBJECT
public:
    ServiceBridge();
    bool setHost(const QString &host);
    bool setHost(QHostAddress::SpecialAddress host);
    const QString getHost();
    void getMyAddress(QVector<QHostAddress> &addrs);
    void setAuth(bool needed = true);
    void setAuth(bool needed, bool use_token, const QString &token=QString());
    void setAuth(bool needed, const QString &name, const QString &passwd);

    bool sendSYN();
    bool sendENQ();

    bool sendSyncHello();

    bool sendActLoadAccount();
    bool sendActSaveAccount();
    bool sendActLoginBjut();
    bool sendActLogoutBjut();
    bool sendActLoginJfself();
    bool sendActLogoutJfself();
    bool sendActRefreshNet();
    bool sendActRefreshJfselfAccount();
    bool sendActRefreshOnline();
    bool sendActEnterDebugMode();
    bool sendActLeaveDebugMode();

    bool sendGetVersion(QString &ver, int &inner_ver);
    bool sendGetAccount(QString &name, QString &password, int &type);
    bool sendGetLoginStatus(bool &login);
    bool sendGetUsedFlow(int &value);
    bool sendGetUsedTime(int &value);
    bool sendGetLeftFee(int &value);
    bool sendGetAllFlow(int &value);
    bool sendGetOnlineDevices(std::vector<std::array<QString, 4>> &devices);
    bool sendGetFlowService(QString &name, int &totalFlow);
    bool sendGetBookedService(QString &name);
    bool sendGetAllServices(std::vector<std::tuple<int, QString, QString>> &services);
    bool sendGetAutoStart(bool &autorun);

    bool sendSetAccount(const QString name, const QString passwd, int type);
    bool sendSetBookedService(int id);
    bool sendSetOfflineDevice(int id);
    bool sendSetAutoStart(bool autorun);

    bool parseJson(const QString &json, int seed, QJsonValue &data);
    bool parseJsonAndVarify(const QString &json, int seed);

    const QString &getLastError() const;

private:
    bool doSendAndReceive(const QString &sdata, QString &rdata);
    bool sendAct_common(bna::MessageValue::ActionAct type);

    QUdpSocket m_socket;
    QHostAddress m_host;
    quint16 m_port;
    int m_nMsgVersion;
    bool m_bNeedVarify;
    bool m_bUseToken;
    QString m_strToken;
    QString m_strName;
    QString m_strPasswd;
    QString m_strLastError;
};

inline const QString &ServiceBridge::getLastError() const
{
    return m_strLastError;
}

inline bool ServiceBridge::sendActLoadAccount()
{
    return sendAct_common(bna::MessageValue::ACT_LOAD_ACCOUNT);
}
inline bool ServiceBridge::sendActSaveAccount()
{
    return sendAct_common(bna::MessageValue::ACT_SAVE_ACCOUNT);
}

inline bool ServiceBridge::sendActLoginBjut()
{
    return sendAct_common(bna::MessageValue::ACT_LOGIN_BJUT);
}
inline bool ServiceBridge::sendActLogoutBjut()
{
    return sendAct_common(bna::MessageValue::ACT_LOGOUT_BJUT);
}
inline bool ServiceBridge::sendActLoginJfself()
{
    return sendAct_common(bna::MessageValue::ACT_LOGIN_JFSELF);
}
inline bool ServiceBridge::sendActLogoutJfself()
{
    return sendAct_common(bna::MessageValue::ACT_LOGOUT_JFSELF);
}
inline bool ServiceBridge::sendActRefreshNet()
{
    return sendAct_common(bna::MessageValue::ACT_REFRESH_NET);
}
inline bool ServiceBridge::sendActRefreshJfselfAccount()
{
    return sendAct_common(bna::MessageValue::ACT_REFRESH_JFSELF_ACCOUNT);
}
inline bool ServiceBridge::sendActRefreshOnline()
{
    return sendAct_common(bna::MessageValue::ACT_REFRESH_ONLINE);
}

inline bool ServiceBridge::sendActEnterDebugMode()
{
    return sendAct_common(bna::MessageValue::ACT_ENTER_DEBUG_MODE);
}

inline bool ServiceBridge::sendActLeaveDebugMode()
{
    return sendAct_common(bna::MessageValue::ACT_LEAVE_DEBUG_MODE);
}
}}
#endif // SERVICEBRIDGE_H
