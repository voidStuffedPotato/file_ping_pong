#include <QFileDialog>
#include "connectiondialog.hpp"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::askUser(QTcpSocket *conn)
{
    this->conn = conn;
    QString text = tr(u8"Входящее подключение от %1:%2. Принять?").arg(conn->peerName()).arg(conn->peerPort());
    ui->connection_text->setText(text);
    show();
}

void ConnectionDialog::on_buttonBox_accepted()
{
    hide();
    emit userAccepts(conn);
}

void ConnectionDialog::on_buttonBox_rejected()
{
    hide();
    emit userRejects(conn);
}
