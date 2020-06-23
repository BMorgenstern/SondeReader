#ifndef SERIALPARSER_H
#define SERIALPARSER_H

#include <QObject>
#include <QDebug>

class SerialParser : public QObject
{
    Q_OBJECT
    QString status;

public:
    QThread* worker;
    explicit SerialParser(QObject *parent = nullptr);


private slots:
    void parse(QString);

signals:
    void parsedResult(QString);
    void calibrationResult(QString);
    void reading(QString);

};

#endif // SERIALPARSER_H
