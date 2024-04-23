#ifndef SERIALCOMMUNICATOR2_H
#define SERIALCOMMUNICATOR2_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMainWindow>


class SerialCommunicator2 {
private:
  QSerialPort port = QSerialPort();
  bool portOpened = false;
  char* message_=(char*)"";
  double lastAngle = 0;
  int bauds_ = 115200;

public:
    SerialCommunicator2();
    ~SerialCommunicator2();
    QList<QSerialPortInfo> getSerialPorts();

    void port_select(QSerialPortInfo);
    void open_port(int, int);
    void close_port();
    bool portSuccessfullyOpened();//{return port!=0;};
    char* getMessage();
    double readLastAngle();
    char* getLastMessage();

    //switch the system into HILS mode

    //Comm's sending: (a function that skips the queue, sends a command directly)
    // You probably shouldn't use these yourself, but let the threads handle them
    void sendStopRequestNow();


    void setHILSSensorData(int a, int b, int c);
    int getLastDebugDataAnalog(int a);
};

#endif
