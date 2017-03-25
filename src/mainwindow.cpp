#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../ThirdParty/tinyexpr-master/tinyexpr.h"
#include <algorithm>

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#define MD_TRY try {
#define MD_CATCH }\
  catch (std::exception& e)\
  {\
    QMessageBox::critical(this,"Error!", QString(e.what()), QMessageBox::Ok);\
  }

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  MD_TRY
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
  MD_CATCH




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
  /*try
  {
    auto i = graph.beginV();
  for(;;)
  {
    qDebug() << QString::fromStdString((*i)->getData());
    ++i;
  }

//  std::for_each(graph.beginV(), graph.endV(), [](mg::Vertex<std::string, double>* i)
//                {qDebug() << QString::fromStdString(i->getData());});
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(this,"Error!", QString(e.what()), QMessageBox::Ok);
  }*/

  QString text = ui->lineEdit_person->text();
  text.replace(" ", "_");
  if(text.isEmpty())
  {
    QMessageBox::critical(this,"Error!", "Can't add nameless person", QMessageBox::Ok);
    return;
  }

  MD_TRY
  graph.addVertex(text.toLocal8Bit().constData());
  updatePersonsList();
  MD_CATCH
}

void MainWindow::addDebt()
{
  MD_TRY
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
  MD_CATCH
  updateGraph();
}

void MainWindow::deletePerson()
{
  MD_TRY
  std::string delVertex = ui->comboBox_personsList->currentText().toLocal8Bit().constData();
  if(!graph.vertexIsIsolated(delVertex))
  {
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Warning", "Vertex '"+ui->comboBox_personsList->currentText()+"' isn't isolated\n"
                          "Removal of this vertex will lead to the removal of all edges leading to it!\n"
                          "Continue?", QMessageBox::Cancel | QMessageBox::Yes);
    //QMessageBox::critical(this,"Error!", QString(e.what()), QMessageBox::Ok);
    if (reply == QMessageBox::Yes)
      graph.deleteVertex(delVertex);
    else
      return;
  }
  else
    graph.deleteVertex(delVertex);
  MD_CATCH
  updatePersonsList();
}

void MainWindow::updatePersonsList()
{
  ui->comboBox_creditor->clear();
  ui->comboBox_debtor->clear();
  ui->comboBox_personsList->clear();

  MD_TRY
  auto vertexes = graph.getVertexes();

  std::for_each (vertexes.begin(), vertexes.end(), [this](mg::Vertex<std::string, double>* i)
  {
    QString name = QString::fromStdString(i->getData());
    ui->comboBox_creditor->addItem(name);
    ui->comboBox_debtor->addItem(name);
    ui->comboBox_personsList->addItem(name);
  });
  MD_CATCH
  updateGraph();
}

void MainWindow::updateGraph()
{
  MD_TRY
  graph.generateDotText("file.dot");
  MD_CATCH

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
  MD_TRY

  // step 1
  std::for_each(graph.beginV(), graph.endV(), [this](mg::Vertex<std::string, double>* i)
  {
    auto outgoingEdges = i->getOutgoingEdges();
    for(auto j = outgoingEdges.begin(); j != outgoingEdges.end(); ++j)
    {
      auto k = j;
      ++k;
      while(k!= outgoingEdges.end())
      {
        if((*j)->getDestenation()->getData() ==
           (*k)->getDestenation()->getData())
        {
          double current = (*j)->getValue();
          (*j)->setValue(current + (*k)->getValue());
          mg::Edge<std::string, double>* delEdgeP = *k;
          auto oldKpos = k;
          ++k;
          graph.deleteEdge(delEdgeP);
          outgoingEdges.erase(oldKpos);
        }
        else ++k;
      }
    }
  });

  // step 2
  std::for_each(graph.beginV(), graph.endV(), [this](mg::Vertex<std::string, double>* i)
  {
    auto outgoingEdges = i->getOutgoingEdges();
    std::for_each(outgoingEdges.begin(), outgoingEdges.end(), [this](mg::Edge<std::string, double>* j)
    {
      auto dst = j->getDestenation();
      auto dstOutgoingEdges = dst->getOutgoingEdges();
      auto reverseEdgePos =
          std::find_if(dstOutgoingEdges.begin(), dstOutgoingEdges.end(),
                       [j](mg::Edge<std::string, double>* k)
      {
        return j->getSource() == k->getDestenation();
      });

      if (reverseEdgePos != dstOutgoingEdges.end())
      {
        auto reverseEdge = *reverseEdgePos;
        double outgoingVal = j->getValue();
        double incomingVal = reverseEdge->getValue();
        if(outgoingVal > incomingVal)
        {
          j->setValue(outgoingVal - incomingVal);
          graph.deleteEdge(reverseEdge);
        }
        else
          if(outgoingVal < incomingVal)
          {
            reverseEdge->setValue(incomingVal - outgoingVal);
            graph.deleteEdge(j);
          }
          else
          {
            graph.deleteEdge(reverseEdge);
            graph.deleteEdge(j);
          }
      }
    });
  });
  MD_CATCH

  updateGraph();
}
