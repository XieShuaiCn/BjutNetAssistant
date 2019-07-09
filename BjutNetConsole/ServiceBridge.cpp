#include "ServiceBridge.h"
#include <boost/exception/all.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "UdpSocket.h"
#include "Version.h"
#include "MessageValue.h"
#include "Utility.h"

using std::string;
using std::stringstream;
using boost_err_str = boost::error_info<struct tag_err_str,string>;

namespace bna{

inline bool __ServiceBridge_SendAndReceive(UdpSocket *socket, const std::string &sdata, std::string &rdata)
{
    if(!socket->send(sdata)) {
        return false;
    }
    if(!socket->receive(rdata)) {
        return false;
    }
    return true;
}

ServiceBridge::ServiceBridge()
{
    m_socket = new UdpSocket();
    assert(m_socket != nullptr);
    srand(time(nullptr));
}

bool ServiceBridge::setHost(const std::string &host)
{
    return m_socket->setRemoteHost(host, MessageValue::ServerPort);
}

const string ServiceBridge::getHost()
{
    string address;
    unsigned short port = 0;
    m_socket->getRemoteHost(address, port);
    return address;
}

void ServiceBridge::getMyAddress(std::vector<std::string> &ip)
{
    if(!ListLocalIpAddress(ip)){
        ip.clear();
    }

}

bool ServiceBridge::sendSYN()
{
    char ch(CHAR_SYN);
    string data;
    while(m_socket->receive_timeout(data, 100));
    if(!m_socket->send(&ch, 1)) {
        return false;
    }
    data.clear();
    if(!m_socket->receive_timeout(data, 2000)) {
        return false;
    }
    return (data.size()==1 && data[0]==CHAR_ACK);
}

bool ServiceBridge::sendENQ()
{
    char ch(CHAR_ENQ);
    string data;
    while(m_socket->receive_timeout(data, 100));
    if(!m_socket->send(&ch, 1)) {
        return false;
    }
    if(!m_socket->receive(data)) {
        return false;
    }
    if(data.size() != 1){
        return false;
    }
    m_nMsgVersion = static_cast<int>(static_cast<unsigned char>(data[0]));
    return m_nMsgVersion <= MessageValue::Version;
}

bool ServiceBridge::sendSyncHello()
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::SYNC
          << ",\"act\":" << MessageValue::HELLO
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendAct_common(MessageValue::ActionAct type)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << static_cast<MessageValue::Type>(MessageValue::ACT)
          << ",\"act\":" << static_cast<MessageValue::Type>(type)
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendGetVersion(std::string &var, int &inner_ver)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_VERSION
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        var = data.get<char>("v", "");
        inner_ver = data.get<int>("n", 0);
        return true;
    }
    return false;
}

bool ServiceBridge::sendGetAccount(std::string &name, std::string &password, int &type)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_ACCOUNT
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        name = data.get<char>("n", "");
        password = data.get<char>("p", "");
        type = data.get<int>("t", 0);
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetUsedFlow(int &value)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_USED_FLOW
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        value = data.get<int>("v", 0);
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetUsedTime(int &value)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_USED_TIME
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        value = data.get<int>("v", 0);
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetLeftFee(int &value)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_LEFT_FEE
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        value = data.get<int>("v", 0);
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetAllFlow(int &value)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_ALL_FLOW
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        value = data.get<int>("v", 0);
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetOnlineDevices(std::vector<std::array<std::string, 4>> &devices)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_DEVICE_ONLINE
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        if(data.size()){
            for(auto &d : data){
                auto id = d.second.get<int>("id", 0);
                auto ipv4 = d.second.get<char>("ipv4", "");
                auto ipv6 = d.second.get<char>("ipv6", "");
                devices.emplace_back(std::array<std::string, 4>{std::to_string(id), ipv4, ipv6, ""});
            }
        }
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetFlowService(std::string &name, int &totalFlow)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_FLOW_SERVICE
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        name = data.get<char>("n", "");
        if(!CheckUtf8ToMultiBytes(name)) {
            m_strLastError = "Fail to convert UTF8 to Locale.";
        }
        totalFlow = data.get<int>("v", 0);
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetBookedService(std::string &name)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_BOOKED_SERVICE
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        name = data.get<char>("n", "");
        if(!CheckUtf8ToMultiBytes(name)) {
            m_strLastError = "Fail to convert UTF8 to Locale.";
        }
        return true;
    }
    return false;
}
bool ServiceBridge::sendGetAllServices(std::vector<std::tuple<int, std::string, std::string>> &services)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_ALL_SERVICES
          << ",\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        if(data.size()){
            for(auto &d : data){
                auto id = d.second.get<int>("id", 0);
                string name = d.second.get<char>("n", "");
                if(!CheckUtf8ToMultiBytes(name)) {
                    m_strLastError = "Fail to convert UTF8 to Locale.";
                }
                string msg = d.second.get<char>("m", "");
                if(!CheckUtf8ToMultiBytes(msg)) {
                    m_strLastError = "Fail to convert UTF8 to Locale.";
                }
                services.emplace_back(id, name, msg);
            }
        }
        return true;
    }
    return false;
}

bool ServiceBridge::sendSetAccount(const std::string name, const std::string passwd, int type)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::SET
          << ",\"act\":" << MessageValue::SET_ACCOUNT
          << ",\"data\":{\"n\":\"" << name << "\",\"p\":\""<<passwd << "\",\"t\":" << type
          << "},\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendSetBookedService(int id)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::SET
          << ",\"act\":" << MessageValue::SET_BOOK_SERVICE
          << ",\"data\":{\"id\":" << id
          << "},\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendSetOfflineDevice(int id)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::SET
          << ",\"act\":" << MessageValue::SET_OFFLINE_DEVICE
          << ",\"data\":{\"id\":" << id
          << "},\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendSetAutoStart(bool autorun)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::SET
          << ",\"act\":" << MessageValue::SET_AUTO_START
          << ",\"data\":{\"v\":" << static_cast<int>(autorun)
          << "},\"seed\":" << seed << "}";
    string buf;
    if(!__ServiceBridge_SendAndReceive(m_socket, ssbuf.str(), buf)) {
        return false;
    }
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::parseJson(const std::string &json, int seed, boost::property_tree::ptree &tree)
{
    try {
        if(json.empty()){
            m_strLastError = "Empty content.";
            return false;
        }

        stringstream rrbuf(json);
        boost::property_tree::ptree pt;
        boost::property_tree::json_parser::read_json(rrbuf, pt);
        int type2 = pt.get<int>("type", -1);
        int seed2 = pt.get<int>("seed", 0);
        if (seed==seed2)
        {
            if(type2==MessageValue::ACK)
            {
                if(pt.find("data")!=pt.not_found())
                {
                    tree = pt.get_child("data");
                    return true;
                }
            }
            else if(type2==MessageValue::ERR){
                if(pt.find("msg")!=pt.not_found()){
                    m_strLastError = pt.get<char>("msg", "");
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
    catch(boost::property_tree::json_parser_error &e){
        m_strLastError = e.message();
    }
    catch(...) {
        m_strLastError = "Unknown error";
    }
    return false;
}

bool ServiceBridge::parseJsonAndVarify(const std::string &json, int seed)
{
    try {
        boost::property_tree::ptree pt;
        stringstream rrbuf(json);
        boost::property_tree::json_parser::read_json(rrbuf, pt);
        int type2 = pt.get<int>("type", -1);
        int seed2 = pt.get<int>("seed", 0);
        if (seed==seed2)
        {
            if(type2==MessageValue::ACK)
            {
                if(pt.find("succ")!=pt.not_found())
                {
                    if(pt.get<int>("succ") == 1){
                        return true;
                    }
                    m_strLastError = "Return failure.";
                }
                else{
                    m_strLastError = "No result.";
                }
            }
            else if(type2==MessageValue::ERR){
                if(pt.find("msg")!=pt.not_found()){
                    m_strLastError = pt.get<char>("msg", "");
                }
                else{
                    m_strLastError = "Error, but no more information.";
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
    catch(boost::property_tree::json_parser_error &e){
        m_strLastError = e.message();
    }
    catch(...) {
        m_strLastError = "Unknown error";
    }
    return false;
}

}
