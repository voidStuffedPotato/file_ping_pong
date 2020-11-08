#ifndef SQLITELOGGER_HPP
#define SQLITELOGGER_HPP

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

class SQLiteLogger : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteLogger(QObject *parent = nullptr);
    ~SQLiteLogger();
    void write(QString data);

signals:
private:
    QSqlDatabase db;

};

#endif // SQLITELOGGER_HPP
