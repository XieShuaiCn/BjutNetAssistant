
#include "common.h"
#include <QDir>
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
#ifdef QT_DEBUG
    qDebug() << QtSingleApplication::applicationDirPath() << endl
             << QDir::currentPath() << endl;
#endif
    // change pwd
    QDir::setCurrent(app.applicationDirPath());
    // create temp file.
    QString tmpName = QDir::temp().absoluteFilePath("BjutNetLogin_.tmp");
    for(int i =0; i < 30; ++i)
    {
        QString rs = RandString(6);
        tmpName = QDir::temp().absoluteFilePath("BjutNetLogin_" + rs + ".tmp");
        QDir tmpDir(tmpName);
        if(tmpDir.exists()){
            if(!tmpDir.rmpath(tmpName) && i > 10)
                break;
        }
        if(!tmpDir.exists()){
            break;
        }
    }
    g_strAppTempPath = tmpName;
    QDir("/").mkpath(g_strAppTempPath);
    InitDebugFile(QDir(g_strAppTempPath).absoluteFilePath("bnl.log"));
    bna::BjutNet bjutnet;
    bna::ServiceInterface si(&bjutnet);

    if(si.Initilize()){
        return app.exec();
    }
    else{
        qDebug() << "Fail to initialize service. Exit now." << endl;
        app.exit(0);
        return 0;
    }
}
