#include "ServiceInterface.h"
#include <QUdpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include "Version.h"
#include "MessageValue.h"

#ifndef QT_DEBUG
#define QT_DEBUG
#endif

namespace bna{

const QByteArray g_acbENQ(1, CHAR_ENQ);
const QByteArray g_acbACK(1, CHAR_ACK);
const QByteArray g_acbNAK(1, CHAR_NAK);
const QByteArray g_acbSYN(1, CHAR_SYN);

ServiceInterface::ServiceInterface(int argc, char **argv)
{
    m_app = new QCoreApplication(argc, argv);
    Q_ASSERT(m_app!=nullptr);
    m_socket = new QUdpSocket();
}

bool ServiceInterface::Initilize()
{
    // bind port and listen
    if(false == m_socket->bind(QHostAddress::Any, MessageValue::ServerPort))
    {
        qDebug() << "Can't bind port:" << MessageValue::ServerPort << endl;
        return false;
    }
    connect(m_socket, &QUdpSocket::readyRead, this, &ServiceInterface::ReadSocketData);
    return true;
}

int ServiceInterface::Run()
{
    if(Initilize()){
        return m_app->exec();
    }
    else{
        qDebug() << "Fail to initialize service. Exit now." << endl;
        m_app->exit(0);
        return 0;
    }
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
#ifdef QT_DEBUG
                qDebug() << "==> ACK" << endl;
#endif
            }
            else if(arrData == g_acbSYN) {
                m_socket->writeDatagram(g_acbACK, address, port);
#ifdef QT_DEBUG
                qDebug() << "==> SYN" << endl;
                qDebug() << "<== ACK" << endl;
#endif
            }
            else if(arrData == g_acbENQ){// recive 'ENQ', reply 'Version'
                QChar chVersion(MessageValue::Version);
                QByteArray array;
                array.append(chVersion);
                m_socket->writeDatagram(array, address, port);
#ifdef QT_DEBUG
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
#ifdef QT_DEBUG
                qDebug() << "==> " << arrData << endl;
#endif
            ProcessCommand(arrData, address, port);
        }
    }
}

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

inline QByteArray __ServiceInterface_AckSuccToByteArray(int seed)
{
    return QString("{\"type\":%1,\"succ\":1,\"seed\":%2}")
            .arg(MessageValue::ACK).arg(seed)
            .toUtf8();
}

void ServiceInterface::ProcessCommand(const QByteArray &cmd, const QHostAddress &address, quint16 port)
{
    using MsgCvtToType = MessageValue::ConvertToType;
    using MsgCvtToSync = MessageValue::ConvertToActSync;
    using MsgCvtToAct = MessageValue::ConvertToActAct;
    using MsgCvtToGet = MessageValue::ConvertToActGet;
    using MsgCvtToSet = MessageValue::ConvertToActSet;

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
            auto seed = jo["seed"].toInt();
            auto type = MsgCvtToType::To(jo["type"].toInt());
            // SYNC
            if(type == MessageValue::SYNC) {
                auto act = MsgCvtToSync::To(jo["act"].toInt());
                switch (act) {
                case MessageValue::HELLO:
                    buffer = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
                            .arg(static_cast<int>(MessageValue::SYNC))
                            .arg(static_cast<int>(MessageValue::HELLO_ACK))
                            .arg(static_cast<int>(seed))
                            .toLocal8Bit();
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
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_SAVE_ACCOUNT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_LOGIN_BJUT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_LOGOUT_BJUT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_LOGIN_JFSELF:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_LOGOUT_JFSELF:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_REFRESH_NET:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_REFRESH_JFSELF_ACCOUNT:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::ACT_REFRESH_ONLINE:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
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
                                QString("{\"n\":\"S123456789\",\"p\":\"123456\",\"t\":2}"),
                                seed);
                    break;
                case MessageValue::GET_USED_FLOW:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":11244670}"),
                                seed);
                    break;
                case MessageValue::GET_USED_TIME:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":1276}"),
                                seed);
                    break;
                case MessageValue::GET_LEFT_FEE:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":2300}"),
                                seed);
                    break;
                case MessageValue::GET_ALL_FLOW:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"v\":12288}"),
                                seed);
                    break;
                case MessageValue::GET_DEVICE_ONLINE:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{"
                                 "\"id1\":111,\"d1v4\":\"172.21.15.11\",\"d1v6\":\"2001:da8:216:210f:8968:7827:8abb:ffb4\","
                                 "\"id2\":222,\"d2v4\":\"172.21.15.10\",\"d2v6\":\"2001:da8:216:210f:8968:7827:8abb:ffb5\""
                                 "}"), seed);
                    break;
                case MessageValue::GET_FLOW_SERVICE:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"n\":\"Default service\", \"v\":12288}"),
                                seed);
                    break;
                case MessageValue::GET_BOOKED_SERVICE:
                    buffer = __ServiceInterface_AckDataToByteArray(
                                QString("{\"n\":\"Default service\"}"),
                                seed);
                    break;
                case MessageValue::GET_ALL_SERVICES:
                    buffer = __ServiceInterface_AckDataToByteArray(QString("["
                                 "{\"id\":1,\"n\":\"Default service\",\"m\":\"12G\"},"
                                 "{\"id\":2,\"n\":\"20 Yuan service\",\"m\":\"25G\"},"
                                 "{\"id\":3,\"n\":\"25 Yuan service\",\"m\":\"30G\"}"
                                 "]"), seed);
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
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
                    break;
                case MessageValue::SET_BOOK_SERVICE:
                    buffer = __ServiceInterface_AckSuccToByteArray(seed);
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
#ifdef QT_DEBUG
            qDebug() << "<==" << buffer << endl;
#endif
            m_socket->writeDatagram(buffer, address, port);
        }
        else {
    #ifdef QT_DEBUG
            qDebug() << "Incomplete json content: " << cmd << endl;
    #endif
            m_socket->writeDatagram(g_acbNAK, address, port);
        }
    }
    else {
#ifdef QT_DEBUG
        qDebug() << "Error json format: " << cmd << endl;
#endif
        m_socket->writeDatagram(g_acbNAK, address, port);
    }
}

}

