#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QByteArray>
#include <vector>
#include <QKeyEvent>
#include <QTableWidget>
#include "addequipment.h"
#include "modifyaddress.h"
#include "setdialog.h"
#include "struct.h"
#include "ui_mainwindow.h"


#include <QEvent>
#include "serialport.h"
#include <QTime>
#include <QApplication>
#include <QFile>
#include"libmodbus/modbus.h"
//extern INDICATOR g_INDICATOR;
//extern NETWORKSTRUCT g_NETWORKSTRUCT;
//extern ADDRSTRUCT g_DEBUGADDR;
//extern ADDRSTRUCT g_LINKADDR;




extern MAP_REALSTATE_PARAM g_MAP_REALSTATE_PARAM;
extern MAP_DEBUGADDR_PARAM g_MAP_DEBUGADDR_PARAM;
extern MAP_BATTARY_PARAM g_MAP_BATTARY_PARAM;
extern MAP_METER_PARAM g_MAP_METER_PARAM;
extern MAP_ELEFIELD_PARAM g_MAP_ELEFIELD_PARAM;
extern MAP_GROUNDCUR_PARAM g_MAP_GROUNDCUR_PARAM;
extern MAP_SHORTCUR_PARAM g_MAP_SHORTCUR_PARAM;
extern MAP_NODEINFO_PARAM g_MAP_NODEINFO_PARAM;


#define BAOWENSHU     200
#define ADDR          01   //从机地址

using namespace std;
struct recive
{
    QString revbaowen;
    int revbw;
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
      void initVar();
      /*采集单元参数保存到结构体*/
      void saveRealStateParam();
      void saveDebugAddrParam();
      void saveBattaryParam();
      void saveMeterParam();
      void saveEleFieldParam();
      void saveGroundCurParam();
      void saveShortCurParam();
      void saveNodeInfoParam();

      void saveCaijiParam();
//      void windowstyel();

      void keyPressEvent(QKeyEvent *ev);

      void setData();
      void addserialname();


private slots:
    void on_system_triggered();
//    void showHuji();
//    void showCaiji();
//    void showConfig();

    void treeMenuFun(QTreeWidgetItem *,int);//定义树型窗体菜单
    void changeAddr();//修改地址

   //void showTreeMenu(QPoint pos);
    void showAddCaijiDevice();//添加采集设备
    void showRemoveDevice();
    /*采集单元*/
    void on_button_write();
    void on_button_read();
    void on_write_single();
    void on_read_single();
    //串口
    void openSerial();
    void setSerial();
    void readData();

    //8个函数只管读取
    void SScus();//实施状态
    void TScus();//调试地址
    void XBcus();//校表参数
    void D_chicus();//电池参数
    void D_chcus();//电场
    void JDcus(); //接地
    void DLcus(); //短路
    void JDiancus();//节点信息
//    void s_show();//显示
//   void send_bw();
    void timerOut();

    void JX_SS();
    void JX_jb();
    void JX_Dchi();
    void JX_Dchang();
    void JX_jd();
    void JX_jdi();
//    void JX_Dlu();
    void pl_read();//批量读取
    void awaken_aa();//唤醒帧aa

    void sleep(unsigned int msec);
    void plthr(QString devAddr);
//    void on_btfaultdetection_clicked();
//故障检测
    void f_openserial();
    void f_closeserial();
    void on_btfaultsimulation_clicked();
    void on_btpowerrecovery_clicked();
//监听

//    void stopthread();

private:
    Ui::MainWindow *ui;
    SetDialog *dialog;//添加私有成员，为一个SetDialog的指针
    QTableWidget *tableWidget;
    addequipment *addrDialog;
    ModifyAddress *modifyDialog;
    QMap<int, QTableWidget*> tables;//绑定界面
    QStringList headItems;
    vector<int> vecStd;
    QSerialPort *serial;//只要是定义了指针，必须开辟空间，否则中断报错
    serialport* m_serialport;
    QByteArray m_readBuf;//定义一个字节
     bool ok;

    recive revd[BAOWENSHU];

    int sum_1,sum_2,sum_3;
    QString s1,s2,s3,sum_3_new1;
    char buf[21];
    int ii;
    int i_dex;
    int addrsum;

    int afg;//作为批量下发中
    QString pl_addr;

    typedef void (MainWindow::*PFUNC)();//定义一个指向MainWidget类成员函数的指针声明
    QMap<int, PFUNC> caijiFuncPtrs;

    //故障检测

  //  f_simulation *threadA;
    QSerialPort *f_serial;
    modbus_t *m_pModbus;

//    //----------------------------
//    monitorThread *threadA;




};

#endif // MAINWINDOW_H
