#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../ThirdParty/tinyexpr-master/tinyexpr.h"
#include <algorithm>

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  try
  {
    graph.addVertex("Viktor");
    graph.addVertex("Danil");
    graph.addVertex("Kirill");
    graph.addVertex("Andrey");
    graph.addEdge("Viktor", "Danil", 38);
    graph.addEdge("Kirill", "Andrey", 100);
    graph.addEdge("Danil", "Kirill", 21.5);
    graph.addEdge("Kirill", "Andrey", 5.5);
    graph.addEdge("Kirill", "Viktor", 51);
    graph.addEdge("Andrey", "Danil", 0.5);
    graph.addEdge("Andrey", "Viktor", 999);
  }
  catch (mg::Exception e)
  {
    QMessageBox::critical(this,"Error!", QString::fromStdString(e.text), QMessageBox::Ok);
  }

  view = new WheelEvent_forQSceneView(this);
  view->setDragMode(QGraphicsView::ScrollHandDrag);
  ui->horizontalLayout_central->addWidget(view);
  scene = new QGraphicsScene(view);
  view->setScene(scene);
  svg = new QGraphicsSvgItem("file.svg");
  scene->addItem(svg);

  // pushbuttons
  connect(ui->pushButton_addPerson, SIGNAL(pressed()), this, SLOT(addPerson()));
  connect(ui->pushButton_AddDebt, SIGNAL(pressed()), this, SLOT(addDebt()));
  connect(ui->pushButton_deletePerson, SIGNAL(pressed()), this, SLOT(deletePerson()));

  //actions
  connect(ui->actionShow_controll_panel, SIGNAL(triggered(bool)), this, SLOT(actionShowControllPanel()));
  connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(actionSaveGraph()));
  connect(ui->actionLoad_graph, SIGNAL(triggered(bool)), this, SLOT(actionLoadGraph()));
  connect(ui->actionReduce_edges, SIGNAL(triggered(bool)), this, SLOT(actionReduseEdges()));

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
    QString text = ui->lineEdit_person->text();
    text.replace(" ", "_");
    graph.addVertex(text.toLocal8Bit().constData());
    updatePersonsList();
  }
  catch (mg::Exception e)
  {
    QMessageBox::critical(this,"Error!", QString::fromStdString(e.text), QMessageBox::Ok);
  }
}

void MainWindow::addDebt()
{
  try
  {
    int error = 0;
    double value = te_interp(ui->lineEdit_debt->text().toLocal8Bit().constData(), &error);
    if (error != 0 )
    {
      QMessageBox::critical(this,"Error!", "'"+ui->lineEdit_debt->text()+"' parse error!", QMessageBox::Ok);
      return;
    }
    graph.addEdge(
          ui->comboBox_creditor->currentText().toLocal8Bit().constData(),
          ui->comboBox_debtor->currentText().toLocal8Bit().constData(),
          value);
  }
  catch (mg::Exception e)
  {
    QMessageBox::critical(this,"Error!", QString::fromStdString(e.text), QMessageBox::Ok);
  }
  updateGraph();
}

void MainWindow::deletePerson()
{
  try
  {
    std::string delVertex = ui->comboBox_personsList->currentText().toLocal8Bit().constData();
    if(!graph.vertexIsIsolated(delVertex))
    {
      QMessageBox::StandardButton reply = QMessageBox::question(this,"Warning", "Vertex '"+ui->comboBox_personsList->currentText()+"' isn't isolated\n"
                            "Removal of this vertex will lead to the removal of all edges leading to it!\n"
                            "Continue?", QMessageBox::Cancel | QMessageBox::Yes);
      //QMessageBox::critical(this,"Error!", QString::fromStdString(e.text), QMessageBox::Ok);
      if (reply == QMessageBox::Yes)
        graph.deleteVertex(delVertex);
      else
        return;
    }
    else
      graph.deleteVertex(delVertex);
  }
  catch (mg::Exception e)
  {
    QMessageBox::critical(this,"Error!", QString::fromStdString(e.text), QMessageBox::Ok);
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
    QMessageBox::critical(this,"Error!", QString::fromStdString(e.text), QMessageBox::Ok);
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

void MainWindow::actionShowControllPanel()
{
  ui->dockWidget_controll->show();
}

void MainWindow::actionSaveGraph()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "",
          tr("(*.mg)"));

  if (!fileName.isEmpty())
  {
    std::ofstream outputFile;
    outputFile.open(fileName.toLocal8Bit().constData());
    outputFile << graph;
    outputFile.close();
  }
}

void MainWindow::actionLoadGraph()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
             tr("*.mg"));

  if (!fileName.isEmpty())
  {
    std::ifstream inputFile;
    inputFile.open(fileName.toLocal8Bit().constData());
    graph.clear();
    inputFile >> graph;
    inputFile.close();
    updateGraph();
  }
}

void MainWindow::actionReduseEdges()
{

}
