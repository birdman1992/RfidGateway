#ifndef GATEWAT_H
#define GATEWAT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class Gatewat : public QObject
{
    Q_OBJECT
public:
    explicit Gatewat(QObject *parent = 0);

signals:

public slots:
};

#endif // GATEWAT_H
