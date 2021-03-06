#include "common.h"
#include <QFile>
#include <QTime>

namespace bna{

bool g_bAppDebug = false;

QString g_strAppTempPath;

QString g_strAppDirPath;

namespace core{
QString RandString(int length)
{
    qsrand(QTime::currentTime().msec()
           +QTime::currentTime().second()*1000
           +QTime::currentTime().minute()*60000);
    const QString set("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    QString result;
    for(int i = 0; i < length; ++i) {
        result.append(set[qrand() % 36]);
    }
    return result;
}

}}
