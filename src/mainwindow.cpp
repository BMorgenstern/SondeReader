#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sensordetail.h"
#include "serialio.h"
#include <sstream>


#undef TESTING_SERIALIO

//#define CONNECT_TO_ARDUINO


void MainWindow::fun1(QString message)
{
    qDebug() << message;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pressureOptions->setGeometry(ui->tempOptions->geometry());
    ui->salinity->hide();
    ui->salinity->setGeometry(ui->probeOptions->geometry());
    ui->pressureOptions->hide();
    this->reading = false;
    this->port = nullptr;
    bool connected = false;
    this->changeMode(SondeMode::NoMode);
    {
        SensorDetail ecDetail(QString("Electrical Conductivity"), QString("Blurb : Electrical Conductivity"));
        SensorDetail orpDetail(QString("Oxygen Reduction Potential") , QString("Blurb : Oxygen Reduction Potential"));
        SensorDetail phDetail(QString("pH"), QString("Blurb : pH"));
        SensorDetail doDetail(QString("Dissolved Oxygen") , QString("Blurb : Dissolved Oxygen"));
        SensorDetail rtdDetail(QString("Resistance Temperature Dectecor") , QString("Blurb : Resistance Temperature Dectecor"));

        this->sensorNames.append(ecDetail);
        this->sensorNames.append(orpDetail);
        this->sensorNames.append(phDetail);
        this->sensorNames.append(doDetail);
        this->sensorNames.append(rtdDetail);
    }
    {
        SensorSpecific probe(ui->probeOptions, SensorSpecific::EC);
        SensorSpecific temp(ui->tempOptions, (SensorSpecific::EC| SensorSpecific::pH | SensorSpecific::DO));
        SensorSpecific drycal(ui->calDry, SensorSpecific::EC | SensorSpecific::ORP);

        this->sensorSpecificWidgets.append( probe );
        this->sensorSpecificWidgets.append( temp );
        this->sensorSpecificWidgets.append( drycal );
    }

    //ui->sensorName->setText(sensorNames.at(0).sensorName);
    //ui->sensorName->setToolTip(sensorNames.at(0).sensorBlurb);

    on_currSensor_currentIndexChanged(ui->currSensor->currentIndex());
    on_currSensor_currentIndexChanged(ui->currSensor->currentText());
#ifdef TESTING_SERIALIO
    {

        QPointer<SerialIO> sio = new SerialIO();
        qDebug() << "Starting SerialIO";
        QObject::connect(this, &MainWindow::doneWithSerial, sio, &QObject::deleteLater);
        QObject::connect(this, SIGNAL(serialWrite(QString)), sio, SLOT(write(QString)));
        //This will call SIO's write when signaled to write
        QObject::connect(this, SIGNAL(serialRead(unsigned long)), sio, SLOT(read(unsigned long)));
        //Similarly, call SIO's read when signaled
        QObject::connect(sio, SIGNAL(doneWriting()), this, SLOT(fun1()));
        //This will call fun1 when SIO has signaled it's finished writing
        sio->start(QThread::HighPriority);
        {
            //emit SerialIO signals for the SerialIO to pick up
            emit serialWrite("This string should appear first");
            emit serialRead(1);
            emit serialWrite("Next, this string will follow the first");
            emit serialWrite("Following the second comes the third string");
            //Throw in a read to see what happens
            emit serialRead(3);
            emit serialWrite("Additionally this is the fourth string in the sequence");
            emit serialWrite("The chain is nearly complete with this, the fifth string");
            emit serialRead(5);
            emit serialWrite("This string is the final string!");
        }
        //emit doneWithSerial();
        sio->quit();
        while(sio->isRunning());
        delete sio;
    }
#endif
    foreach(const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        #ifdef CONNECT_TO_ARDUINO
        if(ARDUNIOID == info.vendorIdentifier())
        {
             setSerial(info);
             qDebug() << "Found Arduino at " << info.portName();
             connected = true;
        }
        #else
        if(SONDEID == info.vendorIdentifier())
        {
            setSerial(info);
            qDebug() << "Found Sonde at " << info.portName();
            connected = true;
        }
        #endif

    }

    if(connected)
    {
        port->worker = new QThread;
        port->moveToThread(port->worker);
        port->worker->start(QThread::HighPriority);

        if(port->open(QSerialPort::ReadWrite))
        {
            QObject::connect(port, SIGNAL(readyRead()), port, SLOT(serialRead()));
            QObject::connect(port, SIGNAL(readChannelFinished()), port, SLOT(sDoneReading()));
                        
            #ifdef CONNECT_TO_ARDUINO
            port->setBaudRate(ARDUINO_BAUD);
            #else
            port->setBaudRate(SONDE_BAUD);
            #endif
            port->setDataBits(QSerialPort::Data8);
            port->setParity(QSerialPort::NoParity);
            port->setStopBits(QSerialPort::OneStop);
            port->setFlowControl(QSerialPort::NoFlowControl);
            QObject::connect(this, SIGNAL(writeToSonde(QString)), port, SLOT(serialWrite(QString)));
            QObject::connect(port, SIGNAL(doneReading(QString)), this, SLOT(fun1(QString)));
        }

        else
        {
            QMessageBox error;
            error.setText("Found, but the port is busy");
            error.exec();
            //this->~MainWindow();

        }

    }
    else
    {
        QMessageBox error;
        error.setText("Couldn't connect to serialport");
        error.exec();
        //this->~MainWindow();
    }
}


void MainWindow::setLCD(QString type, QString val)
{
    if(0 == val.compare(" "))
    {
        val = "----";
    }
    if(0 == type.compare("EC", Qt::CaseInsensitive)){
        ui->ec_lcd->display(val);
    }
    else if(0 == type.compare("SAL", Qt::CaseInsensitive)){
        ui->sal_lcd->display(val);
    }
    else if(0 == type.compare("ORP", Qt::CaseInsensitive)){
        ui->orp_lcd->display(val);
    }
    else if(0 == type.compare("SG", Qt::CaseInsensitive)){
        ui->sg_lcd->display(val);
    }
    else if(0 == type.compare("pH", Qt::CaseInsensitive)){
        ui->ph_lcd->display(val);
    }
    else if(0 == type.compare("RTD", Qt::CaseInsensitive)){
        ui->rtd_lcd->display(val);
    }
    else if(0 == type.compare("DO", Qt::CaseInsensitive)){
        ui->do_lcd->display(val);
    }
    else if(0 == type.compare("SAT", Qt::CaseInsensitive)){
        ui->sat_lcd->display(val);
    }

}



void MainWindow::hideAllSensorSpecificWidgets()
{
    foreach(const SensorSpecific&

            ss, this->sensorSpecificWidgets)
    {
        ss.widget->hide();
    }
}

void MainWindow::toggleSensorSpecificWidgets(SensorSpecific::SensorType sensor)
{
    foreach(const SensorSpecific& ss, this->sensorSpecificWidgets)
    {
        if(0 != (sensor & ss.sensor))
        {
            ss.widget->show();
        }
    }
}


void MainWindow::setTab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}


MainWindow::~MainWindow()
{
    delete ui;
    if(nullptr != port)
    {
        delete port->worker;
        port->close();

    }

}

void MainWindow::ReadSerial()
{
    /*
     * test starting a reading thread
     * Basic threading seems to work ok
    */
#ifndef SYCHRNOUSREADING

#else

    buffer = QByteArray();

    this->reading = true;
    ui->readFrom->setEnabled(false);
    QByteArray readIn = port->readLine();//port->readAll();
    this->setWindowTitle("Reading in please wait..");

    //(buffer.split('\n').length()) || 1 >= buffer.split('\r').length()


    while(0 < readIn.length() || 1 >= buffer.split('\n').length())
    {

        port->waitForReadyRead(25);
        buffer.append(readIn);
        readIn = port->readAll();

    }


   QList<QByteArray> datalist = this->buffer.split('\n');
   foreach(QByteArray currbuff, datalist)
   {
       QString data = "_data:";

       if(currbuff.length() > currbuff.toStdString().find(data.toStdString().c_str()))
       {
           qDebug() << currbuff;
       }

       int start = currbuff.toStdString().find(data.toStdString().c_str()) + data.length();
       int len = currbuff.indexOf('\r', start)-start;
       QString dataT = QString::fromStdString(currbuff.mid(0, start - data.length()).toStdString());
       data = QString::fromStdString(currbuff.mid(start+1, len-1).toStdString());
       char num = data.toStdString().c_str()[0];
       if (isdigit(num) || '-' == num) {
           //qDebug() << dataT << ":" << data;
           QString readout = dataT + " : " + data;
           ui->contReadOut->append(readout);
           ui->dataReadOut->append(readout);
            setLCD(dataT,data);
           //Prints parsed data to the readout on the GUI

           //qDebug() << "data is =" << data << '\n';

   //        ui->textBrowser->append(data);
       }
   }

    ui->readFrom->setEnabled(true);

    this->setWindowTitle("Sonde Reader");

    //SondeData s(buffer.split('\t'));
//    if(10 == data.length())
//    {
//        QList<QLCDNumber*> LCDs;

//        LCDs.append(ui->lcdNumber);
//        LCDs.append(ui->lcdNumber_2);
//        LCDs.append(ui->lcdNumber_3);
//        LCDs.append(ui->lcdNumber_4);
//        LCDs.append(ui->lcdNumber_5);
//        LCDs.append(ui->lcdNumber_6);
//        LCDs.append(ui->lcdNumber_7);
//        LCDs.append(ui->lcdNumber_8);
//        LCDs.append(ui->lcdNumber_9);

//        bool okay;

//        for( int i = 0; i < 9; i++)
//        {

//            double val = data.at(i).toDouble(&okay);

//            if(okay)
//            {
//                LCDs.at(i)->display(val);
//            }
//            else
//            {
//                LCDs.at(i)->display("----");
//            }

//        }

//        ui->time->setTime( QTime::fromString(data.at(9)));

//        /*
//        ui->lcdNumber->display(s.getEC());
//        ui->lcdNumber_2->display(s.getTDS());
//        ui->lcdNumber_3->display(s.getSAL());
//        ui->lcdNumber_4->display(s.getSG());
//        ui->lcdNumber_5->display(s.getDO());
//        ui->lcdNumber_6->display(s.getSAT());
//        ui->lcdNumber_7->display(s.getPH());
//        ui->lcdNumber_8->display(s.getORP());
//        ui->lcdNumber_9->display(s.getRTD());
//

//    }

    this->reading = true;
    QByteArray readIn = port->readAll();

    this->buffer += readIn;
    if(buffer.contains('\n') || buffer.contains('\r'))
    {

        qDebug() << buffer.split('\t');
        buffer = QByteArray();
    }
    qDebug() << "Read " << ++readtimes << " times";

    this->reading = false;
#endif
}

void MainWindow::on_serialSend_clicked()
{
    const char* data = ui->serialData->text().toStdString().c_str();
    sendSerial(data);
}


bool MainWindow::sendSerial(QString data)
{
    if(nullptr != port)
    {
        //port->write(data);
        //qDebug() << data;
        emit writeToSonde(data);
    }
    return false;
}

void MainWindow::on_readFrom_clicked()
{
    if(nullptr != port)
    {
        this->sendSerial("r");
    }
}

void MainWindow::newSession()
{
    sendSerial("newsession");
}

void MainWindow::changeMode(SondeMode::ModeSelection mode)
{
    /*Perform checking before switching*/
    this->currentMode = mode;
}

void DONTCALLDEBUG(){

#ifdef NOTDEFINED
#endif
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(10 > index){
        this->newSession();
    }
    //port->write("newsession");
    //while(!port->waitForBytesWritten(-1));
    //while(!port->waitForReadyRead(-1));
    if (index == 0) index = 4;
//    if (index == 4) index = 5;
    qDebug() << "Mode = " << index;

//    this->newSession();
    this->sendSerial(QString::number(index));
    this->ReadSerial();
//    qDebug() << oss.str().c_str();
}

void MainWindow::CalCommand(QString sensor, QString command, QString args = "", bool expectResult = false)
{
    QMessageBox s;
    s.setWindowTitle("Outcome");
    QString cal_command = sensor + " " + command;
    if(!args.isEmpty())
    {
        cal_command += ","+args;
    }
    sendSerial(cal_command);

    QString data = "_data:";
    int start = buffer.toStdString().find(data.toStdString().c_str()) + data.length();
    int len = buffer.indexOf('\r', start)-start;

    if(0 < len)
    {
        data = QString::fromStdString(this->buffer.mid(start, len).toStdString());
    }
    else
    {
        data = "";
    }/*God help me*/

    //qDebug() << "Data : " << data;

    //qDebug() << this->buffer.toStdString().c_str();
    //qDebug() << this->buffer.length() << this->buffer.toStdString().find("Success");

    if((this->buffer.length() >= this->buffer.toStdString().find("Success")) && (this->buffer.length() < this->buffer.toStdString().find("Fail")))
    {
        QString prefix = (sensor + " " + command + args);
        if(data.isEmpty())
        {
            if(expectResult)
            {
                CalCommand(sensor, command, args, expectResult);
                return;
            }
            ui->readOut->append(prefix + " : " + "Success");

        }
        else
        {
            ui->readOut->append(prefix + " : " + data);
        }

    }
    else
    {
        s.setText("Something went wrong");
        s.exec();
    }


}

void MainWindow::on_currSensor_currentIndexChanged(const QString &arg1)
{
    ui->probeOptions->hide();
    ui->tempOptions->hide();
    ui->ecOptions->hide();
    ui->probeSlope->hide();
    ui->calDry->hide();
    ui->hiloCal->hide();
    ui->midCal->hide();
    ui->pressureOptions->hide();
    ui->rtdOptions->hide();
    //ui->rtdData->hide();
    ui->salinity->hide();
    ui->doOptions->hide();

    #ifdef NOTDEFINED
    #endif
    if(0 == arg1.compare("EC"))
    {
        ui->ecOptions->show();
        ui->probeOptions->show();
        ui->tempOptions->show();
        ui->calDry->show();
        ui->hiloCal->show();

    }
    else if(0 == arg1.compare("ORP"))
    {
        ui->calDry->show();
    }
    else if(0 == arg1.compare("pH", Qt::CaseInsensitive))
    {
        ui->probeSlope->show();
        ui->hiloCal->show();
        ui->midCal->show();
        ui->tempOptions->show();
    }
    else if(0 == arg1.compare("DO"))
    {
        ui->tempOptions->show();
        ui->pressureOptions->show();
        ui->salinity->show();
        ui->doOptions->show();
    }
    else if(0 == arg1.compare("RTD"))
    {

        if(ui->probeOptions->isHidden())
        {
            ui->rtdOptions->setGeometry(ui->probeOptions->geometry().x(), ui->probeOptions->geometry().y(), ui->rtdOptions->geometry().width(), ui->rtdOptions->geometry().height());
        }
        ui->rtdOptions->show();
    }


}



void MainWindow::on_sensorInfo_clicked()
{
    CalCommand(ui->currSensor->currentText(), "I");
}

void MainWindow::on_sensorRead_clicked()
{
    CalCommand(ui->currSensor->currentText(), "R", "", true);
}

void MainWindow::on_exportCal_clicked()
{
    CalCommand(ui->currSensor->currentText(), " Export", "?");
}

void MainWindow::on_calInfo_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Cal", "?");
}

void MainWindow::on_findSensor_clicked()
{
    CalCommand(ui->currSensor->currentText(), " Find");
}


void MainWindow::on_setProbe_clicked()
{
    QList<QString> vals= ui->probeVal->currentText().split(' ');
    CalCommand("EC", vals[0], vals[1]);
}

void MainWindow::on_currProbe_clicked()
{
    CalCommand("EC", "K", "?");
}

void MainWindow::on_sensorStatus_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Status");
}

void MainWindow::on_currSensor_currentIndexChanged(int index)
{
    ui->sensorName->setText(sensorNames.at(index).sensorName);
    ui->sensorName->setToolTip(sensorNames.at(index).sensorBlurb);

    ui->probeOptions->hide();
    ui->tempOptions->hide();
    ui->ecOptions->hide();
    ui->probeSlope->hide();

    ui->hiloCal->hide();
    ui->midCal->hide();

    ui->doOptions->hide();

    switch (index) {
    case 0:
        ui->ecOptions->show();
        ui->probeOptions->show();
        ui->tempOptions->show();
        ui->hiloCal->show();
        return;
    case 2:
        ui->probeSlope->show();
        ui->hiloCal->show();
        ui->midCal->show();
        ui->tempOptions->show();
    case 3:
        ui->doOptions->show();
    default:
        return;
    }
}

void MainWindow::on_sensorSleep_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Sleep");
}

void MainWindow::on_sensorFactory_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Factory");
}

void MainWindow::on_currTemp_clicked()
{
    CalCommand(ui->currSensor->currentText(), "T", "?");
}

void MainWindow::on_setTemp_clicked()
{
    CalCommand(ui->currSensor->currentText(), "T", QString::number(ui->temperatureVal->value()));
}

void MainWindow::on_calDry_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Cal", "dry");
}

void MainWindow::on_calClear_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Cal", "clear");
}

void MainWindow::on_calN_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Cal", QString::number(ui->calTo->value()));
}


void MainWindow::on_sensorRead_2_clicked()
{
    on_sensorRead_clicked();
}

void MainWindow::on_changeSettings_clicked()
{
    QString sensor = ui->currSensor->currentText();
    CalCommand(sensor, "L", (ui->led_on->isChecked()) ? "1" : "0");
}

void spare()
{

#ifdef NOTDEFINED

    if(0 == ui->currSensor->currentText().compare("EC"))
    {
        QString option = "O,";
        CalCommand(sensor, option+"EC", (ui->ecO->isChecked()) ? "1" : "0");
        CalCommand(sensor, option+"TDS", (ui->tdsO->isChecked()) ? "1" : "0");
        CalCommand(sensor, option+"S", (ui->sO->isChecked()) ? "1" : "0");
        CalCommand(sensor, option+"SG", (ui->sgO->isChecked()) ? "1" : "0");
    }
    if (ui->currSensor->currentText().compare("DO")) {
        QString option = "O,";
        CalCommand(sensor, option+"mg", (ui->doO->isChecked()) ? "1" : "0");
        CalCommand(sensor, option+"%", (ui->satO->isChecked()) ? "1" : "0");
    }
#endif
}

void MainWindow::on_rtdTemp_clicked()
{
    QString format;
    if(ui->tempF->isChecked())
    {
        format = "f";
    }
    else if(ui->tempK->isChecked())
    {
        format = "k";
    }
    else
    {
        format = "c";
    }
    CalCommand("rtd", "S", format);
}

void MainWindow::on_probeSlope_clicked()
{
    CalCommand(ui->currSensor->currentText(), "Slope", "");
}

void MainWindow::on_calOptions_currentChanged(int index)
{

}

void MainWindow::on_loCal_clicked()
{
    CalCommand(ui->currSensor->currentText(), "cal,low", QString::number(ui->loCalVal->value()));
}

void MainWindow::on_hiCal_clicked()
{
    CalCommand(ui->currSensor->currentText(), "cal,high", QString::number(ui->loCalVal->value()));
}

void MainWindow::on_midCalButton_clicked()
{
    CalCommand(ui->currSensor->currentText(), "cal,mid", QString::number(ui->loCalVal->value()));
}

void MainWindow::on_setPressure_clicked()
{
     CalCommand(ui->currSensor->currentText(), "P", QString::number(ui->pressureVal->value()));
}

void MainWindow::on_currPressure_clicked()
{
    CalCommand(ui->currSensor->currentText(), "P", "?");
}

void MainWindow::on_setDLI_clicked()
{
    CalCommand(ui->currSensor->currentText(), "D", QString::number(ui->dataLoggerInterval->value()));
}


void MainWindow::on_disableDL_clicked()
{
    CalCommand(ui->currSensor->currentText(), "D", "0");
}


void MainWindow::on_getDLI_clicked()
{
     CalCommand(ui->currSensor->currentText(), "D", "?");
}

//void MainWindow::on_var1_clicked()
//{
//    //ui->ta
//}

void MainWindow::on_memoryClear_clicked()
{
    CalCommand(ui->currSensor->currentText(), "M", "clear");
}

void MainWindow::on_memoryLocation_clicked()
{
    CalCommand(ui->currSensor->currentText(), "M", "?");
}

void MainWindow::on_memoryRecall_clicked()
{
    CalCommand(ui->currSensor->currentText(), "M");
}

void MainWindow::on_setProbe_2_clicked()
{
    QString sal = QString::number(ui->salVal->value());
    if(ui->pptSal->isChecked())
    {
        //CalCommand(ui->currSensor->currentText(), sal, "ppt");
        sal += ",ppt";
    }
    CalCommand(ui->currSensor->currentText(), "S", sal);
}

void MainWindow::on_currSal_clicked()
{
    CalCommand(ui->currSensor->currentText(),"S", "?", true);
}

void MainWindow::on_pushButton_clicked()
{
    QString time_interval;
    time_interval = ui->lineEdit->text();
    sendSerial(time_interval.toStdString().c_str());
}

void MainWindow::on_temp_submit_clicked()
{
    QString time_interval;
    time_interval = ui->temp_interval->text();
    sendSerial(time_interval.toStdString().c_str());
    qDebug() << time_interval;
}

void MainWindow::on_ph_submit_clicked()
{
    QString time_interval;
    time_interval = ui->ph_interval->text();
    sendSerial(time_interval.toStdString().c_str());
    qDebug() << time_interval;
}

void MainWindow::on_ec_submit_clicked()
{
    QString time_interval;
    time_interval = ui->ec_interval->text();
    sendSerial(time_interval.toStdString().c_str());
    qDebug() << time_interval;
}

void MainWindow::on_do_submit_clicked()
{
    QString time_interval;
    time_interval = ui->do_interval->text();
    sendSerial(time_interval.toStdString().c_str());
    qDebug() << time_interval;
}

void MainWindow::on_orp_submit_clicked()
{
    QString time_interval;
    time_interval = ui->orp_interval->text();
    sendSerial(time_interval.toStdString().c_str());
    qDebug() << time_interval;
}

void MainWindow::on_pressure_submit_clicked()
{
    QString time_interval;
    time_interval = ui->pressure_interval->text();
    sendSerial(time_interval.toStdString().c_str());
    qDebug() << time_interval;
}

void MainWindow::on_pushButton_2_clicked()
{
    this->ReadSerial();
}

void MainWindow::on_freq_submit_clicked()
{
    QString freq = ui->freq_val->text();
    qDebug() << freq;
}

void MainWindow::on_coding_rate_submit_clicked()
{
    QString cr = ui->bandwidth_val->currentText();
    qDebug() << cr;
}

void MainWindow::on_spreading_factor_submit_clicked()
{
    QString sf = ui->spread_factor_val->currentText();
    qDebug() << sf;
}

void MainWindow::on_power_submit_clicked()
{
    QString pwr = ui->power_val->currentText();
    qDebug() << pwr;
}

void MainWindow::on_bandwidth_submit_clicked()
{
    QString bw = ui->bandwidth_val->currentText();
    qDebug() << bw;
}
