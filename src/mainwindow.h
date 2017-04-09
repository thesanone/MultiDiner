#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "wheelevent_forqsceneview.h"
#include "multigraph.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QString graphPath = QString(), QWidget *parent = 0);
  ~MainWindow();

private slots:
  void addPerson();
  void addDebt();
  void deletePerson();

  void updatePersonsList();

  void updateGraph();

  // actions
  void actionShowControllPanel();
  void actionSaveGraph();
  void actionLoadGraph();
  void actionReduseEdges();


  //settings
  void readSettings(QString file, QString group = "MainWindow");
  void writeSettings(QString file, QString group = "MainWindow");

private:
  Ui::MainWindow *ui;

  // Graph invariant check
  bool checkGraphInvariant();

  // Main container
  mg::Multigraph<std::string, double> graph;

  // GUI elements
  WheelEvent_forQSceneView *view;
  QGraphicsScene *scene;
  QGraphicsSvgItem *svg;
};

#endif // MAINWINDOW_H
