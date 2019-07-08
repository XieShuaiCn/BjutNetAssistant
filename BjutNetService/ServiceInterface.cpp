#include "ServiceInterface.h"
#include <QDateTime>
#include <QUdpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include "common.h"
#include "BjutNet.h"
#include "MessageValue.h"
#include "Setting.h"

namespace bna{

const QByteArray g_acbENQ(1, CHAR_ENQ);
const QByteArray g_acbACK(1, CHAR_ACK);
const QByteArray g_acbNAK(1, CHAR_NAK);
const QByteArray g_acbSYN(1, CHAR_SYN);

inline QByteArray __ServiceInterface_ErrMsgToByteArray(const QString &msg, int seed)
{
    return QString("{\"type\":%1,\"msg\":\"%2\",\"seed\":%3}")
            .arg(MessageValue::ERR).arg(msg).arg(seed)
            .toUtf8();
}

inline QByteArray __ServiceInterface_AckDataToByteArray(const QString &data, int seed)
{
    return QString("{\"type\":%1,\"data\":%2,\"seed\":%3}")
            .arg(MessageValue::ACK).arg(data).arg(seed)
            .toUtf8();
}

inline QByteArray __ServiceInterface_PushDataToByteArray(int type, const QString &data, int seed)
{
    return QString("{\"type\":%1,\"data\":%2,\"seed\":%3}")
            .arg(type).arg(data).arg(seed)
            .toUtf8();
}

inline QByteArray __ServiceInterface_AckSuccToByteArray(int seed, bool succ = true)
{
    return QString("{\"type\":%1,\"succ\":%2,\"seed\":%3}")
            .arg(MessageValue::ACK).arg(succ?1:0).arg(seed)
            .toUtf8();
}

ServiceInterface::ServiceInterface(BjutNet *bjutNet)
    : m_remoteHost(QHostAddress::Null),
      m_remotePort(0),
      m_bjutnet(bjutNet)
{
    Q_ASSERT(bjutNet!=nullptr);
    m_socket = new QUdpSocket();
}

bool ServiceInterface::Initilize()
{
    // bind port and listen
    if(false == m_socket->bind(QHostAddress::Any, MessageValue::ServerPort))
    {
        qDebug() << "Can't bind port: " << MessageValue::ServerPort << endl;
        qDebug() << m_socket->errorString() << endl;
        return false;
    }
    connect(m_socket, &QUdpSocket::readyRead, this, &ServiceInterface::ReadSocketData);
    connect(m_bjutnet, &BjutNet::message, this, &ServiceInterface::PushMessage);
    return true;
}

void ServiceInterface::ReadSocketData()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray arrData;
        QHostAddress address;
        quint16 port;
        // resize data memory
        arrData.resize(m_socket->bytesAvailable());
        // read DATA
        m_socket->readDatagram(arrData.data(), arrData.size(), &address, &port);
        if(arrData.size() == 1) {
            if(arrData == g_acbACK) {
                // recive 'ACK'
#ifdef BUILD_DEVELOP
                qDebug() << "==> ACK" << endl;
#endif
            }
            else if(arrData == g_acbSYN) {
                m_socket->writeDatagram(g_acbACK, address, port);
#ifdef BUILD_DEVELOP
                qDebug() << "==> SYN" << endl;
                qDebug() << "<== ACK" << endl;
#endif
            }
            else if(arrData == g_acbENQ){// recive 'ENQ', reply 'Version'
                QChar chVersion(MessageValue::Version);
                QByteArray array;
                array.append(chVersion);
                m_socket->writeDatagram(array, address, port);
#ifdef BUILD_DEVELOP
                qDebug() << "==> ENQ" << endl;
                qDebug() << "<== " << chVersion << endl;
#endif
            }
            else{
                m_socket->writeDatagram(g_acbNAK, address, port);
            }
        }
        else if(arrData.size() > 1){
            // reply 'ACK'
            //m_socket->writeDatagram(cbACK, address, port);
#ifdef BUILD_DEVELOP
                qDebug() << "==> " << arrData << endl;
#endif
            ProcessCommand(arrData, address, port);
        }
    }
}

void ServiceInterface::ProcessCommand(const QByteArray &cmd, const QHostAddress &address, quint16 port)
{
    using MsgCvtToType = MessageValue::ConvertToType;
    using MsgCvtToSync = MessageValue::ConvertToActSync;
    using MsgCvtToAct = MessageValue::ConvertToActAct;
    using MsgCvtToGet = MessageValue::ConvertToActGet;
    using MsgCvtToSet = MessageValue::ConvertToActSet;
    auto &lgn = m_bjutnet->getWebLgn();
    auto &jfself = m_bjutnet->getWebJfself();

    // parse json
    QJsonParseError json_error;
    QJsonDocument jdoc = QJsonDocument::fromJson(cmd, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        auto jo = jdoc.object();
        if(jo.contains("type") && jo.contains("act") && jo.contains("seed")){
            //m_socket->writeDatagram(g_acbACK, address, port);
            // process instructions
            QByteArray buffer;
            QString strTemp;
            auto seed = jo["seed"].toInt();
            auto type = MsgCvtToType::To(jo["type"].toInt());
            // SYNC
            if(type == MessageValue::SYNC) {
                auto act = MsgCvtToSync::To(jo["act"].toInt());
                switch (act) {
                case MessageValue::HELLO:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::REGIST_MESSAGE:
                    if(!m_remoteHost.isNull() && m_remotePort!=0){
                        PushMessage(QDateTime::currentDateTime(),
                                    QString("Another device required to register message."
                                            "No more pushed messages from now on."));
                    }
                    m_remoteHost = address;
                    m_remotePort = port;
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::REGIST_STATUS:
                    if(!m_remoteHost.isNull() && m_remotePort!=0){
                        PushMessage(QDateTime::currentDateTime(),
                                    QString("Another device required to register status."
                                            "No more status notification from now on."));
                    }
                    m_remoteHost = address;
                    m_remotePort = port;
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                default:
                    buffer = __ServiceInterface_ErrMsgToByteArray("Bad act", seed);
                    break;
                }
            }
            // action
            else if(type == MessageValue::ACT) {
                auto act = MsgCvtToAct::To(jo["act"].toInt());
                switch (act) {
                case MessageValue::ACT_LOAD_ACCOUNT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                            m_bjutnet->loadAccount());
                    break;
                case MessageValue::ACT_SAVE_ACCOUNT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                            m_bjutnet->saveAccount());
                    break;
                case MessageValue::ACT_LOGIN_BJUT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                    lgn.login() && m_bjutnet->start_monitor());
                    break;
                case MessageValue::ACT_LOGOUT_BJUT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                    m_bjutnet->stop_monitor() && lgn.logout());
                    break;
                case MessageValue::ACT_LOGIN_JFSELF:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                    jfself.login());
                    break;
                case MessageValue::ACT_LOGOUT_JFSELF:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_REFRESH_NET:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                lgn.checkLoginStatus());
                    break;
                case MessageValue::ACT_REFRESH_JFSELF_ACCOUNT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                jfself.refreshAccount());
                    break;
                case MessageValue::ACT_REFRESH_ONLINE:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                jfself.refreshOnline());
                    break;
                case MessageValue::ACT_ENTER_DEBUG_MODE:
                    g_bAppDebug = true;
                    buffer = __ServiceInterface_AckSuccToByteArray(seed, g_bAppDebug);
                    break;
                case MessageValue::ACT_LEAVE_DEBUG_MODE:
                    g_bAppDebug = false;
                    buffer = __ServiceInterface_AckSuccToByteArray(seed, !g_bAppDebug);
                    break;
                default:
                    buffer = __ServiceInterface_ErrMsgToByteArray("Bad act", seed);
                    break;
                }
            }
            // get value
            else if(type == MessageValue::GET) {
                auto act = MsgCvtToGet::To(jo["act"].toInt());
                switch (act) {
                case MessageValue::GET_VERSION:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":\"%1\",\"n\":%2}")
                                .arg(BNA_VERSION).arg(BNA_INNER_VERSION),
                                seed);
                    break;
                case MessageValue::GET_ACCOUNT:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"n\":\"%1\",\"p\":\"%2\",\"t\":%3}")
                                .arg(m_bjutnet->getAccount())
                                .arg(m_bjutnet->getPassword())
                                .arg(int(m_bjutnet->getLoginType())),
                                seed);
                    break;
                case MessageValue::GET_USED_FLOW:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":%1}").arg(lgn.getFlow()),
                                seed);
                    break;
                case MessageValue::GET_USED_TIME:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":%1}").arg(lgn.getTime()),
                                seed);
                    break;
                case MessageValue::GET_LEFT_FEE:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":%1}").arg(lgn.getFee()),
                                seed);
                    break;
                case MessageValue::GET_ALL_FLOW:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":%1}").arg(jfself.getTotalFlow()),
                                seed);
                    break;
                case MessageValue::GET_DEVICE_ONLINE:
                    {
                        QVector<OnlineClientInfo> list = jfself.getOnlineClient();
                        strTemp = "[";
                        for(const auto &dev : list){
                            strTemp += QString("{\"id\":%1,\"ipv4\":\"%2\",\"ipv6\":\"%3\"},")
                                    .arg(dev.nID).arg(dev.strIPv4).arg(dev.strIPv6);
                        }
                        if(strTemp.endsWith(',')){
                            strTemp.remove(strTemp.size()-1, 1);
                        }
                        strTemp+="]";
                        buffer = __ServiceInterface_AckDataToByteArray(strTemp, seed);
                    }
                    break;
                case MessageValue::GET_FLOW_SERVICE:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"n\":\"%1\", \"v\":%2}")
                                .arg(jfself.getServiceName())
                                .arg(jfself.getTotalFlow()),
                                seed);
                    break;
                case MessageValue::GET_BOOKED_SERVICE:
                    if(!jfself.refreshBookService()){
                        buffer = __ServiceInterface_ErrMsgToByteArray("Fail to refresh all services.", seed);
                        break;
                    }
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"n\":\"%1\"}")
                                .arg(jfself.getCurrentBookService()),
                                seed);
                    break;
                case MessageValue::GET_ALL_SERVICES:
                    {
                        if(!jfself.refreshBookService()){
                            buffer = __ServiceInterface_ErrMsgToByteArray("Fail to refresh all services.", seed);
                            break;
                        }
                        QVector<ServiceInfo> list = jfself.getBookServiceList();
                        strTemp = "[";
                        for(const ServiceInfo &sv : list){
                            strTemp += QString("{\"id\":%1,\"n\":\"%2\",\"m\":\"%3\"},")
                                    .arg(sv.nID).arg(sv.strName).arg(sv.strDescription);
                        }
                        if(strTemp.endsWith(',')){
                            strTemp.remove(strTemp.size()-1, 1);
                        }
                        strTemp+="]";
                        buffer = __ServiceInterface_AckDataToByteArray(strTemp, seed);
                    }
                    break;
                case MessageValue::GET_AUTO_START:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":%1}")
                                .arg(static_cast<int>(Setting::getAutoRun())),
                                seed);
                    break;
                default:
                    buffer = __ServiceInterface_ErrMsgToByteArray("Bad act", seed);
                    break;
                }
            }
            // set value
            else if(type == MessageValue::SET) {
                auto act = MsgCvtToSet::To(jo["act"].toInt());
                switch(act){
                case MessageValue::SET_ACCOUNT:
                    if(jo.contains("data")){
                        //{\"n\":\"%1\",\"p\":\"%2\",\"t\":%3}
                        QJsonObject data = jo["data"].toObject();
                        if(data.contains("n")){
                            auto name = data["n"].toString();
                            m_bjutnet->setAccount(name);
                        }
                        if(data.contains("p")){
                            auto passwd = data["p"].toString();
                            m_bjutnet->setPassword(passwd);
                        }
                        if(data.contains("t")){
                            auto type = data["t"].toInt();
                            m_bjutnet->setLoginType(type);
                        }
                        m_bjutnet->synchronizeAccount();
                        buffer = __ServiceInterface_AckSuccToByteArray(seed);
                        break;
                    }
                    buffer = __ServiceInterface_AckSuccToByteArray(seed, false);
                    break;
                case MessageValue::SET_BOOK_SERVICE:
                    if(jo.contains("data")){
                        QJsonObject data = jo["data"].toObject();
                        if(data.contains("id")){
                            buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                    jfself.submitBookService(data["id"].toInt()));
                            break;
                        }
                    }
                    buffer = __ServiceInterface_AckSuccToByteArray(seed, false);
                    break;
                case MessageValue::SET_OFFLINE_DEVICE:
                    if(jo.contains("data")){
                        QJsonObject data = jo["data"].toObject();
                        if(data.contains("id")){
                            buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                    jfself.toOffline(data["id"].toInt()));
                            break;
                        }
                    }
                    buffer = __ServiceInterface_AckSuccToByteArray(seed, false);
                    break;
                case MessageValue::SET_AUTO_START:
                    if(jo.contains("data")){
                        QJsonObject data = jo["data"].toObject();
                        if(data.contains("v")){
                            buffer = __ServiceInterface_AckSuccToByteArray(seed,
                                    Setting::setAutoRun(data["v"].toInt() != 0));
                            break;
                        }
                    }
                    buffer = __ServiceInterface_AckSuccToByteArray(seed, false);
                    break;
                default:
                    buffer = __ServiceInterface_ErrMsgToByteArray("Bad act", seed);
                    break;
                }
            }
            // error command
            else{
                buffer = __ServiceInterface_ErrMsgToByteArray("Bad type", seed);
            }
#ifdef BUILD_DEVELOP
            qDebug() << "<==" << buffer << endl;
#endif
            m_socket->writeDatagram(buffer.data(), buffer.size(), address, port);
        }
        else {
    #ifdef BUILD_DEVELOP
            qDebug() << "Incomplete json content: " << cmd << endl;
    #endif
            m_socket->writeDatagram(g_acbNAK, address, port);
        }
    }
    else {
#ifdef BUILD_DEVELOP
        qDebug() << "Error json format: " << cmd << endl;
#endif
        m_socket->writeDatagram(g_acbNAK, address, port);
    }
}

void ServiceInterface::PushMessage(const QDateTime& time, const QString& info)
{
    if(m_remoteHost.isNull()) return;

    int seed = qrand();
    QByteArray buffer = __ServiceInterface_PushDataToByteArray(
                MessageValue::MESSAGE_CHANGE,
                QString("{\"t\":%1,\"m\":\"%2\"}").arg(time.toTime_t()).arg(info),
                seed);
    m_socket->writeDatagram(buffer.data(), buffer.size(), m_remoteHost, m_remotePort);
}

}

