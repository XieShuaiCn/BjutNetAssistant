#include <iostream>
#include "ParamParse.h"
#include "Interaction.h"

using namespace std;
using namespace bna;


void ShowUsage()
{
    cout << "Usage: bjutnet [command] [parameter...]" << endl << endl;
    cout << "The assistant of BJUT net. It has the functions of automatic login,"
            " online maintemance, service management and so on." << endl << endl;
    cout << "NO commands && NO parameters:     execute command interactively." << endl;
    cout << "command:" << endl;
    cout << "    login        " << endl;
    cout << "    logout       " << endl;
    cout << "parameter:" << endl;
    cout << "    -V, -version           " << endl;
    cout << "    -name           " << endl;
    cout << "    -passwd           " << endl;
    cout << "    -type           " << endl;
}

int main(int argc, char* argv[])
{
    ParamParse parser;
    if(!parser.parse(argc, argv)) {
        ShowUsage();
        return 0;
    }
    InterAction act;
    if(!parser.getHost().empty()){
        act.SetHost(parser.getHost());
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
