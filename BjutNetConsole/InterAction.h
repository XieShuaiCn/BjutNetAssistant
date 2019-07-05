#ifndef BNC_INTERACTION_H
#define BNC_INTERACTION_H

#include "ServiceBridge.h"

namespace bna{

class InterAction{
public:
    void SetHost(const std::string &host);

    void ShowVersion();

    void ShowMenu();

    bool Process();

    bool Connected();

    void ShowStatus();

    void RefreshNet();

    void LoginBjut();

    void LogoutBjut();

    void ShowOnline();

    void ReloadAccount();

    void SetNewAccount();

    void BookService();

    static void InputPasswd(std::string &passwd, char echo);

private:
    ServiceBridge m_service;
};
}
#endif // BNC_INTERACTION_H
