#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include"serialport.h"
struct Settings {
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
};

namespace Ui {
class serialport;
}

class serialport : public QDialog
{
    Q_OBJECT

public:
    explicit serialport(QWidget *parent = 0);
    ~serialport();
public:
    void updateSettings();

    void accept();

public:
    Settings settings;
private:
    Ui::serialport *ui;
};

#endif // SERIALPORT_H
