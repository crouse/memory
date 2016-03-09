#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.setStyleSheet("QLabel {color: rgb(122, 122, 122);}");

    return a.exec();
}
