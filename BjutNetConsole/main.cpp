#include <iostream>
#include "Utility.h"
#include "ParamParse.h"
#include "InterAction.h"

using namespace std;
using namespace bna;
using namespace bna::core;


void ShowUsage()
{
    cout << "Usage: bjutnet [command] [parameter...]" << endl;
    cout << endl;
    cout << "The assistant of BJUT net. It has the functions of automatic login,"
            " online maintemance, service management and so on." << endl;
    cout << endl;
    cout << "NO commands && NO parameters:     execute command interactively." << endl;
    cout << "command:" << endl;
    cout << "    login        login bjut net." << endl;
    cout << "    logout       logout bjut net." << endl;
    cout << "    online       show online devices." << endl;
    cout << "    status       show the status of net." << endl;
    cout << "    start        start the demon." << endl;
    cout << "    stop         stop the demon." << endl;
    cout << "parameter:" << endl;
    cout << "    -V, -version           " << endl;
    cout << "    -name           " << endl;
    cout << "    -passwd           " << endl;
    cout << "    -type           " << endl;
    cout << endl;
}

int main(int argc, char* argv[])
{
    g_strExeFilePath.clear();
    g_strExeFilePath.append(argv[0]);
    ParamParse parser;
    if(!parser.parse(argc, argv)) {
        ShowUsage();
        return 0;
    }
    InterAction act;
    if(!parser.getHost().empty()){
        if(!act.SetHost(parser.getHost())){
            cout << " Faild to set remote host. Exit now!" << endl;
            return 0;
        }
    }
    if(parser.getSize() == 0){
        // interactive
        while(act.Process());
    }
    else{
        auto op = parser.getOperator();

        if(!op.empty()) {
            if (op == "login"){
                // login bjutnet
                act.LoginBjut();
            }
            else if (op == "logout"){
                // logout bjutnet
                act.LogoutBjut();
            }
            else if(op == "online"){
                // online devices
                act.ShowOnline();
            }
            else if(op == "status"){
                // online status
                act.ShowStatus();
            }
            else if(op == "start"){
                // start damon
                act.StartDaemon();
            }
            else if(op == "stop"){
                // stop damon
                act.StopDaemon();
            }
            else{
                cout << "Unrecognized command: " << op << endl;
                cout << "For more information, please type with '-h'." << endl;
            }
        }
        else{
            if(parser.hasSeted("version") || parser.hasSeted("v")){
                act.ShowVersion();
            }
            else if(parser.hasSeted("help") || parser.hasSeted("h")){
                ShowUsage();
            }
        }
    }
    cout << endl;
    return 0;
}
