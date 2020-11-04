#include <QFile>
#include "server.hpp"
#include "mainwindow.hpp"

Server::Server(QObject *parent)
    : QObject(parent)
    , dialog(new ConnectionDialog)
{
    QObject::connect(dialog, &ConnectionDialog::userAccepts,
                     this, &Server::serve);
    QObject::connect(dialog, &ConnectionDialog::userRejects,
                     this, &Server::reject);
}

void Server::listen(int port)
{
    tcpServer = new QTcpServer;

    if (!tcpServer->listen(QHostAddress::LocalHost, port)) {
        error(u8"Сервер не запускается, всё очень плохо");
        tcpServer->close();
        return;
    }

    QObject::connect(tcpServer, &QTcpServer::newConnection,
                     this, &Server::newConnection);
}

void Server::serve(QTcpSocket* conn)
{
    QObject::connect(conn, &QAbstractSocket::disconnected,
                     conn, &QObject::deleteLater);

    qint64 bytesRead = 0, bytesSent = 0, fileSize = 0;
    char* fileContents = new char[256];
    QFile file(filename);

    if (!file.open(QFile::ReadOnly)) {
        conn->disconnectFromHost();
        return;
    }
    fileSize = file.size();

    while (conn->isValid()) {
        bytesRead = file.read(fileContents, Server::blockSize);
        qDebug() << tr(u8"<Сервер> прочитал %1 байт").arg(bytesRead);
        if (bytesRead == 0) break;

        QByteArray block;
        QDataStream data(&block, QIODevice::WriteOnly);

        data.setVersion(QDataStream::Qt_5_10);

        if (bytesSent == 0)
            data << fileSize;
        data << bytesRead;
        data << QByteArray(fileContents, bytesRead);

        conn->write(block);
        bytesSent += block.size();
        qDebug() << tr(u8"<Сервер> Отправлено %1 байт, всего отправлено %2 байт")
                    .arg(block.size())
                    .arg(bytesSent);
    }
    conn->disconnectFromHost();
    file.close();
}

void Server::setFilename(QString name)
{
    filename = name;
}

void Server::close()
{
    if (tcpServer != nullptr)
        tcpServer->close();
}

void Server::newConnection()
{
    auto conn = tcpServer->nextPendingConnection();
    dialog->askUser(conn);
}

void Server::reject(QTcpSocket* conn)
{
    conn->close();
}
