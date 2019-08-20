#include <QObject>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include "WndMain.h"
#include "WndInit.h"

QString g_strDistPath;

int main(int argc, char *argv[])
{
    //Debugging complex command lines is currently not supported on Windows.
#if defined(Q_OS_WIN32) && defined(QT_DEBUG)
    char *argv0 = argv[0];
    argv = new char*[4];
    argv[0] = argv0;
    argv[1] = "-update";
    argv[2] = "-dir";
    argv[3] = "D:\\Program Files (x86)\\BjutNetLogin";
    argc = 4;
#endif
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

    QApplication app(argc, argv);
#ifdef QT_DEBUG
    qDebug() << QDir::currentPath() << endl;
    qDebug() << app.applicationDirPath() << endl;
    qDebug() << app.applicationDisplayName() << endl;
    qDebug() << app.applicationFilePath() << endl;
    qDebug() << app.applicationName() << endl;
    qDebug() << app.applicationPid() << endl;
    qDebug() << app.applicationVersion() << endl;
#endif
#ifdef QT_DEBUG
    if(g_strDistPath != QDir::currentPath())
    {
        qDebug() << "Target dir is different from work path." << endl;
    }
#endif
    if(mapArgv.contains("update"))
    {
        g_strDistPath = QDir(mapArgv["dir"]).absolutePath();
        WndMain wm;
        wm.show();
        return app.exec();
    }
    else {
        g_strDistPath = QDir::currentPath();
        WndInit wi(&app);
        wi.show();
        return app.exec();
    }
}
