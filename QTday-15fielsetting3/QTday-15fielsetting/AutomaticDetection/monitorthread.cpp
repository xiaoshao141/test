#include "monitorthread.h"
#include <QDebug>
#include <QMessageBox>
monitorThread::monitorThread(QObject *parent) : QThread(parent)
{

}
 void monitorThread::run()
 {
     qDebug()<<"监听：888";
     while(1)
            {
                 QThread::sleep(1);
                 monitorport=new QSerialPort(this);
                 connect(monitorport, SIGNAL(readyRead()), this, SLOT(readAllData()));//串口收到数据，readyRead()就会触发一次
                 M_openserial();
            }

 }
void monitorThread::M_openserial()
{
    monitorport->setPortName("COM1");
    monitorport->setBaudRate(4800);//保存该方向的波特率
    monitorport->setDataBits(8);
    monitorport->setParity('E');
    monitorport->setStopBits(1);
     if ( monitorport->open(QIODevice::ReadWrite))
     {
         QDebug()<<"串口已打开";
     }
     else
     {
         QMessageBox::critical(this,QString::fromUtf8("错误") , QString::fromUtf8("串口打开失败，请检查串口设备是否工作正常"));//会弹出一个“错误”的消息框
         return;
     }
     m_readBuf.append( monitorport->readAll());//读取所以报文
     if (m_readBuf.at(0) > m_readBuf.size()) return;//接受大于实际大小
     QString rev(QString::fromUtf8("接收: "));
     for ( ii=0;ii < m_readBuf.size(); ii++)//int ii = 0;
     {
        revd[ii].revbw =m_readBuf[ii]&0xff, 2, 16, QChar('0');//?
         //int bw[40]=revd[ii].revbw;
         revd[ii].revbaowen = rev.append(QString(" %1").arg(m_readBuf[ii]&0xff, 2, 16, QChar('0')));
         //qDebug()<<ii;
     }
     qDebug()<<"bw:";
     qDebug()<<revd[ii-1].revbaowen;




}
