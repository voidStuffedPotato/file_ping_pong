#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QTcpServer>
#include "connectiondialog.hpp"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void listen(int port);
    void close();

signals:
    void error(QString err);

public slots:
    void serve(QTcpSocket* conn);
    void reject(QTcpSocket* conn);
    void newConnection();
    void setFilename(QString name);

private:
    QTcpServer *tcpServer = nullptr;
    QString filename;
    ConnectionDialog *dialog = nullptr;

};

#endif // SERVER_HPP
