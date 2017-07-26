#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QWidget>
#include <QHostAddress>
#include <QPaintEvent>

namespace Ui {
class ServerConfig;
}

class ServerConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ServerConfig(QWidget *parent = 0);
    ~ServerConfig();

signals:
    void changeConfig(QHostAddress addr, quint16 port);

private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

private:
    Ui::ServerConfig *ui;
    QString addr;
    QString port;
    void paintEvent(QPaintEvent *);
};

#endif // SERVERCONFIG_H
