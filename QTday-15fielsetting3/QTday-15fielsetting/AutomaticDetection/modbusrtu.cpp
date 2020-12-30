#include "modbusrtu.h"
#include "ui_modbusrtu.h"
#include<QDebug>

ModbusRTU::ModbusRTU(QWidget *parent) :
   QDialog(parent),
    ui(new Ui::ModbusRTU)
{
    ui->setupUi(this);
//    serial1=new QSerialPort(this);
//     ctx=new modbus_t(this);
    //定义一个端口
//       serial1.setPort(info);//将serial和info设置在一起

//       if(serial1.open(QIODevice::ReadWrite))//如果串口以读写方式成功打开
//       {
//          ui->comboBox_CK->addItem(serial1.portName());//向串口号comboBox组建添加端口名信息
//          serial1.close();//关闭设备，重置错误字符串
//       }

//       connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openSerial()));//打开或关闭串口
//        ui->comboBox_BTL->addItem("4800");//向波特率组件添加4800
//        ui->comboBox_BTL->addItem("9600");//向波特率组件添加9600
//        ui->comboBox_BTL->addItem("19200");//向波特率组件添加19200
//        ui->comboBox_BTL->addItem("115200");//向波特率组件添加115200
//        ui->comboBox_BTL->setCurrentText("9600");//波特率组件默认设置9600
//        ui->comboBox_SJW->addItem("6");//数据位组件添加6
//        ui->comboBox_SJW->addItem("7");//数据位组件添加7
//        ui->comboBox_SJW->addItem("8");//数据位组件添加8
//        ui->comboBox_SJW->setCurrentText("8");//数据位组件默认设置为8
//        ui->comboBox_JOJY->addItem("O");//校验位组件添加O
//        ui->comboBox_JOJY->addItem("E");//校验位组件添加E
//        ui->comboBox_JOJY->addItem("N");//校验位组件添加N
//        ui->comboBox_JOJY->setCurrentText("N");//校验位组件默认设置为N
//        ui->comboBox_TZW->addItem("0");//停止位组件添加0
//        ui->comboBox_TZW->addItem("1");//停止位组件添加1
//        ui->comboBox_TZW->setCurrentText("1");//停止位组件默认设置为1


}

ModbusRTU::~ModbusRTU()
{
    delete ui;
}

//void ModbusRTU::on_closeButton_clicked()
//{
//    this->close();
//}

//void ModbusRTU::on_sendButton_clicked()
//{

//}
//void ModbusRTU::openserial()
//{
//    QString Port = ui->comboBox_Port->currentText();//定义一个port字符串并赋值为comboBox_Port组件当前文本
//       int BoTeLv = ui->comboBox_BTL->currentText().toInt();//将波特率转化为的数字赋值给int
//       QString JiaoYanBit = ui->comboBox_JOJY->currentText();//定义一个JiaoYanBit字符串并赋值为comboBox_JiaoYanBit组件当前文本
//       int DataBit = ui->comboBox_SJW->currentText().toInt();//将数字位转化为的数字赋值给int
//       int StopBit = ui->comboBox_TZW->currentText().toInt();//将停止位转化为的数字赋值给int
//       int SheBeiDiZhi = ui->lineEdit->text().toInt();//将设备地址转化为数字赋值给int
//       const char *port = Port.toLatin1().constData();//将字节数组传递给char*以qbytearray形式返回字符串的拉丁文-1表示形式。
////       if(JiaoYanBit == "O")
////       {
////           ctx = modbus_new_rtu(port, BoTeLv, 'O', DataBit, StopBit);//生成一个modbus_t *结构体指针返回给PLC变量并执行相关RTU模型通信配置，采用奇数校验
////       }
////       else if(JiaoYanBit == "E")
////       {
////           ctx= modbus_new_rtu(port, BoTeLv, 'E', DataBit, StopBit);//同上，采用偶数校验
////       }
////       else if(JiaoYanBit == "N")
////       {
////          ctx = modbus_new_rtu(port, BoTeLv, 'N', DataBit, StopBit);//同上，无奇偶校验
////       }
//       ctx=modbus_new_rtu(ui->comboBox_CK, ui->comboBox_BTL, ui->comboBox_JOJY, ui->comboBox_SJW, ui->comboBox_TZW);//初始化RTU指针
//       qDebug() << Port;//输出端口号
//       qDebug() << QString::number(BoTeLv, 10);//将BoTeLv变量转化为10进制
//       qDebug() << JiaoYanBit;//输出校验位
//       qDebug() << QString::number(DataBit, 10);//将DataBit变量转化为10进制
//       qDebug() << QString::number(StopBit, 10);//将StopBit变量转化为10进制
//      ctx = modbus_new_rtu(port, BoTeLv, 'N', DataBit, StopBit);//生成一个modbus_t *结构体指针返回给PLC变量并执行相关RTU模型通信配置，采用无奇偶校验
//       modbus_set_slave(ctx, SheBeiDiZhi);//设置从机的设备地址
//       qDebug() << QString::number(SheBeiDiZhi, 10);//将SheBeiDiZhi变量转化为10进制
//       int plccount = modbus_connect(ctx);//与PLC变量建立连接
//       struct timeval time;//实例化一个timeval的结构体
//       time.tv_sec = 0;//0s
//       time.tv_usec = 1000000;//1000000us  1s
//       modbus_set_response_timeout(plc, (int)&time.tv_sec, (int)&time.tv_usec);//设置响应时间
//       if(plccount == 0)
//       {
//           qDebug() << "连接成功";//输出连接成功
//       }
//       else
//       {
//           qDebug() << "连接失败";//输出连接失败
//       }

//}
//void ModbusRTU::run()
//{


//    ctx=modbus_new_rtu("COM3", 115200, 'N', 8, 1);//初始化RTU指针
//    if (ctx == NULL) {
//        QString str="Unable to create the libmodbus context";
//        ui->textEdit->append(str);
//        return -1;
////        fprintf(stderr, "Unable to create the libmodbus context\n");
////        return -1;
//    }
//    modbus_set_slave(ctx,0x01);//设置modbus从机地址
//    modbus_connect(ctx);//建立连接
//    if(modbus_connect=-1)//表示打开串口
//    {
//      QString str="连接失败";
//      ui->textEdit->append(str);
//      modbus_strerror(errno);
//      modbus_free(ctx);
//      return -1;
//    }

//     nb=ADDRESS_END-ADDRESS_START;//计算寄存器的个数
//     //申请内存块用于保存发送和读取的数据

//     tab_rq_bits=(uint8_t *)malloc(nb *sizeof(uint8_t));
//     memest(tab_rq_bits,0,nb *sizeof(uint8_t));
//     tab_rp_bits=(uint8_t *)malloc(nb *sizeof(uint8_t));
//     memest(tab_rp_bits,0,nb *sizeof(uint8_t));
//     tab_rq_registers=(uint8_t *)malloc(nb *sizeof(uint8_t));
//     memest(tab_rq_registers,0,nb *sizeof(uint8_t));
//     tab_rp_registers=(uint8_t *)malloc(nb *sizeof(uint8_t));
//     memest(tab_rp_registers,0,nb *sizeof(uint8_t));
//     tab_rw_ra_registers=(uint8_t *)malloc(nb *sizeof(uint8_t));
//     memest(tab_rw_ra_registers,0,nb *sizeof(uint8_t));
//   nb_loop=nb_fail=0;
//   while(nb_loop++<LOOP)
//   {
////       //从起始地址开始
////       for (addr=0;addr<99;addr++)//为什么不能用宏定义
////       {
////           int i;
////           for(i=0;i<nb;i++)
////       }

//   //寄存器批量读写
//     rc=modbus_write_register(ctx,addr,tab_rp_registers[0]);
//     if(rc!=1)
//     {
//         qDebug()<<"写入数据失败";
//         nb_fail++;
//     }
//     else
//     {
//         //写入进行读取
//         rc=modbus_read_registers(ctx,addr,1,tab_rp_registers);
//         if(rc!=1)
//         {
//             qDebug()<<"读数据错误";
//             nb_fail++;

//         }
//         else
//         {
//             //读取后进行比较
//             if(tab_rq_registers[0]!=tab_rp_registers[0])
//             {
//                 nb_fail++;
//             }
//         }
//     }
//     free(tab_rq_registers);
//     free(tab_rp_registers);
//     free(tab_rw_ra_registers);

//     modbus_close(ctx);//关闭连接
//     modbus_free(ctx);//释放内容
     
    
//}

//void ModbusRTU::on_pushButton_clicked()
//{
//    modbus_close(ctx);//关闭跟ctx变量的连接
//        modbus_free(ctx);//释放modbus环境

//}

//void ModbusRTU::on_writeButton_clicked()
//{
//    int Adress = ui->lineEdit_JiCunQiDIZhiWrite->text().toInt();//将写寄存器地址赋值给adress
//        int ShuZhi = ui->lineEdit_JiCunQiShuZhi->text().toInt();//将写寄存器数值赋值给ShuZhi
//        if(ui->radioButton_XianQuan->isChecked() == true)//如果线圈的radiobutton被按下
//        {
//            qDebug() << "写入线圈";
//            modbus_write_bit(plc, Adress, ShuZhi);//强制单线圈
//        }
//        else//如果寄存器的radiobutton被按下
//        {
//           qDebug() << "写入寄存器";
//           modbus_write_register(plc, Adress, ShuZhi);//强制单寄存器
//        }

//}

//void ModbusRTU::on_connect_Button_clicked()
//{
//    run();
//}
