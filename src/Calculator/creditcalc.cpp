#include "creditcalc.h"

#include "ui_creditcalc.h"

CreditCalc::CreditCalc(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  setFixedSize(width(), height());
  connect(ui->Diff, SIGNAL(clicked()), this, SLOT(calc_credit()));
  connect(ui->Annuit, SIGNAL(clicked()), this, SLOT(calc_credit()));
}

CreditCalc::~CreditCalc() { delete ui; }

void CreditCalc::calc_credit() {
  QPushButton *button = (QPushButton *)sender();
  QString summ = ui->line_sum_credit->text();
  QString procent = ui->line_stavka->text();
  QString term = ui->line_srok->text();

  if (!(summ.isEmpty() && procent.isEmpty() && term.isEmpty())) {
    double summ_number = summ.toDouble();
    double procent_number = procent.toDouble();
    double term_number = term.toDouble();

    double pay_mounth = 0;
    double total_summ = 0;
    double number_inter = summ_number;

    QString first_mounth_pay;
    QString mounth_pay;

    if (button == ui->Diff) {
      for (int i = 1; i <= term_number; ++i) {
        pay_mounth = summ_number / term_number +
                     number_inter * procent_number / 12.0 / 100;
        total_summ += pay_mounth;
        number_inter = summ_number - i * summ_number / term_number;
        mounth_pay = QString("%L1").arg(pay_mounth, 0, 'f', 2);
        if (i == 1) {
          first_mounth_pay = mounth_pay;
        }
        mounth_pay = first_mounth_pay + " - " + mounth_pay;
      }
    }
    if (button == ui->Annuit) {
      double proc_stavka = procent_number / 12 / 100;
      pay_mounth = summ_number *
                   (proc_stavka * pow(1 + proc_stavka, term_number)) /
                   (pow(1 + proc_stavka, term_number) - 1);
      total_summ = pay_mounth * term_number;
      mounth_pay = QString("%L1").arg(pay_mounth, 0, 'f', 2);
    }

    ui->mounth_pay->setText(mounth_pay);
    double overPay = total_summ - summ_number;
    ui->final_sum->setText(QString("%L1").arg(total_summ, 0, 'f', 2));
    ui->over_pay->setText(QString("%L1").arg(overPay, 0, 'f', 2));
  } else {
    QMessageBox::critical(this, "Fail", "Ошибка ввода");
  }
}
