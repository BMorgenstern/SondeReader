#include "serialmonitor.h"
#include "ui_serialmonitor.h"

SerialMonitor::SerialMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialMonitor)
{
    ui->setupUi(this);
    this->setWindowTitle("Serial Monitor");
}

void SerialMonitor::initPort(SerialIO* p)
{
    this->port = p;
    QObject::connect(this->port, SIGNAL(doneReading(QString)), this, SLOT(printToMonitor(QString)));
    QObject::connect(this, SIGNAL(writeFromMonitor(QString)), this->port, SLOT(serialWrite(QString)));
    SerialMonitor();
}

SerialMonitor::~SerialMonitor()
{
    delete ui;
}

void SerialMonitor::printToMonitor(QString message)
{
    this->ui->output->append(message);
}

void SerialMonitor::on_userSubmit_clicked()
{
    emit writeFromMonitor(this->ui->userInput->displayText());
    this->ui->userInput->clear();
}
