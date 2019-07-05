#ifndef BNS_SERVICEINTERFACE_H
#define BNS_SERVICEINTERFACE_H

#include <QCoreApplication>
#include <QHostAddress>

class QUdpSocket;

namespace bna{

class BjutNet;

class ServiceInterface : public QObject
{
    Q_OBJECT
public:
    ServiceInterface(BjutNet *BjutNet);

    bool Initilize();

public slots:
    void ReadSocketData();

    void ProcessCommand(const QByteArray &cmd, const QHostAddress &address, quint16 port);

    void PushMessage(const QDateTime& time, const QString& info);

private:
    QUdpSocket *m_socket;
    QHostAddress m_remoteHost;
    quint16 m_remotePort;
    BjutNet *m_bjutnet;
};
}
#endif // BNS_SERVICEINTERFACE_H
