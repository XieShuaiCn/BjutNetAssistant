#include "../BjutNetService/common.h"
#include "../BjutNetService/BjutNet.h"
#include "../BjutNetService/ServiceInterface.h"
#include "WndTrayIcon.h"
#include <QtSingleApplication>
#include <QDir>

using namespace bna::core;

int main(int argc, char *argv[])
{
    QMap<QString, QString> mapArgv;
    {
        QString lastArg;
        for(int i = 1; i < argc; ++i)
        {
            if(argv[i][0] == '-') //key or option, insert temporarily
            {
                QString tmp(argv[i]);
                lastArg = tmp.right(tmp.size()-1);
                mapArgv.insert(lastArg, "");
            }
            else //set value
            {
                if(lastArg.size() && mapArgv.contains(lastArg)){
                    mapArgv[lastArg] = QString(argv[i]);
                    lastArg.clear();
                }else {
                    qDebug() << "Error parameter: Unexpected '" << argv[i] << "' after '" << argv[i-1] << "'.";
                    return 0;
                }
            }
        }
    }
    // start single instance
    QtSingleApplication app("BjutNetAssistant_Board_UUID_APPV4", argc, argv);
    if(app.isRunning())
    {
        app.sendMessage("ShowMainWnd");
        return 0;
    }
#ifdef QT_DEBUG
    qDebug() << QtSingleApplication::applicationDirPath() << endl
             << QDir::currentPath() << endl;
#endif
    // change pwd
    QDir::setCurrent(app.applicationDirPath());
    // create temp file.
    auto timeSince1970 = time(nullptr);
    QString tmpName = QDir::temp().absoluteFilePath(
                QString("BjutNetService_%1.tmp").arg(timeSince1970));
    for(int i =0; i < 255; ++i)
    {
        QDir tmpDir(tmpName);
        if(tmpDir.exists()){
            if(!tmpDir.rmpath(tmpName) && i > 10)
                break;
        }
        if(!tmpDir.exists()){
            break;
        }
        QString rs = bna::core::RandString(6);
        tmpName = QDir::temp().absoluteFilePath(
                    QString("BjutNetService_%1_%2.tmp").arg(timeSince1970).arg(rs));
    }
    bna::g_strAppTempPath = tmpName;
    QDir("/").mkpath(bna::g_strAppTempPath);
    DebugTool::I().init(QDir(bna::g_strAppTempPath).absoluteFilePath("bns.log"));
    bna::g_strAppDirPath = QtSingleApplication::applicationDirPath();

#ifdef BUILD_DEVELOP
    bna::g_bAppDebug = true;
    qDebug() << " Application information (DEBUG MODE):" << endl
             << "   DirPath: " << QtSingleApplication::applicationDirPath() << endl
             << "   FilePath" << QtSingleApplication::applicationFilePath() << endl
             << "   DisplayName: " << QtSingleApplication::applicationDisplayName() << endl
             << "   Name: " << QtSingleApplication::applicationName() << endl
             << "   Version: " << QtSingleApplication::applicationVersion() << endl
             << "   Pid: " << QtSingleApplication::applicationPid() << endl
             << "   CurrentPath: " << QDir::currentPath() << endl
             << "   TempPath: " << bna::g_strAppTempPath << endl
             << "   LogFile: " << DebugTool::I().fileName() << endl
             << " ######################################################################" << endl
             << " Running..." << endl;
#endif

    BjutNet core_bjutnet;
    ServiceInterface core_svrif(&core_bjutnet, &app);
    core_bjutnet.loadAccount();
    if(core_svrif.Initilize()){
        // create tray
        bna::gui::WndTrayIcon gui_wti(&app, &core_bjutnet);
        gui_wti.show();
        if (!mapArgv.contains("tray"))
        {
            gui_wti.cmdShowMainWnd();
        }
        //wti.cmdShowSettingWnd();
        QObject::connect(&app, &QtSingleApplication::messageReceived, &gui_wti, &bna::gui::WndTrayIcon::reciveMessage);

        int app_ret = app.exec();
        DebugTool::I().release();
        return app_ret;
    }
    else{
#ifdef BUILD_DEVELOP
        qDebug() << "Fail to initialize service. Exit now." << endl;
#endif
        DebugTool::I().writeFail("Fail to initialize service. Exit now.");
        DebugTool::I().release();
        app.exit(0);
        return 0;
    }
}
