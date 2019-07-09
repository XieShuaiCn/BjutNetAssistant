#include "DebugInfo.h"
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QByteArray>
#include "common.h"

namespace bna{

void DebugTool::init(const QString &name)
{
    m_file.setFileName(name);
    m_file.open(QFile::ReadWrite);
    m_strLastError.clear();
    m_nLastError = 0;
}

void DebugTool::release()
{
    if(m_file.isOpen()) m_file.close();
}

void DebugTool::setInfo(const QString &content, bool with_time)
{
    m_strLastError.clear();
    QDateTime timeNow = QDateTime::currentDateTime();
    if (with_time)
    {
        QString strCurrentTime = timeNow.toString("[yyyy-MM-dd hh:mm:ss:zzz] ");
        m_strLastError.append(strCurrentTime);
    }
    m_strLastError.append(content);
}

void DebugTool::writeInfo(DebugStatus status, const QString &content, bool with_time, bool end_line)
{
    setInfo(content, with_time);

    if(!m_file.isOpen())
        return;
    qint64 num = 0;
    QDateTime timeNow = QDateTime::currentDateTime();
    if (with_time)
    {
        QString strCurrentTime = timeNow.toString("[yyyy-MM-dd hh:mm:ss:zzz]");
        QByteArray tempData = strCurrentTime.toUtf8();
        num += m_file.write(tempData.data());
    }
    if(status!=STATUS_NONE){
        QString s;
        switch (status) {
        case STATUS_SUCCESS:
            s = "Succ";
            break;
        case STATUS_FAIL:
            s = "Fail";
            break;
        case STATUS_INFO:
            s = "Info";
            break;
        case STATUS_DATA:
            s = "Data";
            break;
        case STATUS_NONE:
        default:
            break;
        }
        num += m_file.write(QString("[%1] ").arg(s).toUtf8());
        if(STATUS_DATA == status && content.size() > 100){
            QString dataFileName = "log_data_" + timeNow.toString("yyyyMMddhhmmss_");
            static QDir dirAppTemp(g_strAppTempPath);
            for(int i =0; i < 32; ++i)
            {
                QString rs = RandString(8);
                auto tmpName = dirAppTemp.absoluteFilePath(dataFileName + rs + ".dat");
                if(!QFileInfo(tmpName).exists()){
                    dataFileName = tmpName;
                    break;
                }
            }
            QFile fData(dataFileName);
            bool writeDataOK = false;
            if(fData.open(QFile::WriteOnly)){
                writeDataOK = 0 < fData.write(content.toUtf8());
                fData.close();
            }
            if(writeDataOK){
                num += m_file.write(dataFileName.toUtf8());
            }
            else{
                num += m_file.write(content.toUtf8());
            }
        }
        else{
            num += m_file.write(content.toUtf8());
        }
    }
    else{
        num += m_file.write(content.toUtf8());
    }
    if(end_line)
    {
        num += m_file.write("\n");
    }
}

}
