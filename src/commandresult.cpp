#include "commandresult.h"

CommandResult::CommandResult(QString se, QString st)
{
    this->sensor = se;
    this->status = st;
}
CommandResult::CommandResult(QString se, QString st, QString res)
{
    this->sensor = se;
    this->status = st;
    this->result = res;
}

CommandResult::CommandResult(QList<QString> data)
{
    if(2 == data.length())
    {
        this->sensor = data.at(0);
        this->status = data.at(1);
    }
    else if(3 == data.length())
    {
        this->sensor = data.at(0);
        this->status = data.at(1);
        this->result = data.at(2);
    }
}
