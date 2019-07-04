#ifndef BNS_SERVICEINTERFACE_H
#define BNS_SERVICEINTERFACE_H

#include <QCoreApplication>
#include <QHostAddress>

class QUdpSocket;

namespace bna{

class ServiceInterface : public QObject
{
    Q_OBJECT
public:
    ServiceInterface(int argc, char **argv);

    int Run();

public slots:
    void ReadSocketData();

    void ProcessCommand(const QByteArray &cmd, const QHostAddress &address, quint16 port);

protected:
    bool Initilize();

private:
    QUdpSocket *m_socket;
    QCoreApplication *m_app;
};
}
#endif // BNS_SERVICEINTERFACE_H
