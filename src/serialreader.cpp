#include "serialreader.h"

SerialReader::SerialReader(QSerialPort* in)
{
    this->input = in;
}

