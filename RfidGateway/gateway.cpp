#include "gateway.h"
#include <QDebug>
#include <QByteArray>

#define API_BROADCAST  "fe070000ffffff"

Gateway::Gateway(QObject *parent) : QObject(parent)
{
    connect(&com, SIGNAL(readyRead()), this, SLOT(readGatewayData()));
    connect(&timer_scan, SIGNAL(timeout()), this, SLOT(scanTimeout()));
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
        if(gateway_state == 1)
        {
            if(onePack.mid(2, 4) == QByteArray::fromHex("ffffffff"))
                continue;
            nodelist<<QString(onePack.mid(2, 4).toHex());
            emit newNode(nodelist.last());
        }
        else if(gateway_state == 2)
        {
            emit nodeFire(QString(onePack.mid(2, 4).toHex()));
        }

        qDebug()<<"[onePack]"<<onePack;
    }
}

void Gateway::startScan()
{
    scanPorcess = 0;
    timer_scan.start(200);
}

void Gateway::scanOver()
{
    int i = 0;
    int j = 0;
    int lastCount = nodelist.count()%60;
    int dataCount = nodelist.count()/60+(lastCount!=0);

    for(j=0; j<dataCount; j++)
    {
        QByteArray qba = QByteArray::fromHex("fe0002");
        if(j == dataCount-1)
            for(i=0; i<lastCount; i++)
            {
                QString node = nodelist.at(i);
                qba += QByteArray::fromHex(node.toLocal8Bit());
            }
        else
            for(i=0; i<60; i++)
            {
                QString node = nodelist.at(i);
                qba += QByteArray::fromHex(node.toLocal8Bit());
            }
        qba.append(0xff);
        qba[1] = qba.size();
        qDebug()<<qba.toHex();
        emit nodeReport(qba);
    }
}

void Gateway::readGatewayData()
{
    buff.append(com.readAll());
    qDebug()<<"[readGatewayData]"<<buff.toHex();
    readData();
}

void Gateway::scanTimeout()
{
    scanPorcess++;
    emit scanProgress(scanPorcess);
    if(scanPorcess>=100)
    {
        timer_scan.stop();
        scanOver();
    }
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
        nodelist.clear();
        gateway_state = 1;
        QByteArray qba;
        qba = QByteArray::fromHex("fe07ffffffffff");
        qDebug()<<"[broadcast>>>>>>>>>]"<<qba.toHex();
        qDebug()<<com.write(qba.data(),7);
        startScan();
    }
}

void Gateway::nodeCheck(QByteArray nodeId)
{
    if(devReady)
    {
        gateway_state = 2;
        QByteArray qba = QByteArray::fromHex("fe07" + nodeId + "ff");
        qba[1] = qba.size();
        qDebug()<<"[nodeCheck]"<<qba.toHex();
        com.write(qba);
    }
}

void Gateway::gatewayCtrl(QByteArray qba)
{
    if(devReady)
    {
        if(qba.mid(2,4) == QByteArray::fromHex("ffffffff"))
        {
            emit clearNodes();
            broadcast();
        }
        else
        {
            gateway_state = 2;
        }
    }
}

QStringList Gateway::nodes()
{
    return nodelist;
}
