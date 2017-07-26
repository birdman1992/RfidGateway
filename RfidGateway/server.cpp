#include "server.h"
#include <QHostAddress>
#include <QTime>
#include <QSettings>
#include <unistd.h>


Server::Server(QObject *parent) : QObject(parent)
{
    connect(&socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(recvData()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(&timer_beat, SIGNAL(timeout()), this, SLOT(beatTimeout()));
}

void Server::networkInit(QHostAddress addr, quint16 port)
{
    if(socket.state() == QAbstractSocket::ConnectedState)
    {
        socket.disconnectFromHost();
        socket.waitForDisconnected(5000);
//        ::sleep(3);
    }
    socket.connectToHost(addr, port);
}

void Server::sendMsg(QByteArray qba)
{
    if(socket.state() == QAbstractSocket::ConnectedState)
        socket.write(qba);
}

void Server::serverReg()
{
    QSettings setting;
    id = setting.value("ID", "").toByteArray();
    if(id.isEmpty())
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        id.resize(8);
        int i = 0;

        for(i=0; i<8; i++)
        {
            id[i] = qrand()%255;
        }
        setting.setValue("ID", id);
    }

    QByteArray qba = QByteArray::fromHex("fe0c01")+id+QByteArray::fromHex("ff");
    qDebug()<<"[reg]"<<qba.toHex()<<id.toHex();
    if(socket.write(qba) > 0)
        emit updateId(QString(id.toHex()));
    timer_beat.start(100000);
}

void Server::socketConnected()
{
    emit networkStateChanged(true);
    serverReg();
}

void Server::socketDisconnected()
{
    timer_beat.stop();
    emit networkStateChanged(false);
}

void Server::recvData()
{
    QByteArray qba = socket.readAll();
    qDebug()<<"[recvData]"<<qba.toHex();

    if((qba.size() == (char)8) && (qba[0] == (char)0xfe) && (qba[7] == (char)0xff))
    {
        qba.remove(2,1);
        qba[1] = 7;
        emit requireRfid(qba);
    }
}

void Server::beatTimeout()
{
    QByteArray qba = QByteArray::fromHex("fe06000000ff");
    qDebug()<<"[beat]"<<qba.toHex();
    socket.write(qba);
}

void Server::socketError(QAbstractSocket::SocketError e)
{
    qDebug()<<e<<socket.errorString();
    emit errString(socket.errorString());
}
