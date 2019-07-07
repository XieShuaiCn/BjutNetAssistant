#ifndef BNC_INTERACTION_H
#define BNC_INTERACTION_H

#include "ServiceBridge.h"

namespace bna{

class InterAction{
public:
    bool SetHost(const std::string &host);

    void ShowVersion();

    void ShowMenu();

    bool Process();

    bool Connected();

    bool ShowStatus();

    bool RefreshNet();

    bool LoginBjut();

    bool LogoutBjut();

    bool ShowOnline();

    bool OfflineDevice();

    bool ReloadAccount();

    bool SetNewAccount();

    bool ShowCurrentService();

    bool BookService();

    static void InputPasswd(std::string &passwd, char echo);

private:
    ServiceBridge m_service;
};
}
#endif // BNC_INTERACTION_H
