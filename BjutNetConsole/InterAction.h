#ifndef BNC_INTERACTION_H
#define BNC_INTERACTION_H

#include "ServiceBridge.h"

namespace bna{

class InterAction{
public:

    void ShowMenu();

    bool Process();

    bool Connected();

    bool ShowVersion();

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

    bool SetAutoMenu();

    bool SetAutoStart();

    bool EnterDebugMode();

    bool LeaveDebugMode();

    bool SetAnotherHost();

    bool ShowMyAddress();

    bool SetHost(const std::string &host);

private:
    bool m_bShowMenu = true;
    bool m_bVerifyMsgVer = false;
    ServiceBridge m_service;
};
}
#endif // BNC_INTERACTION_H
