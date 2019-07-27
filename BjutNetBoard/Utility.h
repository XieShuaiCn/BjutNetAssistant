#ifndef BNA_GUI_UTILITY_H
#define BNA_GUI_UTILITY_H

#include "common.h"
#include <QHostAddress>
#include <QVector>

namespace bna{
namespace gui {

bool ListLocalIpAddress(QVector<QHostAddress> &addrs);

}
}

#endif // BNA_GUI_UTILITY_H
