#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = QString();
    if (argc > 1)
    {
      path = QString(argv[1]);
    }
    MainWindow w(path);
    w.show();

    return a.exec();
}
