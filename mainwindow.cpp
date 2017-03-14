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
        graph.addEdge("Oleg", "Petr", 100);
        graph.addEdge("victor", "Petr", 100);

        graph.generateDotText("file.dot");
        system("dot file.dot -Tsvg -ofile.svg");
    }
    catch (mg::Exception e)
    {
        qDebug() << QString::fromStdString(e.text);
    }

    view = new WheelEvent_forQSceneView(this);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->horizontalLayout_central->addWidget(view);
    scene = new QGraphicsScene(view);
    view->setScene(scene);
    svg = new QGraphicsSvgItem("file.svg");
    scene->addItem(svg);
}

MainWindow::~MainWindow()
{
    delete ui;
}
