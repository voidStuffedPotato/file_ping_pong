#include <QtDebug>
#include <QDir>
#include <QDateTime>
#include <QProcess>
#include "sqlitelogger.hpp"

SQLiteLogger::SQLiteLogger(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath() + QDir::separator() + "ping_pong_logger.db");

    bool ok = db.open();
    if (!ok) {
        qDebug() << u8"Ошибка соединения с базой данных";
    }

    QSqlQuery query("CREATE TABLE IF NOT EXISTS Log ( \"text\" TEXT );");
    if (!query.exec())
        qDebug() << u8"Ошибка: запрос на создание таблицы не выполнен";
}

void SQLiteLogger::write(QString text)
{
    QSqlQuery query;

    auto now = QDateTime::currentDateTime();
    text = now.toString("[dd.MM.yyyy hh:mm:ss] ") + text;

    query.prepare("INSERT INTO Log VALUES (:text)");
    query.bindValue(":text", QVariant(text));

    if (!query.exec())
        qDebug() << u8"Ошибка записи в БД";
}

SQLiteLogger::~SQLiteLogger()
{
    db.close();
    QSqlDatabase::removeDatabase(db.databaseName());
}
