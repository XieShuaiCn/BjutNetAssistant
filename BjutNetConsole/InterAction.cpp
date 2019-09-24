#include "InterAction.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "Version.h"
#include "Utility.h"

using namespace std;
using namespace bna::core;

namespace bna{

void InterAction::ShowMenu()
{
    cout << endl;
    cout << "  =========================================================" << endl;
    cout << "                       Menu                                " << endl;
    cout << "  =========================================================" << endl;
    cout << "  command |        description                             " << endl;
    cout << "  ---------------------------------------------------------" << endl;
    cout << "    m     |  Print menu.(\"atm\" for auto print)           " << endl;
    cout << "    q     |  Quit this session.                            " << endl;
    cout << "    e     |  Exit local demon service.                     " << endl;
    cout << "    r     |  Refresh all status.                           " << endl;
    cout << "    s     |  Show the status of login.                     " << endl;
    cout << "    v     |  Version information.                          " << endl;
    cout << "    li    |  Login BJUT net now.                           " << endl;
    cout << "    lo    |  Logout BJUT net now.                          " << endl;
    cout << "    ol    |  List online devices.                          " << endl;
    cout << "    onl   |  List online devices.                          " << endl;
    cout << "    ofl   |  Offline devices.                              " << endl;
    cout << "    lda   |  Reload account profile.                       " << endl;
    cout << "    sna   |  Set new account profile.                      " << endl;
    cout << "    csv   |  Show the current service                      " << endl;
    cout << "    bsv   |  Book the service of next month.               " << endl;
    cout << "    atm   |  The switch of print menu after a operation.   " << endl;
    cout << "    ats   |  The switch of start service when power on.    " << endl;
    cout << "    addr  |  Show my ip address.                           " << endl;
    cout << "  =========================================================" << endl;
#if defined(_DEBUG) || defined(BUILD_DEVELOP)
    cout << "    enter_debug     Enter debug mode.                      " << endl;
    cout << "    leave_debug     Leave debug mode.                      " << endl;
    cout << "    set_host        Set host address.                      " << endl;
    cout << "    my_host         Show host address.                     " << endl;
    cout << "  =========================================================" << endl;
#endif
}

bool InterAction::Process()
{
    try{
        string input;
        if(m_bShowMenu){
            ShowMenu();
            cout << "Input your command: ";
        }
        else{
            cout << endl << "Input your command('m' for menu): ";
        }
        cout.flush();
        cin >> input;
        auto n_cmd_s = input.find_first_not_of(" \t\r\n");
        auto n_cmd_e = input.find_last_not_of(" \t\r\n");
        n_cmd_s = n_cmd_s==string::npos ? 0 : n_cmd_s;
        n_cmd_e = n_cmd_e==string::npos ? input.size() : n_cmd_e+1;
        string cmd = input.substr(n_cmd_s, n_cmd_e - n_cmd_s);
        bool processed = true;
        cout << endl;
        if(cmd.size() == 1){
            switch (cmd[0]) {
            case 'm':
                if(!m_bShowMenu) ShowMenu();
                break;
            case 'q':
                return false;
            case 's':
                ShowStatus();
                break;
            case 'r':
                RefreshNet();
                break;
            case 'v':
                ShowVersion();
                break;
            default:
                processed = false;
                break;
            }
        }
        else if(cmd.size() == 2){
            if(cmd == "li"){
                LoginBjut();
            }
            else if(cmd == "lo"){
                LogoutBjut();
            }
            else if(cmd == "ol"){
                ShowOnline();
            }
            else{
                processed = false;
            }
        }
        else if(cmd.size() == 3){
            if(cmd == "onl") {
                ShowOnline();
            }
            else if(cmd == "ofl") {
                OfflineDevice();
            }
            else if(cmd == "lda"){
                ReloadAccount();
            }
            else if(cmd == "sna"){
                SetNewAccount();
            }
            else if(cmd == "csv"){
                ShowCurrentService();
            }
            else if(cmd == "bsv") {
                BookService();
            }
            else if(cmd == "atm") {
                AutoShowMenu();
            }
            else if(cmd == "ats") {
                SetAutoStart();
            }
            else{
                processed = false;
            }
        }
        else if(cmd.size() == 4){
            if(cmd == "addr") {
                ShowMyAddress();
            }
            else{
                processed = false;
            }
        }
        else if(cmd == "report_debug"){}
#if defined(_DEBUG) || defined(BUILD_DEVELOP)
        else if(cmd == "set_host"){
            SetAnotherHost();
        }
        else if(cmd == "my_host"){
            cout << endl << " " << m_service.getHost() << endl;
        }
        else if(cmd == "enter_debug"){
            EnterDebugMode();
        }
        else if(cmd == "leave_debug"){
            LeaveDebugMode();
        }
#endif
        else{
            processed = false;
        }
        if(!processed){
            cout << "Unrecognized command: " << cmd << endl;
        }
    }
    catch(...){
        cout << "Something wrong occured. " << endl;
    }
    return true;
}

bool InterAction::Connected()
{
    if(m_service.sendSYN()){
        if(m_bVerifyMsgVer || m_service.sendENQ()){
            m_bVerifyMsgVer = true;
            return true;
        }
        else{
            cout << "The version of <" BNA_NAME "> is different from me. Please upgrade the lower one." << endl;
        }
    }
    else{
        cout << "Can not connect to <" BNA_NAME ">." << endl;
    }
    return false;
}

bool InterAction::AutoShowMenu()
{
    cout << " Do you want to print menu automatically? (y/n) ";
    string input;
    cin >> input;
    if(input == "y" || input == "yes"){
        m_bShowMenu = true;
        cout << " Will print menu automatically." << endl;
    }
    else if(input == "n" || input == "no"){
        m_bShowMenu = false;
        cout << " Will not print menu automatically. type 'm' for print menu once." << endl;
    }
    else{
        cout << " Bad input." << endl;
    }
    return true;
}

bool InterAction::ShowStatus()
{
    if(Connected()){
        int allFlow = 0;
        int flow = 0;
        int time = 0;
        int fee = 0;
        string serviceName;

        m_service.sendGetUsedFlow(flow);
        m_service.sendGetUsedTime(time);
        m_service.sendGetLeftFee(fee);

        string timeUnit[] = {"min", "h"};
        string flowUnit[] = {"KB", "MB", "GB", "TB"};
        string feeUnit[] = {"Yuan"};
        int timeUnitIdx = 0;
        int flowUnitIdx = 0;
        int feeUnitIdx = 0;
        float fflow = flow;
        while(fflow > 1024)
        {
            fflow /= 1024;
            ++flowUnitIdx;
            if(flowUnitIdx >= 3) break;
        }
        float ftime = time;
        while(ftime > 60)
        {
            ftime /= 60;
            ++timeUnitIdx;
            if(timeUnitIdx >= 1) break;
        }
        cout << endl
             << " Used flow: " << fixed << setprecision(3) << fflow << flowUnit[flowUnitIdx]
             << "   | Used time: " << fixed << setprecision(2) << ftime << timeUnit[timeUnitIdx]
             << "   | Left fee: " << fixed << setprecision(2) << static_cast<double>(fee) / 100 << feeUnit[feeUnitIdx]
             << endl;

        m_service.sendGetFlowService(serviceName, allFlow);
        cout << " Service: " << serviceName
             << "   | All flow: " << static_cast<double>(allFlow) / 1024 << flowUnit[2];
        if(allFlow > 0){
            cout << "   | Used: " << fixed << setprecision(2) << (100.0 * flow / allFlow / 1024) << "%";
        }
        else{
            cout << "   | Used: -- %";
        }
        cout << endl;
        return true;
    }
    return false;
}

bool InterAction::LoginBjut()
{
    if(Connected()){
        if(m_service.sendActLoginBjut()){
            cout << " OK." << endl;
            return true;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << " Fail." << endl;
        }
    }
    return false;
}

bool InterAction::LogoutBjut()
{
    if(Connected()){
        if(m_service.sendActLogoutBjut()){
            cout << " OK." << endl;
            return true;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << " Fail." << endl;
        }
    }
    return false;
}

bool InterAction::ShowOnline()
{
    if(Connected()){
        if(!m_service.sendActRefreshOnline()){
            cout << m_service.getLastError() << endl;
            cout << " Fail to refresh the list of online devices." << endl;
            return false;
        }
        vector<array<string, 4>> devices;
        if(m_service.sendGetOnlineDevices(devices))
        {
            if(devices.size()){
                cout << "   ID  |        IPv4        |         IPv6" << endl;
                for(const auto &dev : devices){
                    cout << "  " << setw(9) << std::left << dev[0] << setw(21) << std::left << dev[1]
                         << setw(45) << std::left << dev[2] << endl;
                }
                return true;
            }
            else{
                cout << "No devices found." << endl;
            }
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << "Fail to list online devices." << endl;
        }
    }
    return false;
}

bool InterAction::OfflineDevice()
{
    if(ShowOnline()){
        int id;
        cout << "Input the ID of device which you want to offline: ";
        string data;
        cin >> data;
        try{
            id = std::stoi(data);
        }catch(...){
            cout << "Invalid number." << endl;
            return false;
        }
        if(m_service.sendSetOfflineDevice(id))
        {
            cout << "Success to offline device:" << id << endl;
            return true;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << "Fail to offline device:" << id << endl;
        }
    }
    return false;
}

bool InterAction::RefreshNet()
{
    if(Connected()){
        if(m_service.sendActRefreshNet()){
           cout << " Have refreshed the status of net." << endl;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to refresh the status of net." << endl;
        }
        if(m_service.sendActRefreshJfselfAccount()){
           cout << " Have refreshed the infomation of account." << endl;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to refresh the infomation of account." << endl;
        }
        if(m_service.sendActRefreshOnline()){
           cout << " Have refreshed the list of online devices." << endl;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to refresh the list of online devices." << endl;
        }
        return true;
    }
    return false;
}

bool InterAction::ReloadAccount()
{
    if(Connected()){
        if(m_service.sendActLoadAccount()){
           cout << " Have reloaded account profile." << endl;
           return true;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to reload account profile." << endl;
        }
    }
    return false;
}

bool InterAction::SetNewAccount()
{
    if(Connected()){
        string name;
        string passwd;
        int type = 0;
        do{
            cout << " Input your account name: ";
            cin >> name;
        }while(name.size()==0);
        do{
            ConsoleInputPasswd(" Input your password: ", passwd, '*');
        }while(passwd.size()==0);
        do{
            cout << " Login type:" << endl
                 << "     1  IPv4" << endl
                 << "     2  IPv6" << endl
                 << "     3  IPv4 & IPv6" << endl;
            cout << " Choose the type of logging(1-3): ";
            string data;
            cin >> data;
            try{
                type = std::stoi(data);
            }catch(...){
                cout << "Invalid number." << endl;
                continue;
            }
        }while(type< 1 || type > 3);
        if(m_service.sendSetAccount(name, passwd, type)){
            cout << " Success to set new account profile." << endl;
            return true;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to set new account profile." << endl;
        }
    }
    return false;
}

bool InterAction::ShowCurrentService()
{
    if(Connected()){
        string service;
        int allflow;
        if(!m_service.sendGetFlowService(service, allflow)){
            cout << m_service.getLastError() << endl;
            cout << " Fail to get service." << endl;
            return false;
        }
        cout << "  The current service: "
             << service
             << "    All flow: "
             << static_cast<double>(allflow) / 1024 << "GB" << endl;
        if(!m_service.sendGetBookedService(service)){
            cout << m_service.getLastError() << endl;
            cout << " Fail to get booked service." << endl;
            return false;
        }
        cout << "  The booked service: " << service << endl;
        return true;
    }
    return false;
}

bool InterAction::BookService()
{
    if(Connected()){
        int sid = 0;
        vector<tuple<int, string, string>> services;
        if(!m_service.sendGetAllServices(services)){
            cout << m_service.getLastError() << endl;
            cout << " Fail to list all services." << endl;
            return false;
        }
        if(services.empty()){
            cout << " No avaliable services." << endl;
            return false;
        }
        cout << " The list of all services:" << endl;
        for(int i = 0, i_end = static_cast<int>(services.size()); i < i_end; ++i){
            const auto &s = services[i];
            cout << "  " << i+1 << " | " << get<1>(s) << "  | " << get<2>(s) << endl;
        }
        cout << " Choose a service(1-" << services.size() << ", 0 for menu):";
        string data;
        cin >> data;
        try{
            sid = std::stoi(data);
        }catch(...){
            cout << "Invalid number." << endl;
            return false;
        }
        if(sid < 1 || sid > static_cast<int>(services.size())){
            cout << " Wrong choice. Return to menu." << endl;
            return false;
        }
        if(m_service.sendSetBookedService(get<0>(services[sid-1]))){
            cout << " Success to book service: " << get<1>(services[sid-1]) << endl;
            return true;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to book service." << endl;
        }
    }
    return false;
}

bool InterAction::SetAutoMenu()
{
    cout << " Do you want to print menu automatically after a operation?(y/n) ";
    string input;
    cin >> input;
    if(input == "y" || input == "yes"){
        m_bShowMenu = true;
        cout << "Will print menu automatically after a operation." << endl;
    }
    else if(input == "n" || input == "no"){
        m_bShowMenu = false;
        cout << "Unrecognized input." << endl;
        cout << "Will not print menu after a operation." << endl;
    }
    else{
        cout << "Unrecognized input." << endl;
        return false;
    }
    return true;
}

bool InterAction::SetAutoStart()
{
    if(Connected()){
        cout << " Do you want to start <" BNA_NAME "> automatically? (y/n) ";
        string input;
        cin >> input;
        bool succ = false;
        if(input == "y" || input == "yes"){
            succ = m_service.sendSetAutoStart(true);
        }
        else if(input == "n" || input == "no"){
            succ = m_service.sendSetAutoStart(false);
        }
        if(!succ){
            cout << m_service.getLastError() << endl;
            cout << " Fail to change setting." << endl;
        }
        else{
            cout << " OK." << endl;
        }
        return succ;
    }
    return false;
}

bool InterAction::EnterDebugMode()
{
    if(Connected()){
        if(m_service.sendActEnterDebugMode()){
            cout << " Enter DEBUG mode!!!!!!" << endl;
        }
        else{
            cout << " Fail to enter DEBUG mode." << endl;
        }
        string token;
        size_t nVaildTime;
        double dVaildTime;
        string strUnit[4] = {"seconds", "minutes", "hours", "days"};
        int nUnitIdx = 0;
        if(m_service.sendRegistDevelop(token, nVaildTime)){
            cout << " Regist token code: " << token << endl;
            dVaildTime = static_cast<double>(nVaildTime);
            if(nUnitIdx == 0 && dVaildTime >= 60.){
                dVaildTime /= 60.;
                ++nUnitIdx;
            }
            if(nUnitIdx == 1 && dVaildTime >= 60.){
                dVaildTime /= 60.;
                ++nUnitIdx;
            }
            if(nUnitIdx == 2 && dVaildTime >= 24.){
                dVaildTime /= 24.;
                ++nUnitIdx;
            }
            cout << " This token is vaild for " << setprecision(3) << dVaildTime << " " << strUnit[nUnitIdx] << "." << endl;
            return true;
        }
        else{
            cout << " Fail to regist token code." << endl;
        }
    }
    return false;
}

bool InterAction::LeaveDebugMode()
{
    if(Connected()){
        if(m_service.sendActLeaveDebugMode()){
            cout << " Leave DEBUG mode!!!!!!" << endl;
        }
        else{
            cout << " Fail to leave DEBUG mode." << endl;
        }

        if(m_service.sendUnregistDevelop()){
            cout << " Unregist token code." << endl;
            return true;
        }
        else{
            cout << " Fail to unregist token code." << endl;
        }
    }
    return false;
}

bool InterAction::SetAnotherHost()
{
    string address;
    cout << " Input the host address: ";
    cin >> address;
    if(!SetHost(address)){
        return false;
    }
    if(!Connected()){
        return false;
    }
    if(!m_service.sendSyncHello()){
        cout << m_service.getLastError() << endl;
        return false;
    }
    cout << " Connected to " << address << endl;
    return true;
}

bool InterAction::ShowMyAddress()
{
    std::vector<string> ip;
    m_service.getMyAddress(ip);
    cout << " IP address list: " << endl;
    for(const auto &a : ip){
        cout << "   " << a << endl;
    }
    return !ip.empty();
}

bool InterAction::StartDaemon()
{
    cout << " Starting up the damon of " BNA_NAME << endl;
    bool suc = m_service.startDaemon();
    cout << (suc ? "   Done." : "   Failed.");
    return suc;
}

bool InterAction::StopDaemon()
{
    cout << " Shutting down the damon of " BNA_NAME << endl;
    bool suc = m_service.killDaemon();
    cout << (suc ? "   Done." : "   Failed.");
    return suc;
}

bool InterAction::SetHost(const string &host)
{
    if(m_service.setHost(host)){
        m_bVerifyMsgVer = false;
        cout << " Host address is " << host << " now." << endl;
        if(host == "127.0.0.1"){
            m_service.setAuth(false);
            return true;
        }
        cout << " Which authentication do you want to use?" << endl;
        cout << "   1. account and password."<<endl;
        cout << "   2. token code." << endl;
        cout << " Your choice: ";
        string data;
        int type;
        cin >> data;
        try{
            type = std::stoi(data);
        }catch(...){
            cout << "Invalid number." << endl;
            SetHost("127.0.0.1");
            return false;
        }
        if(type == 1) {
            std::string name, passwd;
            cout << " Input your account: ";
            cin.ignore(INT_MAX, '\n');
            cin >> name;
            ConsoleInputPasswd(" Input your password: ", passwd, '*');
            m_service.setAuth(true, name, passwd);
        }
        else if(type == 2){
            std::string token;
            cout << " Input your token: ";
            cin.ignore(INT_MAX, '\n');
            cin >> token;
            m_service.setAuth(true, true, token);
        }
        else{
            cout << "Error type.";
            SetHost("127.0.0.1");
            return false;
        }
        if(Connected()){
            if(m_service.sendSyncHello()){
                cout << "Success to sync host." << endl;
            }
            else{
                cout << "Fail to sync host." << endl;
                cout << m_service.getLastError() << endl;
                return false;
            }
        }
        else{
            cout << "Fail to connect to host." << endl;
            return false;
        }
        return true;
    }
    else{
        cout << " Invalid host address: " << host << endl;
        SetHost("127.0.0.1");
    }
    return false;
}

bool InterAction::ShowVersion()
{
    string version;
    int inner_ver;
    if(Connected()){
        if(m_service.sendGetVersion(version, inner_ver)){
            cout << BNW_NAME << "  " << version << " (" << inner_ver << ")" << endl;
        }
        else{
            cout << BNW_NAME << "  Fail to query version." << endl;
        }
    }
    cout << BNC_NAME << "  " << BNA_VERSION << " (" << BNA_INNER_VERSION << ")" << endl << endl;
    cout << "The assistant of BJUT net. It has the functions of automatic login,"
            " online maintemance, service management and so on." << endl;
    return true;
}
}
