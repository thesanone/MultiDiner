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
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void addPerson();
  void addDebt();
  void deletePerson();

  /// обновление списка вершин графа в комбо боксах панели контроли
  void updatePersonsList();

  /// обновление отображаемого графа
  void updateGraph();

  // actions
  void actionShowControllPanel();
  void actionSaveGraph();
  void actionLoadGraph();
  void actionReduseEdges();

private:
  Ui::MainWindow *ui;

  // Main container
  mg::Multigraph<std::string, double> graph;

  /// Парсер математических выражений
  //MathParser mathParser;

  // GUI elements
  WheelEvent_forQSceneView *view;
  QGraphicsScene *scene;
  QGraphicsSvgItem *svg;
};

#endif // MAINWINDOW_H
