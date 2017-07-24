#ifndef GATEWAT_H
#define GATEWAT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QComboBox>

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = 0);
    void updateDevice(QComboBox* devList);
    bool openDevice(QString dev);
    QString errorString();
    void broadcast();
    void nodeCheck(QByteArray nodeId);
    QStringList nodes();
private:
    bool devReady;
    int gatewat_state;//1:广播，2：点播
    QSerialPort com;
    QList<QSerialPortInfo> infos;
    QStringList nodelist;
    QString errString;
    QByteArray buff;

    void initDevice();
    void readData();

private slots:
    void readGatewayData();

signals:

public slots:
};

#endif // GATEWAT_H
