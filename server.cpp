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

    if (!tcpServer->listen(QHostAddress::Any, port)) {
        error(u8"Сервер не запускается, всё очень плохо");
        tcpServer->close();
        return;
    }

    QObject::connect(tcpServer, &QTcpServer::newConnection,
                     this, &Server::newConnection);
}

void Server::serve(QTcpSocket* conn)
{
    QFile file(filename);

    if (!file.open(QFile::ReadOnly)) {
        conn->close();
        return;
    }

    QDataStream in(&file);
    QByteArray block;
    in.setVersion(QDataStream::Qt_5_10);

    in >> block;

    QObject::connect(conn, &QAbstractSocket::disconnected,
                     conn, &QObject::deleteLater);

//    QObject::connect(conn, &QIODevice::bytesWritten,
//                     conn, &QObject::deleteLater);

    conn->write(block);
    conn->disconnectFromHost();
}

void Server::setFilename(QString name)
{
    filename = name;
}

void Server::close()
{
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
