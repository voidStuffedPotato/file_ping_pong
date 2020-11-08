#include "sqlitelogger.hpp"
#include <QtDebug>

SQLiteLogger::SQLiteLogger(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ping_pong_logger");

    bool ok = db.open();
    if (!ok) exit(1);

    QSqlQuery query("CREATE TABLE IF NOT EXISTS Log ( \"text\" TEXT );");
    if (query.exec())
        qDebug() << u8"Я здесь";
}

void SQLiteLogger::write(QString data)
{
    db.transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO Log VALUES (:text)");
    query.bindValue(":text", QVariant(data));
    query.exec();
    db.commit();
    QSqlQuery select("SELECT * FROM Log");
    if (select.size() > 0)
        qDebug() << tr(u8"Прочитано %1 записей").arg(select.size());
}

SQLiteLogger::~SQLiteLogger()
{
    db.close();
    QSqlDatabase::removeDatabase("ping_pong_logger");
}
