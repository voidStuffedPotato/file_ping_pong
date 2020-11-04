#ifndef SQLITELOGGER_HPP
#define SQLITELOGGER_HPP

#include <QObject>

class SQLiteLogger : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteLogger(QObject *parent = nullptr);
    void write(QString data);

signals:

};

#endif // SQLITELOGGER_HPP
