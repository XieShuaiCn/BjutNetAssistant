
#include "common.h"
#include <QDir>
#include <time.h>
#include <QtSingleApplication>
#include "BjutNet.h"
#include "ServiceInterface.h"

using namespace bna;

int main(int argc, char *argv[])
{
    QtSingleApplication app("BjutNetAssistant_UUID_APPV4", argc, argv);
    if(app.isRunning())
    {
        app.sendMessage("ShowMainWnd");
        return 0;
    }
    // change pwd
    QDir::setCurrent(app.applicationDirPath());
    // create temp file.
    auto timeSince1970 = time(nullptr);
    QString tmpName = QDir::temp().absoluteFilePath(
                QString("BjutNetLogin_%1.tmp").arg(timeSince1970));
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
        QString rs = RandString(6);
        tmpName = QDir::temp().absoluteFilePath(
                    QString("BjutNetLogin_%1_%2.tmp").arg(timeSince1970).arg(rs));
    }
    g_strAppTempPath = tmpName;
    QDir("/").mkpath(g_strAppTempPath);
    g_debugTool.init(QDir(g_strAppTempPath).absoluteFilePath("bnl.log"));

#ifdef BUILD_DEVELOP
    g_bAppDebug = true;
    qDebug() << " Application information (DEBUG MODE):" << endl
             << "   DirPath: " << QtSingleApplication::applicationDirPath() << endl
             << "   FilePath" << QtSingleApplication::applicationFilePath() << endl
             << "   DisplayName: " << QtSingleApplication::applicationDisplayName() << endl
             << "   Name: " << QtSingleApplication::applicationName() << endl
             << "   Version: " << QtSingleApplication::applicationVersion() << endl
             << "   Pid: " << QtSingleApplication::applicationPid() << endl
             << "   CurrentPath: " << QDir::currentPath() << endl
             << "   TempPath: " << g_strAppTempPath << endl
             << "   LogFile: " << g_debugTool.fileName() << endl
             << " ######################################################################" << endl
             << " Running..." << endl;
#endif

    bna::BjutNet bjutnet;
    bna::ServiceInterface si(&bjutnet);

    bjutnet.loadAccount();
    if(si.Initilize()){
        return app.exec();
    }
    else{
        qDebug() << "Fail to initialize service. Exit now." << endl;
        app.exit(0);
        return 0;
    }
}
