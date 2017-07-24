#include "widget.h"
#include "ui_widget.h"
#include <QListView>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->dev->setView(new QListView());
    ui->nodeList->setColumnCount(1);
    ui->nodeList->setHorizontalHeaderLabels(QStringList("节点列表"));
    dev_gateway = new Gateway(this);
    dev_gateway->updateDevice(ui->dev);

//    if(dev_gateway->openDevice(ui->dev->itemText(ui->dev->currentIndex())))
//    {
//        QPalette pa;
//        pa.setColor(QPalette::WindowText,Qt::green);
//        ui->msg->setPalette(pa);
//        ui->msg->setText("打开成功");
//    }
//    else
//    {
//        QPalette pa;
//        pa.setColor(QPalette::WindowText,Qt::red);
//        ui->msg->setPalette(pa);
//        ui->msg->setText("打开失败");
//    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_dev_currentIndexChanged(const QString &arg1)
{
    if(dev_gateway->openDevice(arg1))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->msg->setPalette(pa);
        ui->msg->setText("打开成功");
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
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
    dev_gateway->broadcast();
}

void Widget::on_check_clicked()
{
    dev_gateway->nodeCheck("00000005");
}
