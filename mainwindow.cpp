#include "mainwindow.h"
#include "window.h"

#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow()
{
  setCentralWidget(new Window(this));
  showMaximized();
}
