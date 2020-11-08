#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QString>
#include "server.hpp"
#include "client.hpp"
#include "sqlitelogger.hpp"
#include "connectiondialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayError(QString msg);

private slots:
    void on_set_listener_clicked();
    void on_receive_clicked();
    void log(QString data);

private:
    bool setClient(QString address, int port);

    Ui::MainWindow *ui;
    Client client;
    Server server;
    SQLiteLogger *logger;
};
#endif // MAINWINDOW_HPP
