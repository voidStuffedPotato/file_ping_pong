#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QFile>
#include <QTcpSocket>
#include <QDataStream>
#include "progressdialog.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connect(QString address, int port, QString filename);

signals:
    void log(QString msg);

public slots:
    void close();

private slots:
    void readPacket();

private:
    void writePacket(QByteArray &buffer);

    QFile file;
    QDataStream in;
    QTcpSocket* tcpSocket = nullptr;
    ProgressDialog *progressDialog = nullptr;
    qint64 fileSize = 0;
    qint64 bytesRead = 0;
};

#endif // CLIENT_HPP
