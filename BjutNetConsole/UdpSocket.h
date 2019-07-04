#ifndef BNC_UDPSOCKET_H
#define BNC_UDPSOCKET_H

#include <string>
#include <boost/asio/ip/udp.hpp>

namespace bna{

class UdpSocket
{
public:
    UdpSocket();
    ~UdpSocket();

    bool send(const std::string &data);
    bool send(const char *data, int size);

    bool receive(std::string &data);
    bool receive(char* &data);

    bool setRemoteHost(const std::string &address, unsigned short port);

private:
    boost::asio::io_context m_ioContext;
    boost::asio::ip::udp::socket *m_socket;
    boost::asio::ip::udp::endpoint m_remote_endpoint;
    boost::asio::ip::udp::endpoint m_local_endpoint;
};

}
#endif // BNC_UDPSOCKET_H
