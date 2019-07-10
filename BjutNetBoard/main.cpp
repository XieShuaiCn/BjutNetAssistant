#include "common.h"
#include "WndTrayIcon.h"
#include <QtSingleApplication>
#include <QDir>

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
    QtSingleApplication app("APP_UUID_BjutNetLogin3", argc, argv);
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
    bnm::g_strAppTempPath = tmpName;
    QDir("/").mkpath(g_strAppTempPath);
    InitDebugFile(QDir(g_strAppTempPath).absoluteFilePath("bnl.log"));
    g_bAppDebug = mapArgv.contains("debug");

    WndTrayIcon wti(&app);
    wti.show();
    if (!mapArgv.contains("tray"))
    {
        wti.cmdShowMainWnd();
    }
    //wti.cmdShowSettingWnd();
    QObject::connect(&app, &QtSingleApplication::messageReceived, &wti, &WndTrayIcon::reciveMessage);
    int r = app.exec();
    WriteDebugInfo(QString("Exit code:") + QString(std::to_string(r).c_str()));
    ReleaseDebugFile();
    return r;
}
