#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <qtcpsocket.h>
#include <QTimer>
#include <QAbstractSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void networkInit(QHostAddress addr, quint16 port);
    void sendMsg(QByteArray qba);

signals:
    void networkStateChanged(bool success);
    void errString(QString);
    void requireRfid(QByteArray qba);
    void updateId(QString id);

public slots:

private:
    QTcpSocket socket;
    QByteArray id;
    QTimer timer_beat;
    QTimer timer_reconnect();
    void serverReg();

private slots:
    void socketConnected();
    void socketDisconnected();
    void recvData();
    void beatTimeout();
    void socketError(QAbstractSocket::SocketError);
};

#endif // SERVER_H
