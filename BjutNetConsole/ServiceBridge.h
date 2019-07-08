#ifndef BNC_SERVICEBRIDGE_H
#define BNC_SERVICEBRIDGE_H

#include <string>
#include <array>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "MessageValue.h"

namespace bna{

class UdpSocket;

class ServiceBridge
{
public:
    ServiceBridge();

    bool setHost(const std::string &host);
    const std::string getHost();
    void getMyAddress(std::vector<std::string> &ip);

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

    bool sendGetVersion(std::string &ver, int &inner_ver);
    bool sendGetAccount(std::string &name, std::string &password, int &type);
    bool sendGetUsedFlow(int &value);
    bool sendGetUsedTime(int &value);
    bool sendGetLeftFee(int &value);
    bool sendGetAllFlow(int &value);
    bool sendGetOnlineDevices(std::vector<std::array<std::string, 4>> &devices);
    bool sendGetFlowService(std::string &name, int &totalFlow);
    bool sendGetBookedService(std::string &name);
    bool sendGetAllServices(std::vector<std::tuple<int, std::string, std::string>> &services);

    bool sendSetAccount(const std::string name, const std::string passwd, int type);
    bool sendSetBookedService(int id);
    bool sendSetOfflineDevice(int id);
    bool sendSetAutoStart(bool autorun);

    bool parseJson(const std::string &json, int seed, boost::property_tree::ptree &tree);
    bool parseJsonAndVarify(const std::string &json, int seed);

    const std::string &getLastError();

private:
    bool sendAct_common(MessageValue::ActionAct type);

    UdpSocket *m_socket;
    int m_nMsgVersion;
    std::string m_strLastError;
};

inline const std::string &ServiceBridge::getLastError()
{
    return m_strLastError;
}

inline bool ServiceBridge::sendActLoadAccount()
{
    return sendAct_common(MessageValue::ACT_LOAD_ACCOUNT);
}
inline bool ServiceBridge::sendActSaveAccount()
{
    return sendAct_common(MessageValue::ACT_SAVE_ACCOUNT);
}

inline bool ServiceBridge::sendActLoginBjut()
{
    return sendAct_common(MessageValue::ACT_LOGIN_BJUT);
}
inline bool ServiceBridge::sendActLogoutBjut()
{
    return sendAct_common(MessageValue::ACT_LOGOUT_BJUT);
}
inline bool ServiceBridge::sendActLoginJfself()
{
    return sendAct_common(MessageValue::ACT_LOGIN_JFSELF);
}
inline bool ServiceBridge::sendActLogoutJfself()
{
    return sendAct_common(MessageValue::ACT_LOGOUT_JFSELF);
}
inline bool ServiceBridge::sendActRefreshNet()
{
    return sendAct_common(MessageValue::ACT_REFRESH_NET);
}
inline bool ServiceBridge::sendActRefreshJfselfAccount()
{
    return sendAct_common(MessageValue::ACT_REFRESH_JFSELF_ACCOUNT);
}
inline bool ServiceBridge::sendActRefreshOnline()
{
    return sendAct_common(MessageValue::ACT_REFRESH_ONLINE);
}

inline bool ServiceBridge::sendActEnterDebugMode()
{
    return sendAct_common(MessageValue::ACT_ENTER_DEBUG_MODE);
}

inline bool ServiceBridge::sendActLeaveDebugMode()
{
    return sendAct_common(MessageValue::ACT_LEAVE_DEBUG_MODE);
}

}

#endif // BNC_SERVICEBRIDGE_H
