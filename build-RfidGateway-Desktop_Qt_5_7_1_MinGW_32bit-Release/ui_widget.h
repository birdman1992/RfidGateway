/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QComboBox *dev;
    QLabel *msg;
    QPushButton *fresh;
    QPushButton *broadcast;
    QPushButton *check;
    QTableWidget *nodeList;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(409, 310);
        dev = new QComboBox(Widget);
        dev->setObjectName(QStringLiteral("dev"));
        dev->setGeometry(QRect(50, 30, 80, 30));
        dev->setMinimumSize(QSize(0, 0));
        dev->setStyleSheet(QStringLiteral("QAbstractItemView::item { min-height: 30px; min-width: 60px; }"));
        msg = new QLabel(Widget);
        msg->setObjectName(QStringLiteral("msg"));
        msg->setGeometry(QRect(150, 35, 231, 21));
        fresh = new QPushButton(Widget);
        fresh->setObjectName(QStringLiteral("fresh"));
        fresh->setGeometry(QRect(10, 30, 31, 31));
        broadcast = new QPushButton(Widget);
        broadcast->setObjectName(QStringLiteral("broadcast"));
        broadcast->setGeometry(QRect(10, 70, 75, 41));
        check = new QPushButton(Widget);
        check->setObjectName(QStringLiteral("check"));
        check->setGeometry(QRect(10, 120, 75, 41));
        nodeList = new QTableWidget(Widget);
        nodeList->setObjectName(QStringLiteral("nodeList"));
        nodeList->setGeometry(QRect(100, 70, 301, 231));
        nodeList->horizontalHeader()->setStretchLastSection(true);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        msg->setText(QString());
        fresh->setText(QString());
        broadcast->setText(QApplication::translate("Widget", "\345\271\277\346\222\255", Q_NULLPTR));
        check->setText(QApplication::translate("Widget", "\347\202\271\346\222\255", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
