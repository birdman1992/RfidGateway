#include "gateway.h"
#include <QDebug>
#include <QByteArray>

#define API_BROADCAST  "fe070000ffffff"

Gateway::Gateway(QObject *parent) : QObject(parent)
{
    connect(&com, SIGNAL(readyRead()), this, SLOT(readGatewayData()));
}

void Gateway::updateDevice(QComboBox *devList)
{
    infos = QSerialPortInfo::availablePorts();
    int i = 0;
    devList->clear();

    for(i=0; i<infos.count();)
    {
        if(infos.at(i).portName() == "COM1")
        {
            infos.removeAt(i);
            continue;
        }
        devList->addItem(infos.at(i).portName());
        qDebug()<<infos.at(i).serialNumber();
        i++;
    }
}

void Gateway::initDevice()
{

}

void Gateway::readData()
{
    while(!buff.isEmpty())
    {
        if(buff.at(0) != (char)0xfc)
        {
            buff.remove(0,1);
            continue;
        }
        int len = buff.at(1);
        if(buff.size()<len)
            return;

        QByteArray onePack = buff.left(len);
        buff.remove(0, len);
        if(gatewat_state == 1)
        {
            nodes<<QString(onePack.mid(2, 4).toHex());
        }


        qDebug()<<"[onePack]"<<onePack;
    }
}

void Gateway::readGatewayData()
{
    buff.append(com.readAll());
    qDebug()<<"[readGatewayData]"<<buff;
    readData();
}

bool Gateway::openDevice(QString dev)
{
    qDebug()<<"open"<<dev;
    com.close();
    com.setPortName(dev);
    bool ret = com.open(QIODevice::ReadWrite);
    com.setBaudRate(QSerialPort::Baud115200);
    com.setDataBits(QSerialPort::Data8);
    com.setFlowControl(QSerialPort::NoFlowControl);
    com.setParity(QSerialPort::NoParity);
    com.setStopBits(QSerialPort::OneStop);
    errString = com.errorString();
//    qDebug()<<com.error()<<com.errorString();
    devReady = ret;
    return ret;
}

QString Gateway::errorString()
{
    return errString;
}

void Gateway::broadcast()
{
    if(devReady)
    {
        nodes.clear();
        gatewat_state = 1;
        QByteArray qba = QByteArray::fromHex(API_BROADCAST);
        qDebug()<<"[broadcast]"<<qba.toHex();
        com.write(qba);
    }
}

void Gateway::nodeCheck(QByteArray nodeId)
{
    if(devReady)
    {
        gatewat_state = 2;
        QByteArray qba = QByteArray::fromHex("fe07" + nodeId + "ff");
        qba[1] = qba.size();
        qDebug()<<"[nodeCheck]"<<qba.toHex();
        com.write(qba);
    }
}

QStringList Gateway::nodes()
{
    return nodelist;
}
