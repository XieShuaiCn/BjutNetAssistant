#ifndef SETTING_H
#define SETTING_H

#include "common.h"

namespace bna{

class Setting
{
public:

    static bool setAutoRun(bool autorun = true);
    static bool getAutoRun();

    // 调试信息
    void debug_info(DebugTool::DebugStatus status, const QString &content, bool with_time = true, bool end_line = true);
};

}

#endif // SETTING_H
