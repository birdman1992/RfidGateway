#include "serverconfig.h"
#include "ui_serverconfig.h"
#include <QSettings>
#include <QPainter>

ServerConfig::ServerConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerConfig)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    QSettings settings;
    addr = settings.value("SERVER_IP","").toString();
    port = settings.value("SERVER_PORT","").toString();
    if(port.isEmpty())
        port = "7777";

    if(port.isEmpty() || addr.isEmpty())
    {
        this->show();
    }

    ui->addr->setText(addr);
    ui->port->setText(port);
}

ServerConfig::~ServerConfig()
{
    delete ui;
}

void ServerConfig::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ServerConfig::on_ok_clicked()
{
    if((ui->addr->text() == addr) && (ui->port->text() == port))
    {
        return;
    }

    addr = ui->addr->text();
    port = ui->port->text();
    QSettings setting;
    setting.setValue("SERVER_IP", addr);
    setting.setValue("SERVER_PORT", port);
    setting.sync();
    qDebug()<<"config"<<addr<<port;
    emit changeConfig(QHostAddress(addr), port.toInt());

    this->close();
}

void ServerConfig::on_cancel_clicked()
{
    ui->addr->setText(addr);
    ui->port->setText(port);
    this->close();
}
