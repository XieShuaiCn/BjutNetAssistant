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
    cout << "================================================" << endl;
    cout << "              Menu                              " << endl;
    cout << "================================================" << endl;
    cout << "command |        description                    " << endl;
    cout << "------------------------------------------------" << endl;
    cout << "  q     |  Quit this session.                   " << endl;
    cout << "  s     |  Show the status of login.            " << endl;
    cout << "  r     |  Refresh all status.                  " << endl;
    cout << "  v     |  Version information.                 " << endl;
    cout << "  li    |  Login BJUT net now.                  " << endl;
    cout << "  lo    |  Logout BJUT net now.                 " << endl;
    cout << "  ol    |  List online devices with you account." << endl;
    cout << "  lda   |  Reload account profile.              " << endl;
    cout << "  sna   |  Set new account profile.             " << endl;
    cout << "  bsv   |  Book the service of next month.      " << endl;
    cout << "================================================" << endl;
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
            else if(cmd == "ol") {
                ShowOnline();
            }
            else{
                processed = false;
            }
        }
        else if(cmd.size() == 3){
            if(cmd == "lda"){
                ReloadAccount();
            }
            else if(cmd == "sna"){
                SetNewAccount();
            }
            else if(cmd == "bsv") {
                BookService();
            }
            else{
                processed = false;
            }
        }
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

void InterAction::ShowStatus()
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
        m_service.sendGetFlowService(serviceName, allFlow);

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
             << endl
             << " Service: " << serviceName
             << "   | All flow: " << static_cast<double>(allFlow) / 1024 << flowUnit[2];
        if(allFlow > 0){
            cout << "   | Used: " << fixed << setprecision(2) << (100.0 * flow / allFlow / 1024) << "%";
        }
        else{
            cout << "   | Used: -- %";
        }
        cout << endl;
    }
}

void InterAction::LoginBjut()
{
    if(Connected()){
        if(m_service.sendActLoginBjut()){
            cout << " OK." << endl;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << " Fail." << endl;
        }
    }
}

void InterAction::LogoutBjut()
{
    if(Connected()){
        if(m_service.sendActLogoutBjut()){
            cout << " OK." << endl;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << " Fail." << endl;
        }
    }
}

void InterAction::ShowOnline()
{
    if(Connected()){
        vector<array<string, 4>> devices;
        if(m_service.sendGetOnlineDevices(devices))
        {
            if(devices.size()){
                cout << "   ID  |        IPv4        |         IPv6" << endl;
                for(const auto &dev : devices){
                    cout << setw(5) << dev[0] << setw(20) << dev[1]
                         << setw(45)<< dev[2] << endl;
                }
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
}

void InterAction::RefreshNet()
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
    }
}

void InterAction::ReloadAccount()
{
    if(Connected()){
        if(m_service.sendActLoadAccount()){
           cout << " Have reloaded account profile." << endl;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to reload account profile." << endl;
        }
    }
}

void InterAction::SetNewAccount()
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
            cin >> type;
        }while(type< 1 || type > 3);
        if(m_service.sendSetAccount(name, passwd, type)){
            cout << " Success to set new account profile." << endl;
        }
        else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to set new account profile." << endl;
        }
    }
}

void InterAction::BookService()
{
    if(Connected()){
        int sid = 0;
        vector<tuple<int, string, string>> services;
        if(!m_service.sendGetAllServices(services)){
            cout << m_service.getLastError() << endl;
            cout << " Fail to list all services." << endl;
            return;
        }
        if(services.empty()){
            cout << " No avaliable services." << endl;
            return;
        }
        cout << " The list of all services:" << endl;
        for(int i = 0, i_end = static_cast<int>(services.size()); i < i_end; ++i){
            const auto &s = services[i];
            cout << "  " << i+1 << " | " << get<1>(s) << "  | " << get<2>(s) << endl;
        }
        cout << " Choose a service(1-" << services.size() << "):";
        cin >> sid;
        if(sid < 1 || sid > static_cast<int>(services.size())){
            cout << " Wrong choice. Return to menu." << endl;
            return;
        }
        if(m_service.sendSetBookedService(get<0>(services[sid-1]))){
            cout << " Success to book service: " << get<1>(services[sid-1]) << endl;
        }else{
            cout << m_service.getLastError() << endl;
            cout << " Fail to book service." << endl;
        }
    }
}

void InterAction::SetHost(const string &host)
{
    if(m_service.setHost(host)){
        cout << "Host address is " << host << " now." << endl;
    }
    else{
        cout << "Invalid host address: " << host << endl;
    }
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
