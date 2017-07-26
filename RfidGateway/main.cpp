#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("德荣");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("RFID");
    Widget w;
    w.show();

    return a.exec();
}
