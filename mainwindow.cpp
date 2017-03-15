#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <algorithm>

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

  connect(ui->pushButton_addPerson, SIGNAL(pressed()), this, SLOT(addPerson()));
  connect(ui->pushButton_AddDebt, SIGNAL(pressed()), this, SLOT(addDebt()));
  connect(ui->pushButton_deletePerson, SIGNAL(pressed()), this, SLOT(deletePerson()));

  updatePersonsList();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::addPerson()
{
  try
  {
    graph.addVertex(ui->lineEdit_person->text().toLocal8Bit().constData());
    updatePersonsList();
  }
  catch (mg::Exception e)
  {
    qDebug() << QString::fromStdString(e.text);
  }
}

void MainWindow::addDebt()
{
  try
  {
    graph.addEdge(
          ui->comboBox_creditor->currentText().toLocal8Bit().constData(),
          ui->comboBox_debtor->currentText().toLocal8Bit().constData(),
          mathParser.Calc(ui->lineEdit_debt->text().toLocal8Bit().constData()));
  }
  catch (mg::Exception e)
  {
    qDebug() << QString::fromStdString(e.text);
  }
  updateGraph();
}

void MainWindow::deletePerson()
{
  try
  {
    graph.deleteVertex(ui->comboBox_personsList->currentText().toLocal8Bit().constData());
  }
  catch (mg::Exception e)
  {
    qDebug() << QString::fromStdString(e.text);
  }
  updatePersonsList();
}

void MainWindow::updatePersonsList()
{
  ui->comboBox_creditor->clear();
  ui->comboBox_debtor->clear();
  ui->comboBox_personsList->clear();

  std::vector<std::string> vertexes = graph.getVertexes();
  std::for_each (vertexes.begin(), vertexes.end(), [this](std::string i)
  {
    QString name = QString::fromStdString(i);
    ui->comboBox_creditor->addItem(name);
    ui->comboBox_debtor->addItem(name);
    ui->comboBox_personsList->addItem(name);
  });
  updateGraph();
}

void MainWindow::updateGraph()
{
  try
  {
    graph.generateDotText("file.dot");
  }
  catch (mg::Exception e)
  {
    qDebug() << QString::fromStdString(e.text);
  }

#if defined(_WIN32) || defined(_WIN64)
  system("%DOT_DIR%\\bin\\dot.exe file.dot -Tsvg -ofile.svg");
#else
  system("dot file.dot -Tsvg -ofile.svg");
#endif

  scene->removeItem(svg);
  delete svg;
  svg = new QGraphicsSvgItem("file.svg");
  scene->addItem(svg);
}
