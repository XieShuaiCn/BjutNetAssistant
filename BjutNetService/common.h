#ifndef BNS_COMMON_H
#define BNS_COMMON_H

#include <QObject>
#include <QString>
#include <QException>
#include "Version.h"
#include "DebugInfo.h"

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#ifndef BNA_PI
#define BNA_PI (3.14159265358979323846)
#endif
#ifndef BNA_2PI
#define BNA_2PI (6.28318530717958647692)
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

extern QString g_strAppTempPath;

extern QString g_strAppDirPath;

namespace core{
QString RandString(int length);
}
}
#endif // BNS_COMMON_H
