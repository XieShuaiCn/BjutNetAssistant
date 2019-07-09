
#include "Setting.h"
#include "common.h"
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QApplication>
#include <QDebug>

namespace bna{

#define BNA_AUTOSTART_LINUX_FILENAME "BjutNetService.sh.desktop"

bool Setting::setAutoRun(bool bAutoRun)
{
    try{
#ifdef Q_OS_WIN
        QSettings autoRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                          QSettings::NativeFormat);
        if(bAutoRun)
        {
            autoRun.setValue(BNS_NAME,
                             QApplication::applicationFilePath().replace(QChar('/'), QChar('\\')));
        }
        else
        {
            autoRun.remove(BNS_NAME);
        }
        return autoRun.contains(BNS_NAME) == bAutoRun;
#elif defined(Q_OS_LINUX)
        QDir autoStart = QDir::home();
        if(autoStart.mkpath(".config/autostart")) {
            if(autoStart.cd(".config/autostart")) {
                QFileInfo startScriptNew(autoStart.absoluteFilePath(BNA_AUTOSTART_LINUX_FILENAME));
                if(bAutoRun){
                    // create desktop file in autostart folder
                    QFile startScript(QDir(g_strAppDirPath).absoluteFilePath(BNA_AUTOSTART_LINUX_FILENAME));
                    if(startScript.exists()){
                        if(startScriptNew.exists()){
                            autoStart.remove(startScriptNew.filePath());
                        }
                        if(!startScript.copy(startScriptNew.filePath())) {
                            g_debugTool.setInfo(startScript.errorString());
                            if(g_bAppDebug){
                                g_debugTool.writeInfo(DebugTool::STATUS_FAIL, startScript.errorString());
                            }
                            return false;
                        }
                        return true;
                    }
                }
                else{
                    if(startScriptNew.exists()){
                        return autoStart.remove(BNA_AUTOSTART_LINUX_FILENAME);
                    }
                }
            }
        }
#else

#endif
    }
    catch(...){ }
    return false;
}

bool Setting::getAutoRun()
{
    try{
#ifdef Q_OS_WIN
        QSettings autoRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                          QSettings::NativeFormat);
        return autoRun.contains(BNS_NAME);
#elif defined(Q_OS_LINUX)
        QDir autoStart = QDir::home();
        if(autoStart.cd(".config/autostart")) {
            return QDir(autoStart.filePath(BNA_AUTOSTART_LINUX_FILENAME)).exists();
        }
#else

#endif
    }catch(...){ }
    return false;
}

#undef BNA_AUTOSTART_LINUX_FILENAME

}
