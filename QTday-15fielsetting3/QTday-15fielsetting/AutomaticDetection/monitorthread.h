#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QThread>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
class monitorThread : public QThread
{
    Q_OBJECT
public:
    explicit monitorThread(QObject *parent = nullptr);

signals:

public slots:
private slots:
    void readAllData();
    void M_openserial();
protected:
    void run();

protected:
    QSerialPort *monitorport;
};

#endif // MONITORTHREAD_H
