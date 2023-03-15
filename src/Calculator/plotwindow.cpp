#include "plotwindow.h"

#include <QMessageBox>
#include <cmath>

#include "ui_plotwindow.h"

#define LEN 1000000

extern "C" {
#include <notation.h>
}

PlotWindow::PlotWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::PlotWindow) {
  ui->setupUi(this);
  setFixedSize(width(), height());
  setAttribute(Qt::WA_DeleteOnClose);
  ui->qplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

PlotWindow::~PlotWindow() { delete ui; }

void PlotWindow::draw_expression(const char* line) {
  double min_X = -10000;
  double max_X = 10000;

  QList<double> x_line(LEN);
  QList<double> y_line(LEN);

  double step = (fabs(min_X) + fabs(max_X)) / LEN;

  for (int i = 0; i < LEN; i++) {
    x_line[i] = step * i + min_X;
  }

  int err = compute_graphic(line, x_line.data(), y_line.data(), LEN);
  if (err) {
    QMessageBox::critical(this, "ERROR", "Ошибка ввода данных");
    close();
  } else {
    ui->qplot->addGraph();
    ui->qplot->graph(0)->setData(x_line, y_line);
    ui->qplot->xAxis->setLabel("x");
    ui->qplot->yAxis->setLabel("y");
    ui->qplot->xAxis->setRange(-10, 10);
    ui->qplot->yAxis->setRange(-2.5, 2.5);
    ui->qplot->replot();
  }
}
