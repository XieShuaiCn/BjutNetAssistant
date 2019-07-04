#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QObject>
#include <QString>

extern bool g_bAppDebug;

enum DebugStatus{
    DEBUG_INFO,
    DEBUG_SUCCESS,
    DEBUG_FAIL
};

void InitDebugFile(const QString &name);

void WriteDebugInfo(const QString &content, bool with_time = true, bool end_line = true);

void WriteDebugInfo(const DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);

void ReleaseDebugFile();


#endif // DEBUGINFO_H
