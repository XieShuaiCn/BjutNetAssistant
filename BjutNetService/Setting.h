#ifndef SETTING_H
#define SETTING_H

namespace bna{

class Setting
{
public:

    static bool setAutoRun(bool autorun = true);
    static bool getAutoRun();
};

}

#endif // SETTING_H
