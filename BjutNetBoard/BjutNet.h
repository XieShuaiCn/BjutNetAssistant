#ifndef BJUTNET_H
#define BJUTNET_H

#include <vector>
#include <list>
#include <atomic>
#include <array>
#include <tuple>
#include <QObject>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QHostAddress>

namespace bna {
namespace gui {
class ServiceBridge;

class BjutNet : public QThread
{
    Q_OBJECT
public:
    enum Action{
        LOAD_ACCOUNT,
        SAVE_ACCOUNT,
        SET_ACCOUNT,
        GET_ACCOUNT,
        LOGIN,
        LOGOUT,
        BOOK_NEW_SERVICE,
        REFRESH_ONLINE_DEVICE,
        GET_ONLINE_DEVICE,
        FORCE_OFFLINE_DEVICE,
        OFFLINE_DEVICE,
        ONLINE_DEVICE,
        REFRESH_NET_INFO,
        GET_NET_INFO,
        GET_SERVICE_INFO,
        GET_REMOTE_VERSION,
        GET_BOOKED_SERVICE,
        GET_ALL_SERVICE,
        SET_AUTO_START,
        GET_AUTO_START
    };

    using ParamAccount = std::tuple<QString, QString, int>;
    using ParamInt = std::tuple<int>;
    using ParamBool = std::tuple<bool>;
    typedef std::vector<std::tuple<int, QString, QString>> TypeAllServices;
    typedef std::vector<std::array<QString, 4>> TypeOnlineDevices;

    BjutNet();

    ~BjutNet();

    void queueAction(Action act, void *data=nullptr);

    void sendLoadAccount();

    void sendSaveAccount();

    void sendLogin();

    void sendLogout();

    void sendRefreshOnlineDevices();

    void sendRefreshNetInfo();

    void sendNewAccount(QString name, QString passwd, int type);

    void sendBookService(int id);

    void sendForceOfflineDevice(int id);

    void sendOfflineDevice(QString ip_addr);

    void sendOnlineDevice(QString ip_addr);

    void sendAutoStart(bool autostart = true);

    void requireAccount();

    void requireNetStatus();

    void requireNetInfo();

    void requireOnlineDevices();

    void requireServiveInfo();

    void requireRemoteVersion();

    void requireBookedService();

    void requireAlldService();

    void requireIsAutoStart();
    //
    void getNetInfo(bool &online, int &flow, int &time, int &fee) const;
    // retrun {online, flow, time, fee}
    std::tuple<bool, int, int, int> getNetInfo()  const;
    //
    bool getNetOnline() const;
    //
    int getUsedFlow() const;
    //
    int getUsedTime() const;
    //
    int getLeftFee() const;
    //
    const std::vector<std::array<QString, 4>> & getOnlineDevice() const;
    //
    const QString &getServiceName() const;
    //
    int getServiceFlow() const;
    //
    const std::vector<std::tuple<int, QString, QString>> &getAllServices() const;
    //
    bool getAutoStart() const;
    //
    const QString &getBookedService() const;
    //
    const QString &getRemoteVersionString() const;
    //
    int getRemoteVersion() const;

    const QString &getVersionString() const;

    int getVersion() const;

    void stop();

    void pause();

    bool isPaused();

    void resume();

    void stopDaemon();

    void restartDaemon();

signals:
    // recive message
    void updateMessage(const QString &msg);

    void updateAccount(const QString &name, const QString &passwd, int type);
    // recive the info of bjut net
    void updateNetInfo(bool online, int flow, int time, int fee);
    // recive the list of online devices
    void updateOnlineDevice(const QVariant &devices);
    // recive the info of account service
    void updateServiceInfo(const QString &name, int totalFlow);
    // recive the version of remote service
    void updateRemoteVersion(const QString &version, int inner_ver);
    //
    void updateAutoStart(bool autostart);
    //
    void updateAllServices(const QVariant &services);
    //
    void updateBookedService(const QString &name);
protected:
    virtual void run() override;
private:

    QString m_strAccountName;
    QString m_strAccountPassword;
    int m_nLoginType = 0;
    bool m_bOnline = false;
    int m_nNetFlow = 0;
    int m_nNetTime = 0;
    int m_nNetFee = 0;
    bool m_bLogin = false;
    int m_nTotalFlow = 0;
    QString m_strServiceName;
    QString m_strBookedService;
    TypeAllServices m_vecAllServices;
    TypeOnlineDevices m_vecOnlineDevices;
    bool m_bAutoStart = false;
    QString m_strRemoteVersion;
    int m_nRemoteVersion = 0;

    std::atomic_bool m_bRun;
    std::atomic_bool m_bPause;
    std::atomic_bool m_bPaused;
    std::list<std::pair<Action, void*>> m_lstAction;
    QMutex m_mtxAction;
    ServiceBridge *m_bridge;
};
}}

Q_DECLARE_METATYPE(bna::gui::BjutNet::TypeAllServices);
Q_DECLARE_METATYPE(bna::gui::BjutNet::TypeOnlineDevices);

/////////////////////////////////////////
namespace bna {
namespace gui {

inline void BjutNet::stop()
{
    m_bRun.store(false);
}

inline void BjutNet::pause()
{
    m_bPause.store(true);
}

inline bool BjutNet::isPaused()
{
    return m_bPaused.load();
}

inline void BjutNet::resume()
{
    m_bPause.store(false);
}

inline void BjutNet::sendLoadAccount()
{
    queueAction(LOAD_ACCOUNT);
}

inline void BjutNet::sendSaveAccount()
{
    queueAction(SAVE_ACCOUNT);
}

inline void BjutNet::sendLogin()
{
    queueAction(LOGIN);
}

inline void BjutNet::sendLogout()
{
    queueAction(LOGOUT);
}

inline void BjutNet::sendRefreshOnlineDevices()
{
    queueAction(REFRESH_ONLINE_DEVICE);
}

inline void BjutNet::sendRefreshNetInfo()
{
    queueAction(REFRESH_NET_INFO);
}

inline void BjutNet::sendNewAccount(QString name, QString passwd, int type)
{
    queueAction(SET_ACCOUNT, new ParamAccount{name, passwd, type});
}

inline void BjutNet::sendBookService(int id)
{
    queueAction(BOOK_NEW_SERVICE, new ParamInt{id});
}

inline void BjutNet::sendForceOfflineDevice(int id)
{
    queueAction(FORCE_OFFLINE_DEVICE, new ParamInt{id});
}

inline void BjutNet::sendOnlineDevice(QString ip_addr)
{
    queueAction(ONLINE_DEVICE, new QHostAddress(ip_addr));
}

inline void BjutNet::sendOfflineDevice(QString ip_addr)
{
    queueAction(OFFLINE_DEVICE, new QHostAddress(ip_addr));
}

inline void BjutNet::sendAutoStart(bool autostart)
{
    queueAction(SET_AUTO_START, new ParamBool{autostart});
}

inline void BjutNet::requireAccount()
{
    queueAction(GET_ACCOUNT);
}

inline void BjutNet::requireNetStatus()
{
    queueAction(GET_NET_INFO);
}

inline void BjutNet::requireNetInfo()
{
    queueAction(GET_NET_INFO);
}

inline void BjutNet::requireOnlineDevices()
{
    queueAction(GET_ONLINE_DEVICE);
}

inline void BjutNet::requireServiveInfo()
{
    queueAction(GET_SERVICE_INFO);
}

inline void BjutNet::requireRemoteVersion()
{
    queueAction(GET_REMOTE_VERSION);
}

inline void BjutNet::requireBookedService()
{
    queueAction(GET_BOOKED_SERVICE);
}

inline void BjutNet::requireAlldService()
{
    queueAction(GET_ALL_SERVICE);
}

inline void BjutNet::requireIsAutoStart()
{
    queueAction(GET_AUTO_START);
}
//
inline void BjutNet::getNetInfo(bool &online, int &flow, int &time, int &fee) const
{
    online = getNetOnline();
    flow = getUsedFlow();
    time = getUsedTime();
    fee = getLeftFee();
}
// retrun {online, flow, time, fee}
inline std::tuple<bool, int, int, int> BjutNet::getNetInfo()  const
{
    return {getNetOnline(), getUsedFlow(), getUsedTime(), getLeftFee()};
}

//
inline bool BjutNet::getNetOnline() const
{
    return m_bOnline;
}

//
inline int BjutNet::getUsedFlow() const
{
    return m_nNetFlow;
}

//
inline int BjutNet::getUsedTime() const
{
    return m_nNetTime;
}

//
inline int BjutNet::getLeftFee() const
{
    return m_nNetFee;
}

//
inline const std::vector<std::array<QString, 4>> &BjutNet::getOnlineDevice() const
{
    return m_vecOnlineDevices;
}

//
inline const QString &BjutNet::getServiceName() const
{
    return m_strServiceName;
}

//
inline int BjutNet::getServiceFlow() const
{
    return m_nTotalFlow;
}

//
inline const std::vector<std::tuple<int, QString, QString>> &BjutNet::getAllServices() const
{
    return m_vecAllServices;
}

//
inline bool BjutNet::getAutoStart() const
{
    return m_bAutoStart;
}

//
inline const QString &BjutNet::getBookedService() const
{
    return m_strBookedService;
}

//
inline const QString &BjutNet::getRemoteVersionString() const
{
    return m_strRemoteVersion;
}

//
inline int BjutNet::getRemoteVersion() const
{
    return m_nRemoteVersion;
}


}
}
#endif // BJUTNET_H
