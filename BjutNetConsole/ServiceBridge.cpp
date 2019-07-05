#include "ServiceBridge.h"

#include <boost/exception/all.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "UdpSocket.h"
#include "Version.h"
#include "MessageValue.h"

using std::string;
using std::stringstream;
using boost_err_str = boost::error_info<struct tag_err_str,string>;

namespace bna{

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

bool ServiceBridge::sendSYN()
{
    char ch(CHAR_SYN);
    if(!m_socket->send(&ch, 1)) {
        return false;
    }
    string data;
    if(!m_socket->receive(data)) {
        return false;
    }
    return (data.size()==1 && data[0]==CHAR_ACK);
}

bool ServiceBridge::sendENQ()
{
    char ch(CHAR_ENQ);
    if(!m_socket->send(&ch, 1)) {
        return false;
    }
    string data;
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree pt;
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendAct_common(MessageValue::ActionAct type)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << static_cast<MessageValue::Type>(MessageValue::ACT)
          << ",\"act\":" << static_cast<MessageValue::Type>(type)
          << ",\"seed\":" << seed << "}";
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::sendGetVersion(std::string &var, int &inner_ver)
{
    stringstream ssbuf;
    int seed = rand();
    ssbuf << "{\"type\":" << MessageValue::GET
          << ",\"act\":" << MessageValue::GET_VERSION
          << ",\"seed\":" << seed << "}";
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        name = data.get<char>("n", "");
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        name = data.get<char>("n", "");
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    if (parseJson(buf, seed, data))
    {
        if(data.size()){
            for(auto &d : data){
                auto id = d.second.get<int>("id", 0);
                auto name = d.second.get<char>("n", "");
                auto msg = d.second.get<char>("m", "");
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree data;
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
    if(!m_socket->send(ssbuf.str())) {
        return false;
    }
    string buf;
    if(!m_socket->receive(buf)) {
        return false;
    }
    boost::property_tree::ptree data;
    return parseJsonAndVarify(buf, seed);
}

bool ServiceBridge::parseJson(const std::string &json, int seed, boost::property_tree::ptree &tree)
{
    try {
//        string buf;
//        size_t json_beg = 0, json_end = 0;
//        while(json_beg < json.size()){
//            json_end = json.find("\"\"", json_beg);
//            json_end = (json_end==string::npos ? json.size() : json_end);
//            buf.append(json, json_beg, json_end-json_beg);
//            json_beg = json_end + 2;
//        }
        boost::property_tree::ptree pt;
        stringstream rrbuf(json);
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
    catch(boost::exception &e){
        m_strLastError = *(boost::get_error_info<boost_err_str>(e));
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
            }
            else if(type2==MessageValue::ERR){
                if(pt.find("msg")!=pt.not_found()){
                    m_strLastError = pt.get<char>("msg");
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
    catch(boost::exception &e){
        m_strLastError = *(boost::get_error_info<boost_err_str>(e));
    }
    catch(...) {
        m_strLastError = "Unknown error";
    }
    return false;
}

}
