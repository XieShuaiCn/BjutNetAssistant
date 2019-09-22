#ifndef BNS_MESSAGEVALUE_H
#define BNS_MESSAGEVALUE_H

namespace bna{
namespace core{

const char CHAR_ENQ = '\x05';
const char CHAR_ACK = '\x06';
const char CHAR_NAK = '\x15';
const char CHAR_SYN = '\x16';

class MessageValue
{
public:
    static const int ServerPort = 6350;
    static const int Version = 1;

    enum Type{
        ERR=0, //ERROR
        SYNC=1,
        ACT=2,
        GET=3,
        SET=4,
        REGIST=5,
        SYS=6,
        ACK=9
    };

    enum ActionSync{
        HELLO=0
    };
    enum ActionRegist{
        REGIST_DEVELOP,
        UNREGIST_DEVELOP,
        REGIST_MESSAGE,
        MESSAGE_CHANGE,
        REGIST_STATUS,
        STATUS_CHANGE,
    };

    enum ActionAct{
        ACT_LOAD_ACCOUNT=1,
        ACT_SAVE_ACCOUNT=2,
        ACT_LOGIN_BJUT=3,
        ACT_LOGOUT_BJUT=4,
        ACT_LOGIN_JFSELF=5,
        ACT_LOGOUT_JFSELF=6,
        ACT_REFRESH_NET=7,
        ACT_REFRESH_JFSELF_ACCOUNT=8,
        ACT_REFRESH_ONLINE=9,
        ACT_ENTER_DEBUG_MODE,
        ACT_LEAVE_DEBUG_MODE,
    };

    enum ActionGet{
        GET_VERSION=1,
        GET_ACCOUNT=2,
        GET_LOGIN_STATUS=3,
        GET_USED_FLOW=4,
        GET_USED_TIME=5,
        GET_LEFT_FEE=6,
        GET_ALL_FLOW=7,
        GET_DEVICE_ONLINE=8,
        GET_FLOW_SERVICE=9,
        GET_BOOKED_SERVICE=10,
        GET_ALL_SERVICES=11,
        GET_AUTO_START=12
    };

    enum ActionSet{
        SET_ACCOUNT=1,
        SET_BOOK_SERVICE=2,
        SET_OFFLINE_DEVICE=3,
        SET_AUTO_START=4
    };

    enum ActionSys{
        SYS_EXIT,
        SYS_PAUSE,
        SYS_START,
    };

    template<typename _Tp>
    struct Convert{
        static _Tp From(int n)
        {
            return _Tp(n);
        }
    };

    using Action = int;

    using ConvertToType = Convert<Type>;
    using ConvertToActSync = Convert<ActionSync>;
    using ConvertToActAct = Convert<ActionAct>;
    using ConvertToActGet = Convert<ActionGet>;
    using ConvertToActSet = Convert<ActionSet>;
    using ConvertToActRegist = Convert<ActionRegist>;
    using ConvertToActSys = Convert<ActionSys>;
};

extern template struct MessageValue::Convert<MessageValue::Type>;
extern template struct MessageValue::Convert<MessageValue::ActionSync>;
extern template struct MessageValue::Convert<MessageValue::ActionAct>;
extern template struct MessageValue::Convert<MessageValue::ActionGet>;
extern template struct MessageValue::Convert<MessageValue::ActionSet>;
extern template struct MessageValue::Convert<MessageValue::ActionRegist>;
extern template struct MessageValue::Convert<MessageValue::ActionSys>;

}
}
#endif // BNS_MESSAGEVALUE_H
