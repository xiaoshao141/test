#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QDialog>
#include<QThread>
#include"libmodbus/modbus-rtu.h"
#include"libmodbus/modbus.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#define LOOP            1  //循环次数
#define SERVER_ID       01 //从设备地址
#define ADDRESS_START   0;//测试寄存器起始地址
#define ADDRESS_END     99;//测试寄存器终止地址
namespace Ui {
class ModbusRTU;
}

class ModbusRTU : public  QDialog
{
    Q_OBJECT

public:
    explicit ModbusRTU(QWidget *parent = 0);
    ~ModbusRTU();
//    void stopThread();//结束线程

private slots:
//    void openserial();
////    void on_closeButton_clicked();

////    void on_sendButton_clicked();
//    void on_pushButton_clicked();

//    void on_writeButton_clicked();

//    void on_connect_Button_clicked();

protected:
     void run();
private:
    Ui::ModbusRTU *ui;
    modbus_t *ctx;
    QSerialPort serial1;
    int rc;
    int nb_fail;
    int nb_loop;
    int addr;
    int nb;
    uint8_t *tab_rq_bits;
    uint8_t *tab_rp_bits;
    uint16_t *tab_rq_registers;
    uint16_t *tab_rw_ra_registers;
    uint16_t *tab_rp_registers;
    
};

#endif // MODBUSRTU_H
