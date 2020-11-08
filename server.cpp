#include <QFile>
#include <QThread>
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
    close();
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
    char* fileContents = new char[Server::blockSize];
    QFile file(filename);

    log(tr(u8"<Сервер> Устанавливаем соединение с %1:%2")
        .arg(conn->peerAddress().toString())
        .arg(conn->peerPort()));

    if (!file.open(QFile::ReadOnly)) {
        log(tr(u8"<Сервер> ошибка при чтении файла %1")
            .arg(filename));

        conn->disconnectFromHost();
        return;
    }
    fileSize = file.size();

    while (conn->isValid()) {
        bytesRead = file.read(fileContents, Server::blockSize);
        if (bytesRead == 0) break;

        QByteArray block;
        QDataStream data(&block, QIODevice::WriteOnly);
        data.setVersion(QDataStream::Qt_5_10);

        if (bytesSent == 0)
            data << fileSize;
        data << bytesRead;
        data << QByteArray(fileContents, bytesRead);

        conn->write(block);
        conn->flush();

        bytesSent += block.size();

        QThread::msleep(1);
    }
    conn->disconnectFromHost();
    file.close();
    log(u8"<Сервер> Файл отправлен, соединение закрыто");
}

void Server::setFilename(QString name)
{
    filename = name;
}

void Server::close()
{
    if (tcpServer != nullptr) {
        tcpServer->close();
        log(u8"<Сервер> Соединение закрыто");
    }
}

void Server::newConnection()
{
    auto conn = tcpServer->nextPendingConnection();
    log(u8"<Сервер> Новое соединение");
    dialog->askUser(conn);
}

void Server::reject(QTcpSocket* conn)
{
    log(u8"<Сервер> Соединение разорвано");
    conn->close();
    delete conn;
}
