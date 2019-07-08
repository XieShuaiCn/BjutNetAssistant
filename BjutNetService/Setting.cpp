
#include "Setting.h"
#include "common.h"
#include <QSettings>
#include <QApplication>

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
#else
#endif
    }catch(...){ }
    return false;
}
