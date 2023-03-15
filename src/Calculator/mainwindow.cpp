#include "mainwindow.h"

#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>

#include "ui_mainwindow.h"

extern "C" {
#include <notation.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setFixedSize(width(), height());

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digit_numbers()));
  connect(ui->pushButton_credit, SIGNAL(clicked()), this, SLOT(credit_calc()));
  connect(ui->pushButton_leftBrick, SIGNAL(clicked()), this,
          SLOT(operations()));
  connect(ui->pushButton_rightBrick, SIGNAL(clicked()), this,
          SLOT(operations()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_x_graph, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->button_e, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_pi, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(functions()));
  connect(ui->pushButton_delete, SIGNAL(clicked()), this,
          SLOT(on_pushButton_delete_clicked()));
  connect(ui->pushButton_enter, SIGNAL(clicked()), this, SLOT(result()));
  connect(ui->pushButton_del, SIGNAL(clicked()), this,
          SLOT(delete_one_simbol()));
  connect(ui->pushButton_ostatok, SIGNAL(clicked()), this, SLOT(operations()));
  connect(ui->pushButton_graph, SIGNAL(clicked()), this, SLOT(window_graph()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::digit_numbers() {
  QPushButton *button = (QPushButton *)sender();
  QString new_label = ui->label->text() + button->text();
  ui->label->setText(new_label);
  if (new_label.size() >= 255) {
    ui->label->setText("ERROR");
  }
}

void MainWindow::operations() {
  QPushButton *button = (QPushButton *)sender();
  QString new_label;
  new_label = ui->label->text();

  if (button->text() == "÷" && !(ui->label->text().endsWith("÷")) &&
      !(ui->label->text().endsWith("/"))) {
    new_label += "/";
  } else if (button->text() == "*" && !(ui->label->text().endsWith("x")) &&
             !(ui->label->text().endsWith("*"))) {
    new_label += "*";
  } else if (button->text() == "+" && !(ui->label->text().endsWith("+"))) {
    new_label += "+";
  } else if (button->text() == "-" && !(ui->label->text().endsWith("-"))) {
    new_label += "-";
  } else if (button->text() == "n²" && !(ui->label->text().endsWith("^"))) {
    new_label += "^";
  } else if (button->text() == "%" && !(ui->label->text().endsWith("%"))) {
    new_label += "%";
  } else if (button->text() == "(") {
    new_label += "(";
  } else if (button->text() == ")") {
    new_label += ")";
  } else if (button->text() == "." && !(ui->label->text().endsWith("."))) {
    new_label += ".";
  } else if (button->text() == "e" && !(ui->label->text().endsWith("e"))) {
    new_label += "2.7182818284";
  } else if (button->text() == "π" && !(ui->label->text().endsWith("π"))) {
    new_label += "3.1415926535";
  } else if (button->text() == "x" && !(ui->label->text().endsWith("x"))) {
    new_label += "x";
  }

  ui->label->setText(new_label);
}

void MainWindow::functions() {
  QPushButton *button = (QPushButton *)sender();
  QString new_label;
  new_label = ui->label->text() + button->text() + "(";
  ui->label->setText(new_label);
}

void MainWindow::delete_one_simbol() {
  QString text = ui->label->text();
  text.chop(1);
  ui->label->setText(text);
}

void MainWindow::on_pushButton_delete_clicked() { ui->label->setText(""); }

void MainWindow::window_graph() {
  QString line = ui->label->text();

  PlotWindow *graphPlot = new PlotWindow;
  graphPlot->draw_expression(line.toStdString().c_str());
  graphPlot->show();
}

void MainWindow::credit_calc() {
  CreditCalc *depos = new CreditCalc;
  depos->show();
}

void MainWindow::result() {
  QString line = ui->label->text();
  const char *line_ptr = line.toStdString().c_str();

  double result = 0.0;
  int err = compute_line(line_ptr, &result);
  if (err)
    ui->label->setText("ERROR");
  else
    ui->label->setText(QString::number(result));
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_0)
    ui->pushButton_0->click();
  else if (e->key() == Qt::Key_1)
    ui->pushButton_1->click();
  else if (e->key() == Qt::Key_2)
    ui->pushButton_2->click();
  else if (e->key() == Qt::Key_3)
    ui->pushButton_3->click();
  else if (e->key() == Qt::Key_4)
    ui->pushButton_4->click();
  else if (e->key() == Qt::Key_5)
    ui->pushButton_5->click();
  else if (e->key() == Qt::Key_6)
    ui->pushButton_6->click();
  else if (e->key() == Qt::Key_7)
    ui->pushButton_7->click();
  else if (e->key() == Qt::Key_8)
    ui->pushButton_8->click();
  else if (e->key() == Qt::Key_9)
    ui->pushButton_9->click();
  else if (e->key() == Qt::Key_Minus)
    ui->pushButton_minus->click();
  else if (e->key() == Qt::Key_Plus)
    ui->pushButton_plus->click();
  else if (e->key() == Qt::Key_Slash)
    ui->pushButton_div->click();
  else if (e->key() == Qt::Key_Asterisk)
    ui->pushButton_mul->click();
  else if (e->key() == Qt::Key_Equal || Qt::Key_Enter)
    ui->pushButton_enter->click();
  else if (e->key() == Qt::Key_Backspace)
    ui->pushButton_del->click();
  else if (e->key() == Qt::Key_Period)
    ui->pushButton_dot->click();
  else if (e->key() == Qt::Key_Return)
    ui->pushButton_delete->click();
}
