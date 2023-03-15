#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QWidget>
#include <Qt>

#include "creditcalc.h"
#include "plotwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;

 private slots:
  void digit_numbers();
  void operations();
  void functions();
  void delete_one_simbol();
  void on_pushButton_delete_clicked();
  void result();
  void window_graph();
  void credit_calc();
  void keyPressEvent(QKeyEvent *e) override;
};

#endif  // MAINWINDOW_H
