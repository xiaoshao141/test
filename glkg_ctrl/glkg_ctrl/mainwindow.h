#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "switchangels.h"
#include "switchport.h"

#include <QMainWindow>
#include <QModbusRtuSerialMaster>
#include <QModbusClient>
#include <QModbusDevice>
#include <QModbusDataUnit>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QFile>
#include <QTime>

#include "modbus/modbus.h"
#include "modbus/modbus-private.h"
#include "modbus/modbus-rtu-private.h"
#include "modbus/modbus-rtu.h"
#include "modbus/modbus-version.h"

#define MODBUS_DEV1_ADDR        (0x01)
#define MODBUS_DEV2_ADDR        (0x02)

#define MODBUS_PORT_NAME        ("COM4")

#define RELAY_ON                (0x01)
#define RELAY_OFF               (0x00)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void handSwOpen();
    void handSwClose();
    void handSwStop();
    void handGswClose();
    void handGswStop();
    void handGswOpen();

    void handSwOpenClear();
    void handSwCloseClear();
    void handSwStopClear();
    void handGswCloseClear();
    void handGswStopClear();
    void handGswOpenClear();
private slots:
    void on_btswitchPort_clicked();

    void on_btswitchAngel_clicked();

    void on_m_timer_start();

    void dev1DataProc();

    void dev2DataProc();
private:
    void Modbus_RTU();
    //设置窗口的外观
    void setWindowStyle();
    //发送报文
    void sendModbus();
    //读取报文
    void readModbus();

private:
    Ui::MainWindow *ui;
    switchangels *angelwidget=new switchangels(this);
    switchPort  *portwidget=new switchPort(this);

    QModbusClient *modbusDevice;

    QTimer *m_timer = new QTimer(this);
};

extern bool g_wdAState;
extern bool g_wdBState;
extern bool g_wdCState;
extern bool g_wdPortAState;
extern bool g_wdPortBState;
extern bool g_wdPortCState;
extern bool g_wdHelpState;
extern bool g_wdTotalState;

extern bool g_ztAState;
extern bool g_ztBState;
extern bool g_ztCState;
extern bool g_ztPortAState;
extern bool g_ztPortBState;
extern bool g_ztPortCState;
extern bool g_ztHelpState;
extern bool g_ztTotalState;

extern MainWindow* g_pMainWindow;

#endif // MAINWINDOW_H
