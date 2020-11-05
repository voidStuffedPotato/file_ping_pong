#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setUp(qint64 size)
{
    ui->progressBar->setMaximum(size);
    this->show();
}

void ProgressDialog::progress(qint64 size)
{
    ui->progressBar->setValue(ui->progressBar->value() + size);
//    ui->progressBar->valueChanged(ui->progressBar->value() + size);
}

void ProgressDialog::tearDown()
{
    ui->progressBar->reset();
    this->hide();
}
