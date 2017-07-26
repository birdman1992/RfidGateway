#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "gateway.h"
#include "server.h"
#include "serverconfig.h"
#include <QTableWidgetItem>
#include <QPaintEvent>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_dev_currentIndexChanged(const QString &arg1);
    void on_fresh_clicked();
    void on_broadcast_clicked();
    void on_check_clicked();
    void getNewNode(QString node);
    void networkState(bool state);
    void networkError(QString err);
    void reportNodes(QByteArray qba);
    void socket2rfid(QByteArray qba);
    void on_act_show();
    void on_act_close();
    void iconIsActived(QSystemTrayIcon::ActivationReason);
    void on_nodeList_itemClicked(QTableWidgetItem *item);
    void updateId(QString id);
    void nodeFired(QString nodeId);
    void clearNodes();

    void on_fresh_network_clicked();

    void on_connect_clicked();

    void on_hide_clicked();

private:
    Ui::Widget *ui;
    Gateway* dev_gateway;
    Server* rfidServer;
    QString selectNode;
    ServerConfig* config;
    QAction* act_show;
    QAction* act_close;
    QMenu* systrayMenu;
    QSystemTrayIcon* sysTray;

    void creatSystemTray();
    void creatActions();
    void paintEvent(QPaintEvent *);
};

#endif // WIDGET_H
