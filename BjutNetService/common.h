#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QString>
#include <QException>
#include "Version.h"
#include "DebugInfo.h"

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define PROPERTY_READ(type, name, var) \
    type get##name() const { return (var); }

#define PROPERTY_READ_CONST(type, name, var) \
    const type &get##name() const { return (var); }

#define PROPERTY_WRITE(type, name, var) \
    void set##name(type value) { var = value; }

#define PROPERTY_READ_WRITE(type, name, var) \
    PROPERTY_READ(type, name, var) \
    PROPERTY_WRITE(type, name, var)

namespace bna{

extern bool g_bAppDebug;

extern DebugTool g_debugTool;

extern QString g_strAppTempPath;

QString RandString(int length);
}
#endif // COMMON_H
