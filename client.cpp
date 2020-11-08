#include <QFileDialog>
#include "client.hpp"

Client::Client(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
    , progressDialog(new ProgressDialog)
{
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_10);

    QObject::connect(tcpSocket, &QTcpSocket::readyRead,
                     this, &Client::readPacket);
    QObject::connect(tcpSocket, &QTcpSocket::disconnected,
                     this, &Client::close);
}

void Client::connect(QString address, int port, QString filename)
{
    // abort previous session
    if (file.isOpen()) file.close();
    tcpSocket->abort();

    fileSize = 0;
    bytesRead = 0;

    file.setFileName(filename);
    file.open(QIODevice::WriteOnly);

    tcpSocket->connectToHost(address, port);
    progressDialog->setUp(1);
}

void Client::close()
{
    if (file.isOpen()) {
        while (tcpSocket->bytesAvailable() > 0)
            readPacket();
        log(tr(u8"<Клиент> Сокет закрыт после чтения %1 байт в %2")
            .arg(bytesRead)
            .arg(file.fileName()));
        progressDialog->tearDown();
        file.close();
    }
}

void Client::readPacket()
{
    in.startTransaction();

    qint64 packetSize;
    QByteArray buffer;

    if (bytesRead == 0) {
        in >> fileSize;
        progressDialog->setUp(fileSize);
    }
    in >> packetSize;
    in >> buffer;

    if (!in.commitTransaction()) {
        log(tr(u8"<Клиент> Не дочитал пакет"));
        return;
    }

    bytesRead += packetSize;
    progressDialog->progress(packetSize);
    log(tr(u8"<Клиент> Прочитал %1/%2")
        .arg(bytesRead)
        .arg(fileSize));
    writePacket(buffer);
}

void Client::writePacket(QByteArray &buffer)
{
    if (!file.isWritable()) {
        log(tr(u8"<Клиент> Ошибка: битый файл"));
        return;
    }
    qint64 bytes = file.write(buffer);
    log(tr(u8"<Клиент> Записал %1 байт в файл").arg(bytes));
}
