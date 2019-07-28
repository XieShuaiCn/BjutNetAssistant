#include "ServiceBridge.h"
#include <QUdpSocket>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QException>
#include "Utility.h"
#include "../BjutNetService/MessageCoder.h"

namespace bna {
namespace gui {

bool ServiceBridge::doSendAndReceive(const QString &sdata, QString &rdata)
{
    rdata.clear();
    while(m_socket.bytesAvailable()){
        m_socket.readAll();
    }
    QByteArray arr;
    QByteArray sbuf = sdata.toUtf8();
    QByteArray rbuf;
    int sz = MessageCoder::Encrypt(sbuf.data(), sbuf.length(), nullptr, 0);
    arr.resize(sz);
    sz = MessageCoder::Encrypt(sbuf.data(), sbuf.length(), arr.data(), arr.length());
    if(sz != arr.length()){
        return false;
    }
    if(!m_socket.writeDatagram(arr, m_host, m_port)) {
        return false;
    }
    if(m_socket.waitForReadyRead(10000)){
        QHostAddress address;
        quint16 port;
        arr.resize(m_socket.bytesAvailable());
        if(m_socket.readDatagram(arr.data(), arr.size(), &address, &port) <= 0) {
            return false;
        }
        if(port == m_port && m_host.isEqual(address, QHostAddress::TolerantConversion)){
            if(arr.size() > 2){
                sz = MessageCoder::Decrypt(arr.data(), arr.length(), nullptr, 0);
                rbuf.resize(sz);
                sz = MessageCoder::Decrypt(arr.data(), arr.length(), rbuf.data(), rbuf.length());
                rdata = QString::fromUtf8(rbuf);
                return sz == rbuf.length();
            }
            else{
                rdata.append(arr);
            }
        }
    }
    return false;
}

ServiceBridge::ServiceBridge()
    : m_host("127.0.0.1"),
      m_port(6350),
      m_nMsgVersion(MessageValue::Version)
{
    qsrand(uint(QDateTime::currentMSecsSinceEpoch()));
}

bool ServiceBridge::setHost(const QString &host)
{
    return m_host.setAddress(host);
}

const QString ServiceBridge::getHost()
{
    return m_host.toString();
}

void ServiceBridge::getMyAddress(QVector<QHostAddress> &addrs)
{
    ListLocalIpAddress(addrs);
}

bool ServiceBridge::sendSYN()
{
    char ch(CHAR_SYN);
    while(m_socket.bytesAvailable()){
        m_socket.readAll();
    }
    if(1 != m_socket.writeDatagram(&ch, 1, m_host, m_port)) {
        return false;
    }
    if(m_socket.waitForReadyRead(100)){
        QByteArray arr;
        arr.resize(m_socket.bytesAvailable()+1);
        if(1!=m_socket.readDatagram(arr.data(), arr.size())) {
            return false;
        }
        return arr[0] == CHAR_ACK;
    }
    return false;
}

bool ServiceBridge::sendENQ()
{
    char ch(CHAR_ENQ);
    while(m_socket.bytesAvailable()){
        m_socket.readAll();
    }
    if(1 != m_socket.writeDatagram(&ch, 1, m_host, m_port)) {
        return false;
    }
    if(m_socket.waitForReadyRead(100)){
        QByteArray arr;
        arr.resize(m_socket.bytesAvailable()+1);
        if(1!=m_socket.readDatagram(arr.data(), arr.size())) {
            return false;
        }
        m_nMsgVersion = static_cast<int>(static_cast<unsigned char>(arr[0]));
        return m_nMsgVersion == MessageValue::Version;
    }
    return false;
}

bool ServiceBridge::sendSyncHello()
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::SYNC).arg(MessageValue::HELLO).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendAct_common(MessageValue::ActionAct type)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::ACT).arg(type).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendGetVersion(QString &var, int &inner_ver)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_VERSION).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("v") && obj.contains("n")){
                var = obj["v"].toString();
                inner_ver = obj["n"].toInt();
                return true;
            }
        }
    }
    return false;
}

bool ServiceBridge::sendGetAccount(QString &name, QString &password, int &type)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_ACCOUNT).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("n") && obj.contains("p") && obj.contains("t")){
                name = obj["n"].toString();
                password = obj["p"].toString();
                type = obj["t"].toInt();
                return true;
            }
        }
    }
    return false;
}
bool ServiceBridge::sendGetUsedFlow(int &value)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_USED_FLOW).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("v")){
                value = obj["v"].toInt();
                return true;
            }
        }
    }
    return false;
}
bool ServiceBridge::sendGetUsedTime(int &value)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_USED_TIME).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("v")){
                value = obj["v"].toInt();
                return true;
            }
        }
    }
    return false;
}
bool ServiceBridge::sendGetLeftFee(int &value)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_LEFT_FEE).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("v")){
                value = obj["v"].toInt();
                return true;
            }
        }
    }
    return false;
}
bool ServiceBridge::sendGetAllFlow(int &value)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_ALL_FLOW).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("v")){
                value = obj["v"].toInt();
                return true;
            }
        }
    }
    return false;
}
bool ServiceBridge::sendGetOnlineDevices(std::vector<std::array<QString, 4>> &devices)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_DEVICE_ONLINE).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isArray()){
            devices.clear();
            QJsonArray arr = jdata.toArray();
            for(const auto &a : arr){
                if(a.isObject()){
                    QJsonObject obj = a.toObject();
                    auto id = obj["id"].toInt();
                    auto ipv4 = obj["ipv4"].toString();
                    auto ipv6 = obj["ipv6"].toString();
                    devices.emplace_back(std::array<QString, 4>{QString().setNum(id), ipv4, ipv6, QString("")});
                }
            }
            return true;
        }
    }
    return false;
}
bool ServiceBridge::sendGetFlowService(QString &name, int &totalFlow)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_FLOW_SERVICE).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("n")){
                name = obj["n"].toString("None");
            }
            if(obj.contains("v")){
                totalFlow = obj["v"].toInt(0);
            }
            return true;
        }
    }
    return false;
}
bool ServiceBridge::sendGetBookedService(QString &name)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_BOOKED_SERVICE).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("n")){
                name = obj["n"].toString("None");
                return true;
            }
        }
    }
    return false;
}
bool ServiceBridge::sendGetAllServices(std::vector<std::tuple<int, QString, QString>> &services)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_ALL_SERVICES).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isArray()){
            QJsonArray arr = jdata.toArray();
            for(const auto &a : arr){
                if(a.isObject()){
                    QJsonObject obj = a.toObject();
                    auto id = obj["id"].toInt();
                    auto name = obj["n"].toString();
                    auto msg = obj["m"].toString();
                    services.emplace_back(id, name, msg);
                }
            }
            return true;
        }
    }
    return false;
}
bool ServiceBridge::sendGetAutoStart(bool &autorun)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"seed\":%3}")
            .arg(MessageValue::GET).arg(MessageValue::GET_AUTO_START).arg(seed);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    QJsonValue jdata;
    if(parseJson(buf, seed, jdata))
    {
        if(jdata.isObject()){
            QJsonObject obj = jdata.toObject();
            if(obj.contains("v")){
                autorun = static_cast<bool>(obj["v"].toInt());
                return true;
            }
        }
    }
    return false;
}

bool ServiceBridge::sendSetAccount(const QString name, const QString passwd, int type)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"data\":{\"n\":\"%4\",\"p\":\"%5\",\"t\":%6},\"seed\":%3}")
            .arg(MessageValue::SET).arg(MessageValue::SET_ACCOUNT).arg(seed)
            .arg(name).arg(passwd).arg(type);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendSetBookedService(int id)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"data\":{\"id\":\"%4\"},\"seed\":%3}")
            .arg(MessageValue::SET).arg(MessageValue::SET_BOOK_SERVICE).arg(seed)
            .arg(id);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendSetOfflineDevice(int id)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"data\":{\"id\":%4},\"seed\":%3}")
            .arg(MessageValue::SET).arg(MessageValue::SET_OFFLINE_DEVICE).arg(seed)
            .arg(id);
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendSetAutoStart(bool autorun)
{
    int seed = qrand();
    QString sdata = QString("{\"type\":%1,\"act\":%2,\"data\":{\"v\":\"%4\"},\"seed\":%3}")
            .arg(MessageValue::SET).arg(MessageValue::SET_AUTO_START).arg(seed)
            .arg(static_cast<int>(autorun));
    QString buf;
    if(!doSendAndReceive(sdata, buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::parseJson(const QString &json, int seed, QJsonValue &data)
{
    try {
        if(json.size()==0){
            m_strLastError = "Empty content.";
            return false;
        }

        QJsonParseError err;
        QJsonDocument jdoc = QJsonDocument::fromJson(json.toUtf8(), &err);
        if(err.error != QJsonParseError::NoError){
            m_strLastError = err.errorString();
            return false;
        }
        if(!jdoc.isObject()){
            m_strLastError = "Json is not object.";
            return false;
        }
        QJsonObject jobj = jdoc.object();
        if(jobj.contains("type") && jobj.contains("seed")){
            int type2 = jobj["type"].toInt(-1);
            int seed2 = jobj["seed"].toInt(0);
            if(seed == seed2){
                if(type2==MessageValue::ACK) {
                    if(jobj.contains("data"))
                    {
                        data = jobj.value("data");
                        return true;
                    }
                }
                else if(type2==MessageValue::ERR){
                    if(jobj.contains("msg")){
                        m_strLastError = jobj["msg"].toString();
                    }
                }
                else{
                    m_strLastError = "Error message type.";
                }
            }
            else{
                m_strLastError = "Unmatched reply. Try again.";
            }
        }
        else{
            m_strLastError = "Incompleted json. Try again.";
        }
    }
    catch(QException &e){
        m_strLastError = e.what();
    }
    catch(std::exception &e){
        m_strLastError = e.what();
    }
    catch(...) {
        m_strLastError = "Unknown error";
    }
    return false;
}

bool ServiceBridge::parseJsonAndVarify(const QString &json, int seed)
{
    try {
        if(json.size()==0){
            m_strLastError = "Empty content.";
            return false;
        }

        QJsonParseError err;
        QJsonDocument jdoc = QJsonDocument::fromJson(json.toUtf8(), &err);
        if(err.error != QJsonParseError::NoError){
            m_strLastError = err.errorString();
            return false;
        }
        if(!jdoc.isObject()){
            m_strLastError = "Json is not object.";
            return false;
        }
        QJsonObject jobj = jdoc.object();
        if(jobj.contains("type") && jobj.contains("seed")){
            int type2 = jobj["type"].toInt(-1);
            int seed2 = jobj["seed"].toInt(0);
            if(seed == seed2){
                if(type2==MessageValue::ACK) {
                    if(jobj.contains("succ"))
                    {
                        if(jobj.value("succ").toInt(0)==0){
                            m_strLastError = "Return failure.";
                            return false;
                        }
                        return true;
                    }
                    else{
                        m_strLastError = "No result.";
                    }
                }
                else if(type2==MessageValue::ERR){
                    if(jobj.contains("msg")){
                        m_strLastError = jobj["msg"].toString();
                    }
                }
                else{
                    m_strLastError = "Error message type.";
                }
            }
            else{
                m_strLastError = "Unmatched reply. Try again.";
            }
        }
        else{
            m_strLastError = "Incompleted json. Try again.";
        }
    }
    catch(QException &e){
        m_strLastError = e.what();
    }
    catch(std::exception &e){
        m_strLastError = e.what();
    }
    catch(...) {
        m_strLastError = "Unknown error";
    }
    return false;
}
}}
