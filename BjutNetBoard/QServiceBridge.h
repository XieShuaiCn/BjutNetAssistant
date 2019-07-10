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


namespace bna{

class QServiceBridge : QObject
{
    Q_OBJECT
public:
    QServiceBridge();
    bool setHost(const QString &host);
    const QString getHost();
    void getMyAddress(std::vector<QString> &ip);

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
    bool sendGetUsedFlow(int &value);
    bool sendGetUsedTime(int &value);
    bool sendGetLeftFee(int &value);
    bool sendGetAllFlow(int &value);
    bool sendGetOnlineDevices(std::vector<std::array<QString, 4>> &devices);
    bool sendGetFlowService(std::string &name, int &totalFlow);
    bool sendGetBookedService(std::string &name);
    bool sendGetAllServices(std::vector<std::tuple<int, QString, QString>> &services);
    bool sendGetAutoStart(bool &autorun);

    bool sendSetAccount(const QString name, const QString passwd, int type);
    bool sendSetBookedService(int id);
    bool sendSetOfflineDevice(int id);
    bool sendSetAutoStart(bool autorun);

    bool parseJson(const QString &json, int seed, QJsonValue &data);
    bool parseJsonAndVarify(const QString &json, int seed);

    const QString &getLastError();

private:
    bool doSendAndReceive(const QString &sdata, QString &rdata);
    bool sendAct_common(MessageValue::ActionAct type);

    QUdpSocket m_socket;
    QHostAddress m_host;
    quint16 m_port;
    int m_nMsgVersion;
    QString m_strLastError;
};

inline const std::string &QServiceBridge::getLastError()
{
    return m_strLastError;
}

inline bool QServiceBridge::sendActLoadAccount()
{
    return sendAct_common(MessageValue::ACT_LOAD_ACCOUNT);
}
inline bool QServiceBridge::sendActSaveAccount()
{
    return sendAct_common(MessageValue::ACT_SAVE_ACCOUNT);
}

inline bool QServiceBridge::sendActLoginBjut()
{
    return sendAct_common(MessageValue::ACT_LOGIN_BJUT);
}
inline bool QServiceBridge::sendActLogoutBjut()
{
    return sendAct_common(MessageValue::ACT_LOGOUT_BJUT);
}
inline bool QServiceBridge::sendActLoginJfself()
{
    return sendAct_common(MessageValue::ACT_LOGIN_JFSELF);
}
inline bool QServiceBridge::sendActLogoutJfself()
{
    return sendAct_common(MessageValue::ACT_LOGOUT_JFSELF);
}
inline bool QServiceBridge::sendActRefreshNet()
{
    return sendAct_common(MessageValue::ACT_REFRESH_NET);
}
inline bool QServiceBridge::sendActRefreshJfselfAccount()
{
    return sendAct_common(MessageValue::ACT_REFRESH_JFSELF_ACCOUNT);
}
inline bool QServiceBridge::sendActRefreshOnline()
{
    return sendAct_common(MessageValue::ACT_REFRESH_ONLINE);
}

inline bool QServiceBridge::sendActEnterDebugMode()
{
    return sendAct_common(MessageValue::ACT_ENTER_DEBUG_MODE);
}

inline bool QServiceBridge::sendActLeaveDebugMode()
{
    return sendAct_common(MessageValue::ACT_LEAVE_DEBUG_MODE);
}

}
#endif // SERVICEBRIDGE_H
