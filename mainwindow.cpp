#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    try
    {
        graph.addVertex("victor");
        graph.addVertex("Petr");
        graph.addVertex("Oleg");
        graph.addEdge("victor", "Petr", 100);

        graph.addEdge("victor", "Petr", 100);
    }
    catch (mg::Exception e)
    {
        qDebug() << QString::fromStdString(e.text);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
