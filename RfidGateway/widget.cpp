#include "widget.h"
#include "ui_widget.h"
#include <QListView>
#include <QSystemTrayIcon>
#include <QHostAddress>
#include <QSettings>
#include <QPainter>
//#define SERVER_ADDR "120.25.205.94"
//#define SERVER_ADDR "192.168.0.12"
//#define SERVER_PORT 7777


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->dev->setView(new QListView());
    ui->msg_fire->hide();
    ui->fire_id->hide();
    ui->nodeList->setColumnCount(1);
    ui->nodeList->setHorizontalHeaderLabels(QStringList("节点列表"));
    ui->nodeList->horizontalHeader()->setStyleSheet("QHeaderView::section{font: 18px \"微软雅黑\";\
                                                    color: rgb(255,255,255);\
                                                    background-color: rgb(22, 52, 73);}");
//    ui->scanProgress->hide();
    dev_gateway = new Gateway(this);
    dev_gateway->updateDevice(ui->dev);
    connect(dev_gateway, SIGNAL(newNode(QString)), this, SLOT(getNewNode(QString)));
    connect(dev_gateway, SIGNAL(scanProgress(int)), ui->scanProgress, SLOT(setValue(int)));
    connect(dev_gateway, SIGNAL(nodeReport(QByteArray)), this, SLOT(reportNodes(QByteArray)));
    connect(dev_gateway, SIGNAL(nodeFire(QString)), this, SLOT(nodeFired(QString)));
    connect(dev_gateway, SIGNAL(clearNodes()), this, SLOT(clearNodes()));

    rfidServer = new Server(this);
    connect(rfidServer, SIGNAL(networkStateChanged(bool)), this, SLOT(networkState(bool)));
    connect(rfidServer, SIGNAL(errString(QString)), this, SLOT(networkError(QString)));
    connect(rfidServer, SIGNAL(requireRfid(QByteArray)), this, SLOT(socket2rfid(QByteArray)));
    connect(rfidServer, SIGNAL(updateId(QString)), this, SLOT(updateId(QString)));

    config = new ServerConfig();

    QSettings setting;
    QString port = setting.value("SERVER_PORT","").toString();
    QString addr = setting.value("SERVER_IP","").toString();

    if(!(port.isEmpty() || addr.isEmpty()))
        rfidServer->networkInit(QHostAddress(addr), port.toInt());

    creatSystemTray();
}

Widget::~Widget()
{
    delete sysTray;
    delete ui;
}

void Widget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    p.fillRect(this->rect(), QColor(22, 52, 73, 200));
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Widget::on_dev_currentIndexChanged(const QString &arg1)
{
    if(dev_gateway->openDevice(arg1))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(122,185,0));
        ui->msg->setPalette(pa);
        ui->msg->setText("打开成功");
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(234,32,0));
        ui->msg->setPalette(pa);
        ui->msg->setText(dev_gateway->errorString());
    }
}

void Widget::on_fresh_clicked()
{
    dev_gateway->updateDevice(ui->dev);
}

void Widget::on_broadcast_clicked()
{
    ui->msg_fire->hide();
    ui->fire_id->hide();
    ui->nodeList->clearContents();
    ui->nodeList->setRowCount(0);
    dev_gateway->broadcast();
}

void Widget::on_check_clicked()
{
    ui->msg_fire->hide();
    ui->fire_id->hide();
    if(selectNode.isEmpty())
        return;

    dev_gateway->nodeCheck(selectNode.toLocal8Bit());
}

void Widget::getNewNode(QString node)
{
    int nodeNum = ui->nodeList->rowCount();

    ui->nodeList->setRowCount(nodeNum+1);
    ui->nodeList->setItem(nodeNum, 0, new QTableWidgetItem(node));
}

void Widget::networkState(bool state)
{
    qDebug()<<state;
    if(state)
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(122,185,0));
        ui->msg_network->setPalette(pa);
        ui->msg_network->setText("网络连接成功");
    }
//    else
//    {
//        QPalette pa;
//        pa.setColor(QPalette::WindowText,QColor(234,32,0));
//        ui->msg_network->setPalette(pa);
//        ui->msg_network->setText("网络连接中断");
//    }
}

void Widget::networkError(QString err)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(234,32,0));
    ui->msg_network->setPalette(pa);
    ui->msg_network->setText(err);
}

void Widget::reportNodes(QByteArray qba)
{
    rfidServer->sendMsg(qba);
}

void Widget::socket2rfid(QByteArray qba)
{
    dev_gateway->gatewayCtrl(qba);
}

void Widget::on_act_show()
{
    raise();
    this->show();
}

void Widget::on_act_close()
{
    this->close();
}

void Widget::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //点击托盘显示窗口
        case QSystemTrayIcon::Trigger:
        {
            raise();
            this->show();
            break;
        }
        //双击托盘显示窗口
        case QSystemTrayIcon::DoubleClick:
        {
            raise();
            this->show();
            break;
        }
        default:
        break;
    }
}


void Widget::on_nodeList_itemClicked(QTableWidgetItem *item)
{
    selectNode = item->text();
}

void Widget::updateId(QString id)
{
    ui->id->setText(id);
}

void Widget::nodeFired(QString nodeId)
{
    ui->msg_fire->show();
    ui->fire_id->show();
    ui->fire_id->setText(nodeId);
}

void Widget::clearNodes()
{
    ui->nodeList->clearContents();
    ui->nodeList->setRowCount(0);
}

void Widget::creatSystemTray()
{
    sysTray = new QSystemTrayIcon();
    sysTray->setIcon(QIcon(":/imgs/systray.png"));
    sysTray->setToolTip("德荣仓库标签管理终端");
    systrayMenu = new QMenu(this);
    act_show = new QAction("打开主面板");
    act_close = new QAction("退出");

    systrayMenu->addAction(act_show);
    systrayMenu->addAction(act_close);

    connect(act_show, SIGNAL(triggered()), this, SLOT(on_act_show()));
    connect(act_close, SIGNAL(triggered()), this, SLOT(on_act_close()));
    connect(sysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    sysTray->setContextMenu(systrayMenu);
    sysTray->show();
}

void Widget::on_fresh_network_clicked()
{
    config->show();
}

void Widget::on_connect_clicked()
{
    QSettings setting;
    QString port = setting.value("SERVER_PORT","").toString();
    QString addr = setting.value("SERVER_IP","").toString();

    if(!(port.isEmpty() || addr.isEmpty()))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,QColor(122,185,0));
        ui->msg_network->setPalette(pa);
        ui->msg_network->setText("正在连接");
        rfidServer->networkInit(QHostAddress(addr), port.toInt());
    }
}

void Widget::on_hide_clicked()
{
    this->hide();
}
