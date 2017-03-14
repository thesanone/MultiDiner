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

private:
  Ui::MainWindow *ui;

  // Main container
  mg::Multigraph<std::string, double> graph;

  // GUI elements
  WheelEvent_forQSceneView *view;
  QGraphicsScene *scene;
  QGraphicsSvgItem *svg;
};

#endif // MAINWINDOW_H
