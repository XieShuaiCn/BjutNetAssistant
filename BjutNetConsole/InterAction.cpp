#include "InterAction.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include "Version.h"

using namespace std;

namespace bna{

void InterAction::ShowMenu()
{
    cout << endl;
    cout << "  ================================================" << endl;
    cout << "                       Menu                       " << endl;
    cout << "  ================================================" << endl;
    cout << "  command |        description                    " << endl;
    cout << "  ------------------------------------------------" << endl;
    cout << "    q     |  Quit this session.                   " << endl;
    cout << "    s     |  Show the status of login.            " << endl;
    cout << "    r     |  Refresh all status.                  " << endl;
    cout << "    v     |  Version information.                 " << endl;
    cout << "    li    |  Login BJUT net now.                  " << endl;
    cout << "    lo    |  Logout BJUT net now.                 " << endl;
    cout << "    ol    |  List online devices.                 " << endl;
    cout << "    onl   |  List online devices.                 " << endl;
    cout << "    ofl   |  Offline devices.                     " << endl;
    cout << "    lda   |  Reload account profile.              " << endl;
    cout << "    sna   |  Set new account profile.             " << endl;
    cout << "    csv   |  Show the current service             " << endl;
    cout << "    bsv   |  Book the service of next month.      " << endl;
    cout << "    ats   |  Auto start service when power on.    " << endl;
    cout << "    addr  |  Show my ip address.                  " << endl;
    cout << "  ================================================" << endl;
#if defined(_DEBUG) || defined(BUILD_DEVELOP)
    cout << "    enter_debug     Enter debug mode.             " << endl;
    cout << "    leave_debug     Leave debug mode.             " << endl;
    cout << "    set_host        Set host address.             " << endl;
    cout << "    my_host         Show host address.            " << endl;
    cout << "  ================================================" << endl;
#endif
    cout << "Input your command: ";
    cout.flush();
}

bool InterAction::Process()
{
    try{
        string input;
        ShowMenu();
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
        return true;
    }
    else{
        cout << "Can not connect to <BjutNetService>." << endl;
        return false;
    }
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
        cout << " Used flow: " << fixed << setprecision(3) << fflow << flowUnit[flowUnitIdx]
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
    if(Connected()){
        if(!ShowOnline()){
            return false;
        }
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
            cout << " Input your password: ";
            InputPasswd(passwd, '*');
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

bool InterAction::SetAutoStart()
{
    cout << " Do you want to start <BjutNetService> automatically (y/n)? ";
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
    return succ;
}

bool InterAction::EnterDebugMode()
{
    if(Connected()){
        if(m_service.sendActEnterDebugMode()){
            cout << " Enter DEBUG mode!!!!!!" << endl;
            return true;
        }
        cout << " Fail to enter DEBUG mode." << endl;
    }
    return false;
}

bool InterAction::LeaveDebugMode()
{
    if(Connected()){
        if(m_service.sendActLeaveDebugMode()){
            cout << " Leave DEBUG mode!!!!!!" << endl;
            return true;
        }
        cout << " Fail to leave DEBUG mode." << endl;
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

bool InterAction::SetHost(const string &host)
{
    if(m_service.setHost(host)){
        cout << " Host address is " << host << " now." << endl;
        return true;
    }
    else{
        cout << " Invalid host address: " << host << endl;
    }
    return false;
}

void InterAction::InputPasswd(std::string &passwd, char echo)
{
    char ch;
    while((ch=getch())!='\r')
    {
        if(ch!=8)//不是回撤就录入
        {
            passwd.push_back(ch);
            putchar(echo);//并且输出*号
        }
        else if(passwd.size())
        {
            putchar('\b');//这里是删除一个，我们通过输出回撤符 /b，回撤一格，
            putchar(' ');//再显示空格符把刚才的*给盖住，
            putchar('\b');//然后再 回撤一格等待录入。
            passwd.pop_back();
        }
    }
    passwd.push_back('\0');
    putchar('\r');
    putchar('\n');
}

void InterAction::ShowVersion()
{
    string version;
    int inner_ver;
    if(Connected() && m_service.sendGetVersion(version, inner_ver)){
        cout << BNS_NAME << "  " << BNA_VERSION << " (" << BNA_INNER_VERSION << ")" << endl;
    }
    cout << BNC_NAME << "  " << BNA_VERSION << " (" << BNA_INNER_VERSION << ")" << endl;
    cout << "The assistant of BJUT net. It has the functions of automatic login,"
            " online maintemance, service management and so on." << endl;
}
}
