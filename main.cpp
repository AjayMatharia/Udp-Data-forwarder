#include "mainwindow.h"

#include <QApplication>
#include <QIcon>


int main(int argc, char *argv[])
{
    // Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/Icons/resources/icons/icon.svg"));
    w.showMaximized();
    return a.exec();
}
