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

    // 2KB block size
    static constexpr quint64 blockSize = 2 << 10;

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
    quint64 bytesSent = 0;
    ConnectionDialog *dialog = nullptr;

};

#endif // SERVER_HPP
