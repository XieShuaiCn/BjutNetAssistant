#include "MessageValue.h"

namespace bna{

template struct MessageValue::Convert<MessageValue::Type>;
template struct MessageValue::Convert<MessageValue::ActionSync>;
template struct MessageValue::Convert<MessageValue::ActionAct>;
template struct MessageValue::Convert<MessageValue::ActionGet>;
template struct MessageValue::Convert<MessageValue::ActionSet>;
template struct MessageValue::Convert<MessageValue::ActionRegist>;
template struct MessageValue::Convert<MessageValue::ActionSys>;

}
