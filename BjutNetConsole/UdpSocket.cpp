#include "UdpSocket.h"

#include <boost/asio.hpp>

using namespace boost;
using udp = asio::ip::udp;

namespace bna{

UdpSocket::UdpSocket()
{
    m_socket = new udp::socket(m_ioContext);
    m_remote_endpoint = udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 6350);
    m_socket->open(m_remote_endpoint.protocol());
    //m_local_endpoint = m_socket->local_endpoint();
}

UdpSocket::~UdpSocket()
{
    if(m_socket){
        m_socket->close();
        m_socket->release();
        delete m_socket;
        m_socket = nullptr;
    }
}

bool UdpSocket::setRemoteHost(const std::string &address, unsigned short port)
{
    boost::system::error_code err;
    auto addr = asio::ip::address::from_string(address, err);
    if(err.failed()){
        return false;
    }
    m_remote_endpoint = udp::endpoint(addr, port);
    return true;
}

bool UdpSocket::send(const std::string &data)
{
    try{
    int sz = m_socket->send_to(asio::buffer(data), m_remote_endpoint);
    return sz > 0;
    }
    catch(...)
    {
        return false;
    }
}

bool UdpSocket::send(const char *data, int size)
{
    try{
    int sz = m_socket->send_to(asio::buffer(data, size), m_remote_endpoint);
    return sz > 0;
    }
    catch(...)
    {
        return false;
    }
}

bool UdpSocket::receive(std::string &data)
{
    try{
    char buf[1600];
    auto sz = m_socket->receive(asio::buffer(buf, 1600));
    if(sz){
        data.append(static_cast<char*>(buf), sz);
        return true;
    }
    return false;
    }catch(...){
        return false;
    }
}

bool UdpSocket::receive(char* &data)
{
    try{
    asio::mutable_buffer buf;
    auto sz = m_socket->receive_from(asio::buffer(buf), m_remote_endpoint);
    if(sz){
        data = new char[sz+1];
        memcpy(data, buf.data(), sz);
        data[sz] = 0;
        return true;
    }
    return false;
    }
    catch(...)
    {
        return false;
    }
}

}
