#ifndef COMMANDRESULT_H
#define COMMANDRESULT_H

#include <QList>
#include <QString>

class CommandResult
{
    QString sensor;
    QString status;
    QString result;
public:
    CommandResult(QString, QString);
    CommandResult(QString, QString, QString);
    CommandResult(QList<QString>);
    inline QString getSensor()
    {
        return this->sensor;
    }
    inline QString getStatus()
    {
        return this->status;
    }
    inline QString getResult()
    {
        return this->result;
    }
};

#endif // COMMANDRESULT_H
