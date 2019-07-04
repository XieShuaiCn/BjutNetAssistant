#include "DebugInfo.h"
#include <QFile>
#include <QDateTime>
#include <QByteArray>

bool g_bAppDebug = false;
QFile g_fileDebug;
bool g_bDbgFileOpened = false;


void InitDebugFile(const QString &name)
{
    g_fileDebug.setFileName(name);
    g_fileDebug.open(QFile::ReadWrite);
    g_bDbgFileOpened = g_fileDebug.isOpen();
}

void ReleaseDebugFile()
{
    if(g_fileDebug.isOpen()) g_fileDebug.close();
}

void WriteDebugInfo(const DebugStatus status, const QString &content, bool with_time, bool end_line)
{
    QString s;
    switch (status) {
    case DEBUG_SUCCESS:
        s = "Succ";
        break;
    case DEBUG_FAIL:
        s = "Fail";
        break;
    case DEBUG_INFO:
    default:
        s = "Info";
        break;
    }
    WriteDebugInfo(QString("[%1] %2").arg(s).arg(content), with_time, end_line);
}

void WriteDebugInfo(const QString &content, bool with_time, bool end_line)
{
    if(!g_fileDebug.isOpen())
        return;
    qint64 num = 0;
    if (with_time)
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strCurrentTime = time.toString("[yyyy-MM-dd hh:mm:ss:zzz]");
        QByteArray tempData = strCurrentTime.toUtf8();
        num += g_fileDebug.write(tempData.data());
    }
    {
        QByteArray tempData = content.toUtf8();
        num += g_fileDebug.write(tempData.data());
    }
    if(end_line)
    {
        num += g_fileDebug.write("\n");
    }
}
