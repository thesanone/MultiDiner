#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "multigraph.h"

#include <QMainWindow>

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
    mg::Multigraph<std::string, double> graph;
};

#endif // MAINWINDOW_H
