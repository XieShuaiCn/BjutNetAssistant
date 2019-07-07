#ifndef BNS_MESSAGEVALUE_H
#define BNS_MESSAGEVALUE_H

namespace bna{

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
        ACK=9
    };

    enum ActionSync{
        HELLO=0,
        REGIST_MESSAGE=1,
        MESSAGE_CHANGE=2,
        REGIST_STATUS=3,
        STATUS_CHANGE=4,
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
        ACT_REFRESH_ONLINE=9
    };

    enum ActionGet{
        GET_VERSION=1,
        GET_ACCOUNT=2,
        GET_USED_FLOW=3,
        GET_USED_TIME=4,
        GET_LEFT_FEE=5,
        GET_ALL_FLOW=6,
        GET_DEVICE_ONLINE=7,
        GET_FLOW_SERVICE=8,
        GET_BOOKED_SERVICE=9,
        GET_ALL_SERVICES=10
    };

    enum ActionSet{
        SET_ACCOUNT=1,
        SET_BOOK_SERVICE=2,
        SET_OFFLINE_DEVICE=3
    };

    template<typename _Tp>
    struct Convert{
        static _Tp To(int n)
        {
            return _Tp(n);
        }
    };

    using ConvertToType = Convert<Type>;
    using ConvertToActSync = Convert<ActionSync>;
    using ConvertToActAct = Convert<ActionAct>;
    using ConvertToActGet = Convert<ActionGet>;
    using ConvertToActSet = Convert<ActionSet>;
};

extern template struct MessageValue::Convert<MessageValue::Type>;
extern template struct MessageValue::Convert<MessageValue::ActionSync>;
extern template struct MessageValue::Convert<MessageValue::ActionAct>;
extern template struct MessageValue::Convert<MessageValue::ActionGet>;
extern template struct MessageValue::Convert<MessageValue::ActionSet>;

}

#endif // BNS_MESSAGEVALUE_H
