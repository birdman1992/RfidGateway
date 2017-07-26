#ifndef GATEWAT_H
#define GATEWAT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QComboBox>
#include <QTimer>

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
    void gatewayCtrl(QByteArray qba);
    QStringList nodes();
private:
    bool devReady;
    int gateway_state;//1:广播，2：点播
    int scanPorcess;//扫描进度
    QTimer timer_scan;
    QSerialPort com;
    QList<QSerialPortInfo> infos;
    QStringList nodelist;
    QString errString;
    QByteArray buff;

    void initDevice();
    void readData();
    void startScan();
    void scanOver();

private slots:
    void readGatewayData();
    void scanTimeout();

signals:
    void newNode(QString id);
    void scanProgress(int);
    void nodeReport(QByteArray);
    void nodeFire(QString nodeId);
    void clearNodes();

public slots:
};

#endif // GATEWAT_H
