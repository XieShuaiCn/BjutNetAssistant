#ifndef BNC_UDPSOCKET_H
#define BNC_UDPSOCKET_H

#include <vector>
#include <string>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace bna{

class UdpSocket
{
public:
    UdpSocket();
    ~UdpSocket();

    bool send(const std::string &data);
    bool send(const char *data, int size);

    bool receive(std::string &data);
    bool receive_timeout(std::string &data, int milliseconds);

    bool setRemoteHost(const std::string &address, unsigned short port);
    bool getRemoteHost(std::string &address, unsigned short &port);

    static void handle_receive(const boost::system::error_code& ec, std::size_t length,
                               boost::system::error_code* out_ec, std::size_t* out_length);

    static void myAddress(std::vector<std::string> &ip);

private:
    void check_deadline();

    boost::asio::io_context m_ioContext;
    boost::asio::ip::udp::socket *m_socket;
    boost::asio::deadline_timer m_deadline;
    boost::asio::ip::udp::endpoint m_remote_endpoint;
    boost::asio::ip::udp::endpoint m_local_endpoint;
};

}
#endif // BNC_UDPSOCKET_H
