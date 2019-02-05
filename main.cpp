#include "mainwindow.h"
#include "procdialog.h"
#include <QApplication>
#include <QObject>
#include <any>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}
