#include "serialCommunicator2.h"
////////////Functions:////////////////////////////////

SerialCommunicator2::SerialCommunicator2(void){

};

SerialCommunicator2::~SerialCommunicator2() = default;

void SerialCommunicator2::port_select(QSerialPortInfo portInfo)
{
    qDebug() << "port_select called!";
    port.setPort(portInfo);
}

void SerialCommunicator2::open_port(int bauds, int portNumber)
{
    qDebug() << "opening with " << bauds << " bauds and port number " << portNumber << "!";
    bauds_ = bauds;
    port_select(getSerialPorts()[portNumber]);
    port.setBaudRate(bauds);
    port.open(QIODevice::ReadWrite);
    portOpened = true;
}

QList<QSerialPortInfo> SerialCommunicator2::getSerialPorts(void)
{
    return QSerialPortInfo::availablePorts();
}

void SerialCommunicator2::close_port(void)
{
    if (portOpened)
    {
        port.close();
        portOpened = false;
    }
}

bool SerialCommunicator2::portSuccessfullyOpened(void) { return portOpened; }

char *SerialCommunicator2::getMessage(void)
{
    qDebug() << "getMessage called!";
    if (portSuccessfullyOpened())
    {
        while (port.bytesAvailable() > 0)
        {
            qDebug() << "bytes in queue: " << port.bytesAvailable();
            auto bytes = port.readLine(1000);

            qDebug() << "bytes: " << bytes;
            message_ = bytes.data();
            if (bytes.size() > 0)
            {
                if (QString(bytes.sliced(0, 2)) == QString("QT"))
                {
                    lastAngle = QString(bytes.sliced(2)).toDouble();
                    if (lastAngle == 0.0)
                    {
                        lastAngle = 37;
                        qDebug() << "incorrect readings, setting to 37";
                    }
                    qDebug() << "lastAngle: " << lastAngle;
                }
                else
                {
                    qDebug() << "message: " << bytes.toStdString();
                }
            }
        }
        qDebug()<< "message: " << message_;
        return message_;
    }
    else
    {
        qDebug() << "Port not opened";
        lastAngle = -37;
        message_ = (char*)"Port not opened";
        return message_;
    }
}

char *SerialCommunicator2::getLastMessage(void) { return message_; }


double SerialCommunicator2::readLastAngle(void)
{
    getMessage();
    return lastAngle;
}

void SerialCommunicator2::sendStopRequestNow() { qDebug() << "Not implemented!!!"; }

void SerialCommunicator2::setHILSSensorData(int a, int b, int c) { qDebug() << "Not implemented!!!!dgfh!"; }
int SerialCommunicator2::getLastDebugDataAnalog(int a)
{
    // qDebug() << "Not implemented!!!!!!";
    return 1;
}
