#ifndef CREDITCALC_H
#define CREDITCALC_H

#include <QMainWindow>
#include <QMessageBox>
#include <QWidget>

#include "mainwindow.h"

namespace Ui {
class CreditCalc;
}

class CreditCalc : public QWidget {
  Q_OBJECT

 public:
  explicit CreditCalc(QWidget *parent = nullptr);
  ~CreditCalc();

 private:
  Ui::CreditCalc *ui;

 private slots:
  void calc_credit();
};

#endif  // CREDITCALC_H
