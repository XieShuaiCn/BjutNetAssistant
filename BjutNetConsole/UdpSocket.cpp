#include "UdpSocket.h"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost;
using udp = asio::ip::udp;

namespace bna{

UdpSocket::UdpSocket()
    : m_deadline(m_ioContext)
{
    m_socket = new udp::socket(m_ioContext);
    m_remote_endpoint = udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 6350);
    m_socket->open(m_remote_endpoint.protocol());
    //m_local_endpoint = m_socket->local_endpoint();
    m_deadline.expires_at(boost::posix_time::pos_infin);
    check_deadline();
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
    if(m_socket->is_open()) {
        m_socket->close();
    }
    m_socket->open(m_remote_endpoint.protocol());
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

bool UdpSocket::receive_timeout(std::string &data, int milliseconds)
{
    try{
        system::error_code receive_ec = asio::error::would_block;
        size_t length = 0;
        data.clear();
        char buf[1600];
        // set timer
        system::error_code deadline_ec;
        m_deadline.expires_from_now(boost::posix_time::milliseconds(milliseconds), deadline_ec);
        if(deadline_ec.failed()){
            return false;
        }
        // recevice data
        m_socket->async_receive(asio::buffer(buf),
                                boost::bind(&UdpSocket::handle_receive,
                                            boost::placeholders::_1,
                                            boost::placeholders::_2,
                                            &receive_ec,
                                            &length));
        do{
            m_ioContext.run_one();
        }while(receive_ec == asio::error::would_block );
        m_deadline.expires_at(boost::posix_time::pos_infin);
        // copy data
        if(length){
            data.append(static_cast<char*>(buf), length);
            return true;
        }
        return false;
    }catch(...){
        return false;
    }
}
void UdpSocket::check_deadline()
{
    if (m_deadline.expires_at() <= asio::deadline_timer::traits_type::now())
    {
        m_socket->cancel();
        m_deadline.expires_at(boost::posix_time::pos_infin);
    }
    m_deadline.async_wait(boost::bind(&UdpSocket::check_deadline, this));
}
void UdpSocket::handle_receive(const boost::system::error_code& ec, std::size_t length,
                           boost::system::error_code* out_ec, std::size_t* out_length)
{
    *out_ec = ec;
    *out_length = length;
}

}
