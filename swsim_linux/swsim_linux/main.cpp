#include "dialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("生态流量监测客户端软件");
    w.show();
    w.hide();

    return a.exec();
}
