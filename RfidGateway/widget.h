#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <gateway.h>

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

private:
    Ui::Widget *ui;
    Gateway* dev_gateway;
};

#endif // WIDGET_H
