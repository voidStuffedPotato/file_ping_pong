#ifndef CONNECTIONDIALOG_HPP
#define CONNECTIONDIALOG_HPP

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();
    void askUser(QTcpSocket *conn);

signals:
    void userAccepts(QTcpSocket* conn);
    void userRejects(QTcpSocket* conn);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ConnectionDialog *ui;
    QTcpSocket *conn = nullptr;
};

#endif // CONNECTIONDIALOG_HPP
