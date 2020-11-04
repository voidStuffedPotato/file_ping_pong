#include <QMessageBox>
#include <QFileDialog>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "connectiondialog.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , server(this)
{
    ui->setupUi(this);
    QObject::connect(&server, &Server::error,
                     this, &MainWindow::displayError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayError(QString msg)
{
    QMessageBox::critical(this, tr(u8"Ошибка"), msg);
}

void MainWindow::on_set_listener_clicked()
{
    bool ok;
    int port = ui->listener_port->toPlainText().toInt(&ok);
    if (!ok) {
        displayError(tr(u8"Порт должен быть целым числом"));
        return;
    }

    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr(u8"Выберите раздаваемый файл"),
                                            QDir::homePath(), tr("All files (*)"));
    if (filename.isEmpty()) {
        displayError(tr(u8"Файл не выбран"));
        return;
    }

    server.setFilename(filename);
    server.close();
    server.listen(port);
}
