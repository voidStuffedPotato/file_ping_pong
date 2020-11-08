#include <QMessageBox>
#include <QFileDialog>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "connectiondialog.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , server(this)
    , logger(new SQLiteLogger)
{
    ui->setupUi(this);
    QObject::connect(&server, &Server::error,
                     this, &MainWindow::displayError);
    QObject::connect(&server, &Server::log,
                     this, &MainWindow::log);
    QObject::connect(&client, &Client::log,
                     this, &MainWindow::log);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete logger;
}

void MainWindow::displayError(QString msg)
{
    QMessageBox::critical(this, tr(u8"Ошибка"), msg);
    log(u8"Ошибка: " + msg);
}

void MainWindow::on_set_listener_clicked()
{
    bool ok;
    int port = ui->listener_port->toPlainText().toInt(&ok);
    if (!ok) {
        displayError(tr(u8"Порт должен быть целым числом"));
        log(u8"<Сервер> Ошибка: порт должен быть целым числом");
        return;
    }

    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr(u8"Выберите раздаваемый файл"),
                                            QDir::homePath(), tr("All files (*)"));
    if (filename.isEmpty()) {
        displayError(tr(u8"Файл не выбран"));
        log(u8"<Сервер> Ошибка: Файл не выбран");
        return;
    }

    server.setFilename(filename);
//    server.close();
    server.listen(port);
}

void MainWindow::on_receive_clicked()
{
    QString addr = ui->server_address->toPlainText();
    bool ok;
    int port = ui->server_port->toPlainText().toInt(&ok);
    if (ok) {
        QString filename;
        filename = QFileDialog::getSaveFileName(nullptr, tr(u8"Выберите файл для сохранения"),
                                                QDir::homePath(), tr("All files (*)"));
        if (filename.isEmpty()) return;

        log(tr(u8"<Клиент> Подключаюсь к %1:%2").arg(addr).arg(port));
        client.connect(addr, port, filename);
    } else {
        displayError(tr(u8"Порт должен быть целым числом"));
        log(u8"<Клиент> Ошибка: порт должен быть целым числом");
    }
}

void MainWindow::log(QString data)
{
    logger->write(data);
}
