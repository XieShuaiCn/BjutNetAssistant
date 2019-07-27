#include "BjutNet.h"
#include <assert.h>
#include <QDateTime>
#include "ServiceBridge.h"

namespace bna {
namespace gui {

BjutNet::BjutNet()
{
}

BjutNet::~BjutNet()
{
    stop();
    wait();
    if(m_bridge){
        try{
            delete m_bridge;
            m_bridge = nullptr;
        }catch(...){ }
    }
}

void BjutNet::queueAction(Action act, void *data)
{
    m_mtxAction.lock();
    m_lstAction.emplace_back(act, data);
    m_mtxAction.unlock();
}

#define SEND_ACT_AND_UPDATE_MESSAGE(func)                     \
    if(func){ emit updateMessage("[ OK ] " + strMsg);}        \
    else{ emit updateMessage("[FAIL] " + strMsg); }

void BjutNet::run()
{
    m_bRun.store(true);

    m_bridge = new ServiceBridge;
    assert(m_bridge!=nullptr);

    QString strMsg = "Connect to damon.";
    while(true){
        if(m_bridge->sendSYN()){
            emit updateMessage("[ OK ] " + strMsg);
            break;
        }
        else{
            emit updateMessage("[Fail] " + strMsg);
        }
        this->msleep(1000);
    }
    QTime tmRefresh;
    m_mtxAction.lock();
    m_lstAction.emplace_front(GET_NET_INFO, nullptr);
    m_mtxAction.unlock();
    tmRefresh.start();
    //
    ParamAccount *param_a = nullptr;
    ParamInt *param_i = nullptr;
    ParamInt *param_b = nullptr;
    //
    while(m_bRun.load()){
        // milliseconds
        if(tmRefresh.elapsed() > 10000 && (m_lstAction.empty() || m_lstAction.front().first != GET_NET_INFO)){
            m_mtxAction.lock();
            m_lstAction.emplace_front(GET_NET_INFO, nullptr);
            m_mtxAction.unlock();
        }
        else if(m_lstAction.size()){
            m_mtxAction.lock();
            std::pair<Action, void*> act = m_lstAction.front();
            m_lstAction.pop_front();
            m_mtxAction.unlock();
            switch (act.first) {
            case LOAD_ACCOUNT:
                strMsg = "Load account.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendActLoadAccount());
                break;
            case SAVE_ACCOUNT:
                strMsg = "Save account.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendActSaveAccount());
                break;
            case LOGIN:
                strMsg = "Login bjut net.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendActLoginBjut());
                break;
            case LOGOUT:
                strMsg = "Logout bjut net.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendActLogoutBjut());
                break;
            case SET_ACCOUNT:
                strMsg = "Set new account.";
                param_a = static_cast<ParamAccount*>(act.second);
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendSetAccount(
                                                std::get<0>(*param_a),
                                                std::get<1>(*param_a),
                                                std::get<2>(*param_a)));
                delete param_a;
                param_a = nullptr;
                break;
            case BOOK_NEW_SERVICE:
                strMsg = "Book new service.";
                param_i = static_cast<ParamInt*>(act.second);
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendSetBookedService(std::get<0>(*param_i)));
                delete param_i;
                param_i = nullptr;
                break;
            case  OFFLINE_DEVICE:
                strMsg = "offline device.";
                param_i = static_cast<ParamInt*>(act.second);
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendSetOfflineDevice(std::get<0>(*param_i)));
                delete param_i;
                param_i = nullptr;
                break;
            case SET_AUTO_START:
                strMsg = "Set auto start.";
                param_b = static_cast<ParamInt*>(act.second);
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendSetAutoStart(std::get<0>(*param_b)));
                delete param_b;
                param_b = nullptr;
                break;
            case GET_NET_INFO:
                strMsg = "Get net info.";
                m_bridge->sendActRefreshNet();
                m_bridge->sendGetUsedFlow(m_nNetFlow);
                m_bridge->sendGetUsedTime(m_nNetTime);
                m_bridge->sendGetLeftFee(m_nNetFee);
                emit updateNetInfo(true, m_nNetFlow, m_nNetTime, m_nNetFee);
                tmRefresh.restart();
                break;
            case GET_ACCOUNT:
                strMsg = "Get account info.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetAccount(
                                                m_strAccountName, m_strAccountPassword, m_nLoginType));
                emit updateAccount(m_strAccountName, m_strAccountPassword, m_nLoginType);
                break;
            case GET_ONLINE_DEVICE:
                strMsg = "Get online devices.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetOnlineDevices(m_vecOnlineDevices));
                emit updateOnlineDevice(QVariant::fromValue(m_vecOnlineDevices));
                break;
            case GET_SERVICE_INFO:
                strMsg = "Get service info.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetFlowService(m_strServiceName, m_nTotalFlow));
                emit updateServiceInfo(m_strServiceName, m_nTotalFlow);
                break;
            case GET_REMOTE_VERSION:
                strMsg = "Get damon version.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetVersion(m_strRemoteVersion, m_nRemoteVersion));
                emit updateRemoteVersion(m_strRemoteVersion, m_nRemoteVersion);
                break;
            case GET_BOOKED_SERVICE:
                strMsg = "Get booked services.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetBookedService(m_strBookedService));
                emit updateBookedService(m_strBookedService);
                break;
            case GET_ALL_SERVICE:
                strMsg = "Get all services.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetAllServices(m_vecAllServices));
                emit updateAllServices(QVariant::fromValue(m_vecAllServices));
                break;
            case  GET_AUTO_START:
                strMsg = "Get auto start.";
                SEND_ACT_AND_UPDATE_MESSAGE(m_bridge->sendGetAutoStart(m_bAutoStart));
                emit updateAutoStart(m_bAutoStart);
                break;
            default:
                break;
            }
        }
        else{
            this->msleep(50);
        }
    }

    if(m_bridge){
        try{
            delete m_bridge;
            m_bridge = nullptr;
        }catch(...){ }
    }
}

#undef SEND_ACT_AND_UPDATE_MESSAGE

}
}
