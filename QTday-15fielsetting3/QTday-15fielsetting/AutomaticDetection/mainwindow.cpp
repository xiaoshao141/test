#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"setdialog.h"
#include "addequipment.h"
#include"modifyaddress.h"
#include<QDebug>
#include<QTime>
#include<QApplication>


//INDICATOR g_INDICATOR;
//NETWORKSTRUCT g_NETWORKSTRUCT;
//ADDRSTRUCT g_DEBUGADDR;
//ADDRSTRUCT g_LINKADDR;

MAP_REALSTATE_PARAM g_MAP_REALSTATE_PARAM;//定义结构体变量
MAP_DEBUGADDR_PARAM g_MAP_DEBUGADDR_PARAM;
MAP_BATTARY_PARAM   g_MAP_BATTARY_PARAM;
MAP_METER_PARAM     g_MAP_METER_PARAM;
MAP_ELEFIELD_PARAM  g_MAP_ELEFIELD_PARAM;
MAP_GROUNDCUR_PARAM g_MAP_GROUNDCUR_PARAM;
MAP_SHORTCUR_PARAM  g_MAP_SHORTCUR_PARAM;
MAP_NODEINFO_PARAM  g_MAP_NODEINFO_PARAM;
//MAP_BWNO_PARAM g_MAP_BWNO_PARAM;
//QMap<int, PFUNC> caijiFuncPtrs;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    m_serialport=new serialport(this);
    serial=new QSerialPort(this);
//    threadA=new monitorThread(this);


//样式 添加qss文件

   QFile file("://qss(1)/no.qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = tr(file.readAll());
   this->setStyleSheet(styleSheet);
   file.close();

    initVar();

//    setStyleSheet(" QMainWindow { background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4792D7, stop: 1 #70CED7);}" );

//串口
    connect(ui->openCom, SIGNAL(clicked()),   this, SLOT(openSerial()));//打开或关闭串口
    connect(ui->setCom,  SIGNAL(clicked()),   this, SLOT(setSerial()));
    connect(serial,      SIGNAL(readyRead()), this, SLOT(readData()));//串口收到数据，readyRead()就会触发一次
    tableWidget = new QTableWidget(3,3);
        //send_bw();

//故障检测
    setData();
    connect(ui->btOpen, SIGNAL(clicked()),this,SLOT(f_openserial()));
    connect(ui->btclose,SIGNAL(clicked()),this,SLOT(f_closeserial()));
    //标题样式
    ui->label->setStyleSheet("QLabel{color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #1F36D7, stop: 1.0 #77ACD7);"
                             "font-size:75px;"
                             "font-family:KaiTi;"
                             "background:transparent;}");
    ui->widget_5->setStyleSheet("QWidget{background:transparent}");

    addserialname();

//监听
//   threadA->start();//线程开始

//   connect(this,&MainWindow::destroyed,this,&MainWindow::stopthread);//当按下右上角的关闭，线程结束


}
MainWindow::~MainWindow()
{ 
    delete ui;  
}
void MainWindow::initVar()
{
  addrDialog = new addequipment(this);//添加地址对话框
  modifyDialog = new ModifyAddress (this);//修改地址对话框

  QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_3,QStringList(QString::fromUtf8("采集设备")),0);//0表示的是目录树第一列
   //在treeWidget_3中添加一个项目

  connect(ui->treeWidget_3,SIGNAL(itemPressed (QTreeWidgetItem *,int)),this,SLOT(treeMenuFun(QTreeWidgetItem *,int)));
  tables.insert(0, ui->tableWidget_realState);//分别关联八个窗口
  tables.insert(1, ui->tableWidget_debugAddr);
  tables.insert(2, ui->tableWidget_xiaobiao);
  tables.insert(3, ui->tableWidget_dianchi);
  tables.insert(4, ui->tableWidget_dianchang);
  tables.insert(5, ui->tableWidget_jiedi);
  tables.insert(6, ui->tableWidget_duanlu);
  tables.insert(7, ui->tableWidget_jiedian);
  tables.insert(8,ui->tab_FaultDisplay);
  //绑定界面

  QMap<int, QTableWidget*>::iterator t_iter;//
//  QString horiStyle = "QHeaderView::section \
//                      {background-color:lightblue;color: black;padding-left: 4px;border: 1px solid #6c6c6c}"; //设置表头字体，颜色，模式
//  QString vertStyle = "QHeaderView::section \
//                      {background-color:skyblue;color: black;padding-left: 4px;border: 1px solid #6c6c6c}";//设置列前的颜色
//  QString tabBarStyle = "QTabBar::tab {min-width:100px;color:black;border: 1px solid;\
//                        border-top-left-radius: 5px;border-top-right-radius: 5px;padding:5px;}\
//                        QTabBar::tab:selected {background:#5FE4F7;color:black;}";//TabWidget标题的字体颜色

  for (t_iter = tables.begin(); t_iter != tables.end(); t_iter++)
  {
      t_iter.value()->setColumnHidden(0, true);//隐藏指定的列
      t_iter.value()->setAlternatingRowColors(true);
//    t_iter.value()->horizontalHeader()->setStyleSheet(horiStyle);
//    t_iter.value()->verticalHeader()->setStyleSheet(vertStyle);
  }


 /* ui->tableWidget_config->horizontalHeader()->setStyleSheet(horiStyle);
  ui.tableWidget_config->verticalHeader()->setStyleSheet(vertStyle)*/

//  ui->tabWidget_caiji->setStyleSheet(tabBarStyle);
//  ui->tableWidget_xiaobiao->horizontalHeader()->setDefaultSectionSize(75);

  caijiFuncPtrs.clear();
  caijiFuncPtrs.insert(0, &MainWindow::saveRealStateParam);//对应的指针函数保存 实时
  caijiFuncPtrs.insert(1, &MainWindow::saveDebugAddrParam);//调试地址
  caijiFuncPtrs.insert(2, &MainWindow::saveMeterParam);//校表
  caijiFuncPtrs.insert(3, &MainWindow::saveBattaryParam);//电池

  caijiFuncPtrs.insert(4, &MainWindow::saveEleFieldParam);//电场
  caijiFuncPtrs.insert(5, &MainWindow::saveGroundCurParam);//接地
  caijiFuncPtrs.insert(6, &MainWindow::saveShortCurParam);//短路
  caijiFuncPtrs.insert(7, &MainWindow::saveNodeInfoParam);//节点
  connect(ui->treeWidget_3,    SIGNAL(itemPressed (QTreeWidgetItem *,int)),this,SLOT(treeMenuFun(QTreeWidgetItem *,int)));
  connect(ui->pushButton_write,SIGNAL(clicked()),this,SLOT(on_button_write()));//采集单元总写参数
  connect(ui->pushButton_read, SIGNAL(clicked()),this,SLOT(on_button_read()));//采集单元总读参数



}
void MainWindow::addserialname()
{
    ui->comboBox_9->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //搜索串口并添加到选项上
    {
        QStringList list;
        ui->comboBox_9->addItem(info.portName());//获取串口名称
    }
}
void MainWindow::setData()
{
    ui->comboBox_btl2->addItem(("4800"),   QSerialPort::Baud4800);
    ui->comboBox_btl2->addItem(("9600"),   QSerialPort::Baud9600);
    ui->comboBox_btl2->addItem(("19200"),  QSerialPort::Baud19200);
    ui->comboBox_btl2->addItem(("38400"),  QSerialPort::Baud38400);
    ui->comboBox_btl2->addItem(("115200"), QSerialPort::Baud115200);
    ui->comboBox_btl2->setCurrentIndex(0);
//设置数据位数
    ui->comboBox_data2->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_data2->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_data2->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_data2->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_data2->setCurrentIndex(3);
//设置奇偶校验
    ui->comboBox_xiaoy2->addItem(QString::fromUtf8("O"), QSerialPort::NoParity);
    ui->comboBox_xiaoy2->addItem(QString::fromUtf8("N"), QSerialPort::EvenParity);
    ui->comboBox_xiaoy2->addItem(QString::fromUtf8("E"), QSerialPort::OddParity);
    ui->comboBox_xiaoy2->setCurrentIndex(2);
//设置停止位
    ui->comboBox_stop2->addItem(("1"),   QSerialPort::OneStop);
    ui->comboBox_stop2->addItem(("1.5"), QSerialPort::OneAndHalfStop);
    ui->comboBox_stop2->addItem(("2"),   QSerialPort::TwoStop);
    ui->comportname->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //搜索串口并添加到选项上
    {
//        QStringList list;
        ui->comportname->addItem(info.portName());//获取串口名称
    }
//    f_updateSettings();
}
  //按右键添加地址
void MainWindow::treeMenuFun(QTreeWidgetItem *item,int column)
{
    if (qApp->mouseButtons() == Qt::RightButton)
    {
        if (item->type() == 0)//点击第一列出现的菜单
        {
            QAction addPart(QString::fromUtf8("增加采集设备"),this);//增加采集设备
            QObject::connect(&addPart,SIGNAL(triggered()),this,SLOT(showAddCaijiDevice()));
            QMenu menu(ui->treeWidget_3);
            menu.addAction(&addPart);
            menu.exec(QCursor::pos());//显示菜单
        }
        else if(item->type() == 1)//点击第二列后右键出现的菜单
        {
            QAction readPart(QString::fromUtf8("读参数"), this);
            QAction writePart(QString::fromUtf8("写参数"), this);
            QAction removePart(QString::fromUtf8("删除采集设备"), this);//删除添加的某一项
            QAction modifyAddr(QString::fromUtf8("修改设备地址"),this);
            QAction addnodeInfo(QString::fromUtf8("添加节点信息"),this);
            QAction caijireturn(QString::fromUtf8("采集单元复归"),this);

            connect(&writePart,SIGNAL(triggered()),this,SLOT(on_write_single()));
            connect(&readPart,SIGNAL(triggered()),this,SLOT(on_read_single()));
            connect(&removePart, SIGNAL(triggered()), this, SLOT(showRemoveDevice()));
            connect(&modifyAddr,SIGNAL(triggered()),this,SLOT(changeAddr()));
//          connect(&addnodeInfo,SIGNAL(triggered()),this,SLOT(on_addr()));
//          connect(&caijireturn,SIGNAL(triggered()),this,SLOT(on_cancel()));


            QMenu menu(ui->treeWidget_3);//在treeWidget_3建立菜单
            menu.addAction(&readPart);
            menu.addAction(&writePart);
            menu.addAction(&modifyAddr);
            menu.addAction(&removePart);
            menu.addAction(&addnodeInfo);
            menu.addAction(&caijireturn);



            menu.exec(QCursor::pos());
        }
    }
}
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    int type= ui->treeWidget_3->currentItem()->type();
    if (ui->treeWidget_3->currentItem()->type()==0)
    {
        return;
    }

    if(ev->key() == Qt::Key_Delete)
    {
        showRemoveDevice();
    }
}

void MainWindow::showRemoveDevice()
{
    QTreeWidgetItem *item = ui->treeWidget_3->currentItem();

    int rowNo = item->parent()->indexOfChild(item);

    if(item->parent() != NULL)
    item->parent()->removeChild(item);

    QMap<int, QTableWidget*>::iterator t_iter;

    for (t_iter = tables.begin(); t_iter != tables.end(); t_iter++)
    {
        t_iter.value()->removeRow(rowNo);
    }
}

//修改设备地址
void MainWindow::changeAddr()
{
    QTreeWidgetItem *item = ui->treeWidget_3->currentItem();

    qDebug()<<item->type();
    if (item->type() != 1)
    {
        return;
    }
    int rowNo = item->parent()->indexOfChild(item);

    modifyDialog->isOk = false;
    modifyDialog->exec();

    if (modifyDialog->isOk == false)
    {
        return;
    }

    QString devAddr = QString::number(modifyDialog->addr, 16);
    while (devAddr.size() < 6)
    {
        devAddr.prepend("0");//在前面添加0
    }
    QString devName = QString::fromLocal8Bit("采集设备_0x") + devAddr;
    item->setText(0, devAddr);

    QMap<int, QTableWidget*>::iterator t_iter;

    for (t_iter = tables.begin(); t_iter != tables.end(); t_iter++)
    {
        t_iter.value()->item(rowNo, 1)->setText(devAddr);//在表格中显示修改地址后的地址
    }
}
//处理增加采集设备槽函数。
void MainWindow::showAddCaijiDevice()
{
    addrDialog->setFocus();
    addrDialog->exec();

   QString devAddr = QString::number(addrDialog->addr, 16);//
   if(addrDialog->plisOk==true)
   {
       while (devAddr.size()<5)
       {
           devAddr.prepend("0");
       }
       for(int i=0;i<3;i++)
       {
           devAddr.replace(5,1,'a'+i);
           plthr(devAddr);

       }
     }

   if (addrDialog->isOk == false)
   {
       return;
   }
   else
       if(addrDialog->isOk==true)

   {
       while (devAddr.size()<6)
    {
        devAddr.prepend("0");
    }

}
    //遍历treeWidget
    QTreeWidgetItemIterator it(ui->treeWidget_3);
    while (*it)
    {
        if ((*it)->text(0) == devAddr)
        {
            (*it)->setSelected(true);
            return;
        }
        ++it;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_3->currentItem(),QStringList(devAddr),1);

    ui->treeWidget_3->sortItems(0, Qt::AscendingOrder);


    int rowNo = ui->treeWidget_3->currentItem()->indexOfChild(item);

    QMap<int, QTableWidget*>::iterator t_iter;

    for (t_iter = tables.begin(); t_iter != tables.end(); t_iter++)
    {
        t_iter.value()->insertRow(rowNo);//添加行

        for (int i = 0; i < t_iter.value()->columnCount(); i++)//用for循环进行遍历
        {
            t_iter.value()->setItem(rowNo, i, new QTableWidgetItem);
        }

        t_iter.value()->item(rowNo, 0)->setText(QString::number(rowNo));//显示序号
        t_iter.value()->item(rowNo, 1)->setText(devAddr);//显示地址
        t_iter.value()->item(rowNo, 1)->setFlags(Qt::NoItemFlags);//对地址进行属性设置，此处没有设置属性
}
//        QTableWidgetItem *item = t_iter.value()->item(rowNo, 1);
//        item->setFlags(item->flags() & (~Qt::ItemIsEditable));


//    int index = 0;
//    for (int i = 0; i < headItems.count(); i++)
//    {
//        if (headItems.at(i).contains(QString::fromLocal8Bit("标准值")))
//        {
//            QString stdVal = QString::number(vecStd.at(index));
//            ui->tableWidget_xiaobiao->item(rowNo, i)->setText(stdVal);
//            ui->tableWidget_xiaobiao->item(rowNo, i)->setBackgroundColor(QColor(240,130,120));

//            index++;
//        }
//        else if (headItems.at(i).contains(QString::fromLocal8Bit("采样值")))
//        {
//            ui->tableWidget_xiaobiao->item(rowNo, i)->setBackgroundColor(QColor(150,250,120));
//        }
//        else if (headItems.at(i).contains(QString::fromLocal8Bit("实际值")))
//        {
//            ui->tableWidget_xiaobiao->item(rowNo, i)->setBackgroundColor(QColor(160,230,250));
//        }
//    }

   ui->treeWidget_3->setItemsExpandable(false);
    ui->treeWidget_3->expandAll();


}
void MainWindow::plthr(QString devAddr)
{

    //遍历treeWidget
    QTreeWidgetItemIterator it(ui->treeWidget_3);
    while (*it)
    {
        if ((*it)->text(0) == devAddr)
        {
            (*it)->setSelected(true);
            return;
        }
        ++it;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_3->currentItem(),QStringList(devAddr),1);

    ui->treeWidget_3->sortItems(0, Qt::AscendingOrder);


    int rowNo = ui->treeWidget_3->currentItem()->indexOfChild(item);

    QMap<int, QTableWidget*>::iterator t_iter;

    for (t_iter = tables.begin(); t_iter != tables.end(); t_iter++)
    {
        t_iter.value()->insertRow(rowNo);//添加行

        for (int i = 0; i < t_iter.value()->columnCount(); i++)//用for循环进行遍历
        {
            t_iter.value()->setItem(rowNo, i, new QTableWidgetItem);
        }

        t_iter.value()->item(rowNo, 0)->setText(QString::number(rowNo));//显示序号
        t_iter.value()->item(rowNo, 1)->setText(devAddr);//显示地址
        t_iter.value()->item(rowNo, 1)->setFlags(Qt::NoItemFlags);//对地址进行属性设置，此处没有设置属性

//        QTableWidgetItem *item = t_iter.value()->item(rowNo, 1);
//        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    }
}
//打开串口
void MainWindow::openSerial()
{


    if (serial->isOpen())
    {
        serial->close();
        ui->openCom->setText(QString::fromUtf8("打开"));
        ui->openCom->setStyleSheet("background-color: #E88894;");//按钮的背景是红色
        return;
    }

    Settings pSerCombox =m_serialport->settings;
    serial->setPortName(ui->comboBox_9->currentText());
    serial->setBaudRate(pSerCombox.baudRate);//保存该方向的波特率
    serial->setDataBits(pSerCombox.dataBits);
    serial->setParity(pSerCombox.parity);
    serial->setStopBits(pSerCombox.stopBits);
    serial->setFlowControl(pSerCombox.flowControl);
    if (serial->open(QIODevice::ReadWrite))//使用OpenMode模式打开串行端口，QIODevice::ReadWrite 该设备可以读写
    {
        ui->openCom->setText(QString::fromUtf8("关闭"));
        ui->openCom->setStyleSheet("background-color: #8EE8A0;");
    } else {

        QMessageBox::critical(this,QString::fromUtf8("错误") , QString::fromUtf8("串口打开失败，请检查串口设备是否工作正常"));//会弹出一个“错误”的消息框
        return;
    }
}
void MainWindow::setSerial()
{
    m_serialport->exec();
}
void MainWindow::readData()//接收总的报文函数   on_read_single()1176
{
    int index = ui->tabWidget_caiji->currentIndex();//表示所在tabwidget的子窗口
    //bool ok;
    m_readBuf.append(serial->readAll());//读取所以报文
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
    //int num=m_readBuf.size();
    //ui->progressBar->setRange(0,num);
    // 	for(int i=1;i<=num;i++)
    // 	{
    // 		ui->progressBar->setValue(i);
    // 	}
    /*connect(ui->pButtonCancel,SIGNAL(clicked()),this,SLOT(cancelSlot()));//进度条*/

    //ui.plainTextEdit->setPlainText(rev);


//    int jx_hxz=QString::number(revd[ii].revbw,10).toInt();//唤醒帧
//    int jx_hxzaa=170;//aa的十进制
//    if(jx_hxz==jx_hxzaa)//判断最后一位是否是aa
//    {

//      qDebug()<<"隐藏";
//        ui->centralWidget->setEnabled(true);
//    }
//    else
//    {
    ui->textEdit->append(rev);
    ui->centralWidget->setEnabled(true);

    if (index==0&&i_dex==0)//index 的值是指的tabwidget的页数,前面右相应的定义
    {
        JX_SS();//实时解析
    }
    else if (index==2&&i_dex==2)
    {
        JX_jb();//校表解析
    }
    else if (index==3&&i_dex==3)
    {
        JX_Dchi();//电池解析
    }
    else if (index==4&&i_dex==4)
    {
        JX_Dchang();//电场解析
    }
    else if (index==7&&i_dex==7)
    {
        JX_jd();//节点解析
    }
    else if (index==5&&i_dex==5)
    {
        JX_jdi();//接地解析
    }
}
//    else if (index==6&&i_dex==6)
//    {
//        JX_Dlu();//短路解析
//    }

//}
void MainWindow::awaken_aa()
{
    char buf[9]={0x09,0x01,0x01,0x01,0x02,0x02,0x02,0x00,0x00};//校验和计算按照index的不同，功能码不同 在从新计算

    buf[8]=0xaa;
    buf[1]=s1.toInt(&ok,16);//十六进制 采集单元地址
    buf[2]=s2.toInt(&ok,16);

    buf[3]=s3.toInt(&ok,16);
    QString snd(QString::fromUtf8("发送: "));//设定一个字符串
    serial->write(buf, 9);
    for (int i = 0;i < buf[0]; i++)//i小于总长度
    {
           QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        //QString QString:：arg（const QString&a，int fieldWidth，QChar fillChar=qlatinchar（''）const
        //a 代表的是返回的字符串 ， fieldWidth是字的位数，输出的是16进制，一般写QcharQChar('0')为自动补全0
        snd.append(stemp);

    }
    ui->textEdit->append(snd);//报文中显示校验的内容
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(10000, this, SLOT(timerOut()));//10秒弹出超时对话框


}
//void MainWindow::send_bw()
//{   int in_index = 0;
//    char sendMessage[50];
//    int sun_1=0;


//    sendMessage[in_index++] =0x00;
//    sendMessage[in_index++] =0x00;
//    //qDebug()<<sendMessage[1];
//    sendMessage[in_index++]= 0x00;
//    sendMessage[in_index++]=0xc6;
//    //qDebug()<<sendMessage[4];
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x02;
//    //qDebug()<<sendMessage[7];
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x7e;
//    sendMessage[in_index++]=0x09;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x36;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0xc6;
//    sendMessage[in_index++]=0x89;
//    sendMessage[in_index++]=0x16;
//    sendMessage[in_index++]=0x18;

//    sendMessage[in_index++]=0x09;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x02;
//    sendMessage[in_index++]=0x36;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0x00;
//    sendMessage[in_index++]=0xc6;
//    sendMessage[in_index++]=0x89;
//    sendMessage[in_index++]=0x16;
//    sendMessage[in_index++]=0x18;

//    sendMessage[0]=in_index+1;//求出总长度
//    //sendMessage[0] =QString("c6") ;
//    //sendMessage[in_index];
//    serial->write(sendMessage,in_index);
//    QString snd(QString::fromUtf8("发送: "));
//    for (int i = 0;i < sendMessage[0]; i++)
//    {
//        QString stemp = QString(" %1").arg(sendMessage[i]&0xff, 2, 16, QChar('0'));
//        snd.append(stemp);

//    }
//    ui->textEdit->append(snd);

//}
//void MainWindow::s_show()
//{
//    QString snd(QString::fromUtf8("发送: "));
//    for (int i = 0;i < buf[0]; i++)
//    {
//        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
//        snd.append(stemp);

//    }
//    ui->textEdit->append(snd);
//    m_readBuf.clear();
//    ui->centralWidget->setEnabled(false);//centralWidget表示的是总串口

//    QTimer::singleShot(10000, this, SLOT(timerOut()));//定时器
//}

void MainWindow::SScus()//实时状态
{

        char buf[21]={0x15,0x01,0x01,0x01,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x36,0x00,0x00,0x01,0x01,0x01,0x51,0x16};//校验和计算按照index的不同，功能码不同 在从新计算


        buf[1]=s1.toInt(&ok,16);//十六进制 采集单元地址
        buf[2]=s2.toInt(&ok,16);

        buf[3]=s3.toInt(&ok,16);

        buf[16]=s1.toInt(&ok,16);
        buf[17]=s2.toInt(&ok,16);
        buf[18]=s3.toInt(&ok,16);


        addrsum=0;//求校验和
        for (int i=0;i<buf[9]+2;i++)//校表有效长度+2
        {
            addrsum+=(uchar)buf[i+8];//不加uchar会有负数
//            qDebug()<<addrsum;
        }

         buf[19]=addrsum&0xff;//校验和
        serial->write(buf, 21);
        QString snd(QString::fromUtf8("发送: "));//设定一个字符串
        for (int i = 0;i < buf[0]; i++)//i小于总长度
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            //QString QString:：arg（const QString&a，int fieldWidth，QChar fillChar=qlatinchar（''）const
            //a 代表的是返回的字符串 ， fieldWidth是字的位数，输出的是16进制，一般写QcharQChar('0')为自动补全0
            snd.append(stemp);

        }
        ui->textEdit->append(snd);//报文中显示校验的内容
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(20000, this, SLOT(timerOut()));//20秒弹出超时对话框

}
void MainWindow::JX_SS()//实时解析
{
    int jx_xyh;
    QString jx_ss=revd[ii-1].revbaowen;//读取的报文
    int jx_sss=QString::number(revd[ii-1].revbw,10).toInt();
    addrsum=0;
    for (int i=0;i<ii-9;i++)//接收求校验和
    {
        addrsum+=revd[i+8].revbw;

    }
    jx_xyh=addrsum&0xff;

if (jx_xyh==jx_sss)//实际校验和与初始校验和是否相等
{
    int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
    //QString aa =revd[34].revbaowen ;
    QString num = jx_ss.mid(62,5);// 00 40  相当于buf[]
    QString num1 =jx_ss.mid(68,5);//00 00
    QString num2 =jx_ss.mid(74,5);// de 04
    QString num3 =jx_ss.mid(80,5);//04 00
    QString num4 =jx_ss.mid(86,5); //1a 0e
    QString num5 =jx_ss.mid(92,5); //CT电压
    QString num6 =jx_ss.mid(98,2); //00
    QString num7 =jx_ss.mid(101,2);//00

    QString num_1 =num.left(2);   //00
    QString num_2 =num.right(2);  //40


    float  fhdl=num_2.append(num_1).mid(1,3).toInt(&ok,16)/8;
    QString sfhdl = QString::number(fhdl,'f', 2);//f是参数格式
    ui->tableWidget_realState->item(in_row,2)->setText(sfhdl);//线路负荷电流
    //double a=33.22/8;
    QString num1_1 = num1.left(2); //00
    QString num1_2 = num1.right(2);//00
    int  dlls =num1_2.append(num1_1).toInt(&ok,16);
    QString sdlls =QString::number(dlls,10);
    ui->tableWidget_realState->item(in_row,3)->setText(sdlls);//电流裸数值

    QString num2_1 =num2.left(2);    //b0
    QString num2_2 =num2.right(2);   //04
    float drdy=num2_2.append(num2_1).toInt(&ok,16)/1000;//电容电压
    QString sdrdy =QString::number(drdy,'f', 2);
    ui->tableWidget_realState->item(in_row,4)->setText(sdrdy);

    QString num3_1 =num3.left(2);     //00
    QString num3_2 =num3.right(2);    //00
    float dcpj=num3_2.append(num3_1).toInt(&ok,16)/8;//电场平均值
    QString sdcpj=QString::number(dcpj,'f', 2);
    ui->tableWidget_realState->item(in_row,5)->setText(sdcpj);

    QString num4_1 =num4.left(2);    //88
    QString num4_2 =num4.right(2);   //0f  电池电压
    QString numdc=num4_2.append(num4_1);
    float dc=numdc.toInt(&ok,16)/1000;
    QString str = QString::number(dc, 'f', 2);
    ui->tableWidget_realState->item(in_row,6)->setText(str);

    // 	QString num5_1 =num5.left(2); //40
    // 	QString num5_2 =num2.right(2);//1f
    ui->tableWidget_realState->item(in_row,7)->setText(num5);
    ui->tableWidget_realState->item(in_row,8)->setText(num7);
    afg=0;
}
else
   {
       QMessageBox::critical(this,QString::fromUtf8("错误") , QString::fromUtf8("回复报文校验和错误"));
   }



}
void MainWindow::TScus()
{
    char buf[21]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x36,0x00,0x00,0x00,0x00,0x00,0x52,0x16};//校验和计算按照index的不同，功能码不同 在从新计算



    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];//不加uchar会有负数
//        qDebug()<<addrsum;
    }
//    qDebug()<<addrsum;
    buf[19]=addrsum&0xff;;//校验和

    serial->write(buf, 21);
    QString snd(QString::fromLocal8Bit("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));//此静态函数在给定的时间间隔后调用插槽。
}
void MainWindow::XBcus()
{
    char buf[21]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x32,0x16};//校验和计算按照index的不同，功能码不同 在从新计算
    buf[13]=0xd9;
    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];//不加uchar会有负数
//        qDebug()<<addrsum;
    }
//    qDebug()<<addrsum;
    buf[19]=addrsum&0xff;;//校验和

    serial->write(buf, 21);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));
}
void MainWindow::JX_jb()//校表解析
{
    int jx_xyh;
    QString jx_ss=revd[ii-1].revbaowen;
    int jx_sss=QString::number(revd[ii-1].revbw,10).toInt();
    addrsum=0;
    for (int i=0;i<ii-9;i++)
    {
        addrsum+=revd[i+8].revbw;//不加uchar会有负数
        //qDebug()<<addrsum;

    }
    jx_xyh=addrsum&0xff;//十进制
    qDebug()<<jx_xyh;
    if (jx_xyh==jx_sss)
    {
        int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数

        QString num = jx_ss.mid(62,5);// 00 40
        QString num1 =jx_ss.mid(68,5);//00 00
        QString num2 =jx_ss.mid(74,5);// de 04
        QString num3 =jx_ss.mid(80,5);//04 00
        QString num4 =jx_ss.mid(86,5); //1a 0e
        QString num5 =jx_ss.mid(92,5); //40 1f
        QString num6 =jx_ss.mid(98,5); //00
        QString num7 =jx_ss.mid(104,5);//00
//        QString num8 =jx_ss.mid(110,5);//
//        QString num9 =jx_ss.mid(116,5);//
//        QString num10 =jx_ss.mid(122,5);//
//        QString num11 =jx_ss.mid(128,5); //
//        QString num12 =jx_ss.mid(134,5); //
//        QString num13 =jx_ss.mid(140,5);

        QString num_1 =num.left(2);  //40
        QString num_2 =num.right(2);   //00

        int  fhd1=num_2.append(num_1).toInt(&ok,16);
        QString sfh1 = QString::number(fhd1, 10);
        ui->tableWidget_xiaobiao->item(in_row,2)->setText(sfh1);

        QString num1_1 =num1.left(2);
        QString num1_2 =num1.right(2);
        int  fhd2=num1_2.append(num1_1).toInt(&ok,16);
        QString sfhd2 = QString::number(fhd2, 10);
        ui->tableWidget_xiaobiao->item(in_row,3)->setText(sfhd2);

        QString num2_1 =num2.left(2);
        QString num2_2 =num2.right(2);
        int  fhd3=num2_2.append(num2_1).toInt(&ok,16);
        QString sfhd3 = QString::number(fhd3,10);
        ui->tableWidget_xiaobiao->item(in_row,4)->setText(sfhd3);

        QString num3_1 =num3.left(2);
        QString num3_2 =num3.right(2);
        int  fhd4=num3_2.append(num3_1).toInt(&ok,16);
        QString sfhd4 = QString::number(fhd4,10);
        ui->tableWidget_xiaobiao->item(in_row,5)->setText(sfhd4);

        QString num4_1 =num4.left(2);
        QString num4_2 =num4.right(2);
        int  fhd5=num4_2.append(num4_1).toInt(&ok,16);
        QString sfhd5 = QString::number(fhd5,10);
        ui->tableWidget_xiaobiao->item(in_row,6)->setText(sfhd5);

        QString num5_1 =num5.left(2);
        QString num5_2 =num5.right(2);
        int  fhd6=num5_2.append(num5_1).toInt(&ok,16);
        QString sfhd6 = QString::number(fhd6,10);
        ui->tableWidget_xiaobiao->item(in_row,7)->setText(sfhd6);

        QString num6_1 =num6.left(2);
        QString num6_2 =num6.right(2);
        int  fhd7=num6_2.append(num6_1).toInt(&ok,16);
        QString sfhd7 = QString::number(fhd7,10);
        ui->tableWidget_xiaobiao->item(in_row,8)->setText(sfhd7);

        QString num7_1 =num7.left(2);
        QString num7_2 =num7.right(2);
        int  fhd8=num7_2.append(num7_1).toInt(&ok,16);
        QString sfhd8 = QString::number(fhd8,10);
        ui->tableWidget_xiaobiao->item(in_row,9)->setText(sfhd8);

//        QString num8_1 =num8.left(2);
//        QString num8_2 =num8.right(2);
//        int  fhd9=num8_2.append(num8_1).toInt(&ok,16);
//        QString sfhd9 = QString::number(fhd9,10);
//        ui->tableWidget_xiaobiao->item(in_row,10)->setText(sfhd9);

//        QString num9_1 =num9.left(2);
//        QString num9_2 =num9.right(2);
//        int  fhd10=num9_2.append(num9_1).toInt(&ok,16);
//        QString sfhd10 = QString::number(fhd10,10);
//        ui->tableWidget_xiaobiao->item(in_row,11)->setText(sfhd10);

//        QString num10_1 =num10.left(2);
//        QString num10_2 =num10.right(2);
//        int  fhd11=num10_2.append(num10_1).toInt(&ok,16);
//        QString sfhd11 = QString::number(fhd11,10);
//        ui->tableWidget_xiaobiao->item(in_row,12)->setText(sfhd11);

//        QString num11_1 =num11.left(2);
//        QString num11_2 =num11.right(2);
//        int  fhd12=num11_2.append(num11_1).toInt(&ok,16);
//        QString sfhd12 = QString::number(fhd12,10);
//        ui->tableWidget_xiaobiao->item(in_row,13)->setText(sfhd12);

//        QString num12_1 =num12.left(2);
//        QString num12_2 =num12.right(2);
//        int  fhd13=num12_2.append(num12_1).toInt(&ok,16);
//        QString sfhd13 = QString::number(fhd13,10);
//        ui->tableWidget_xiaobiao->item(in_row,14)->setText(sfhd13);

//        QString num13_1 =num13.left(2);
//        QString num13_2 =num13.right(2);
//        int  fhd14=num13_2.append(num13_1).toInt(&ok,16);
//        QString sfhd14 = QString::number(fhd14,10);
//        ui->tableWidget_xiaobiao->item(in_row,15)->setText(sfhd14);
    }
    else
    {
        QMessageBox::critical(this,QString::fromLocal8Bit("错误") , QString::fromLocal8Bit("回复报文校验和错误"));
   }
}
void MainWindow::D_chicus()
{
    char buf[21]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x33,0x00,0x00,0x00,0x00,0x00,0x52,0x16};//校验和计算按照index的不同，功能码不同 在从新计算
    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    buf[13]=0xd1;
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];
//        qDebug()<<addrsum;
    }
//    qDebug()<<addrsum;
    buf[19]=addrsum&0xff;//校验和

    serial->write(buf, 21);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));//10000毫秒
}
void MainWindow::JX_Dchi()
{
    QString jx_ss=revd[ii-1].revbaowen;



    int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
    //QString aa =revd[34].revbaowen ;
    QString num = jx_ss.mid(62,5);//  低电压告警
    QString num1 =jx_ss.mid(68,5);//电池周期

    QString num_2 =num.right(2);  //
    QString num_1 =num.left(2);   //
    int  jxnum=num_2.append(num_1).toInt(&ok,16);
    QString sjxnum = QString::number(jxnum, 10);
    ui->tableWidget_dianchi->item(in_row,2)->setText(sjxnum);

    QString num1_1=num1.left(2);
    QString num1_2=num1.right(2);
    int  jxnum1=num1_2.append(num1_1).toInt(&ok,16);
    QString sjxnum1 = QString::number(jxnum1, 10);
    ui->tableWidget_dianchi->item(in_row,3)->setText(sjxnum1);
}
void MainWindow::D_chcus()
{
    char buf[21]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x15,0x00,0x00,0x00,0x00,0x00,0x52,0x16};//校验和计算按照index的不同，功能码不同 在从新计算
    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];//不加uchar会有负数
//        qDebug()<<addrsum;
    }

    buf[19]=addrsum&0xff;;//校验和

    serial->write(buf, 21);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));
}
void MainWindow::JX_Dchang()
{
    QString jx_ss=revd[ii-1].revbaowen;
    int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
    QString num = jx_ss.mid(62,3);// 00 40
    QString num1 =jx_ss.mid(66,3);//00 00
        QString num2 =jx_ss.mid(70,3);



    int  num_1=num.toInt(&ok,16);  //
    int  num1_1 =num1.toInt(&ok,16);   //
    int  num2_1=num2.toInt(&ok,16);
    QString  snum_1=QString::number(num_1,10);
    QString  snum_2=QString::number(num1_1,10);
    QString  snum_3=QString::number(num2_1,10);
    ui->tableWidget_dianchang->item(in_row,2)->setText(snum_1);
    ui->tableWidget_dianchang->item(in_row,3)->setText(snum_2);
    ui->tableWidget_dianchang->item(in_row,4)->setText(snum_3);

}
void MainWindow::JDcus()
{

    char buf[21]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x26,0x00,0x00,0x00,0x00,0x00,0x52,0x16};//校验和计算按照index的不同，功能码不同 在从新计算
    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    buf[13]=0x2e;
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];//不加uchar会有负数
//        qDebug()<<addrsum;
    }
//    qDebug()<<addrsum;
    buf[19]=addrsum&0xff;//校验和

    serial->write(buf, 21);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));
}
void MainWindow::JX_jdi()
{
    QString jx_ss=revd[ii-1].revbaowen;
    int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
    //QString aa =revd[34].revbaowen ;
    QString num = jx_ss.mid(62,5);//  电流突变量
    QString num1 =jx_ss.mid(68,5);//电场阈值
     QString num2 =jx_ss.mid(74,5);//下降比例
      QString num3 =jx_ss.mid(80,5);//下降时间

    QString num_2 =num.right(2);  //
    QString num_1 =num.left(2);   //
    int  jxnum=num_2.append(num_1).toInt(&ok,16);
    QString sjxnum = QString::number(jxnum, 10);
    ui->tableWidget_jiedi->item(in_row,2)->setText(sjxnum);

    QString num1_1=num1.left(2);
    QString num1_2=num1.right(2);
    int  jxnum1=num1_2.append(num1_1).toInt(&ok,16);
    QString sjxnum1 = QString::number(jxnum1, 10);
    ui->tableWidget_jiedi->item(in_row,3)->setText(sjxnum1);

    QString num2_1=num2.left(2);
    QString num2_2=num2.right(2);
    int  jxnum2=num2_2.append(num2_1).toInt(&ok,16);
    QString sjxnum2 = QString::number(jxnum2, 10);
    ui->tableWidget_jiedi->item(in_row,4)->setText(sjxnum2);

    QString num3_1=num3.left(2);
    QString num3_2=num3.right(2);
    int  jxnum3=num3_2.append(num3_1).toInt(&ok,16);
    QString sjxnum3 = QString::number(jxnum3, 10);
    ui->tableWidget_jiedi->item(in_row,5)->setText(sjxnum3);


}
void MainWindow:: DLcus()
{
    char buf[21]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x2c,0x00,0x00,0x00,0x00,0x00,0x25,0x16};
    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];//不加uchar会有负数
//        qDebug()<<addrsum;
    }
//    qDebug()<<addrsum;
    buf[19]=addrsum&0xff;;//校验和

    serial->write(buf,21);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));
}
//void MainWindow::JX_Dlu()
//{

//}
void MainWindow::JDiancus()
{


    char buf[24]={0x15,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x09,0x02,0x02,0x02,0x6c,0x00,0x00,0x00,0x00,0x00,0x52,0x16};
    buf[1]=s1.toInt(&ok,16);
    buf[2]=s2.toInt(&ok,16);
    buf[3]=s3.toInt(&ok,16);
    buf[16]=s1.toInt(&ok,16);
    buf[17]=s2.toInt(&ok,16);
    buf[18]=s3.toInt(&ok,16);
    buf[13]=0xd5;
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];
//        qDebug()<<addrsum;
    }
    buf[19]=addrsum&0xff;
    serial->write(buf,24);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(20000, this, SLOT(timerOut()));
}
void MainWindow::JX_jd()
{
    int jx_xyh;
    QString jx_ss=revd[ii-1].revbaowen;
    int jx_sss=QString::number(revd[ii-1].revbw,10).toInt();
    addrsum=0;
    for (int i=0;i<ii-9;i++)
    {
        addrsum+=revd[i+8].revbw;
        //qDebug()<<addrsum;

    }
    jx_xyh=addrsum&0xff;//十进制
    if (jx_xyh==jx_sss)
    {
    int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
    QString num = jx_ss.mid(62,8); //邻节点1
    ui->tableWidget_jiedian->item(in_row,2)->setText(num);
    QString num1 =jx_ss.mid(71,8);//邻节点2
    ui->tableWidget_jiedian->item(in_row,3)->setText(num1);
    QString num2 =jx_ss.mid(80,8);//父节点
    ui->tableWidget_jiedian->item(in_row,4)->setText(num2);
    QString num3 =jx_ss.mid(89,2);//中继级数
    ui->tableWidget_jiedian->item(in_row,5)->setText(num3);
    QString num4 =jx_ss.mid(92,2);//支路数
    ui->tableWidget_jiedian->item(in_row,6)->setText(num4);
    int  num5 =jx_ss.mid(95,2).toInt(&ok,16);

          //相位
    if (num5==1)
     {
        ui->tableWidget_jiedian->item(in_row,7)->setText("A");
     }
    else if(num5==2)
     {
        ui->tableWidget_jiedian->item(in_row,7)->setText("B");
     }
    else if(num5==3)
     {
        ui->tableWidget_jiedian->item(in_row,7)->setText("C");
     }
    }
    else
    {
        QMessageBox::critical(this,QString::fromUtf8("错误") , QString::fromUtf8("回复报文校验和错误"));
    }
}
//单台设备读参数
void MainWindow::on_read_single()//右键单台单独读取
{
    bool ok;
    int index = ui->tabWidget_caiji->currentIndex();

// 	map<int, QTableWidget*>::iterator t_iter = tables.find(index);//跳转到保存界面的参数的函数
// 	map<int, PFUNC>::iterator f_iter = caijiFuncPtrs.find(index);
// 	(this->*(f_iter->second))();
//
// 	QTableWidget *table = t_iter->second;
    int rowNo = ui->treeWidget_3->currentIndex().row();

    QString addr =ui->treeWidget_3->currentItem()->text(0); //获取当前的读取 的地址
//    qDebug()<<addr;
    //checkself(QTreeWidgetItem*item ,int));
    /*组报文（读）*/


//    int aa ;
//    map<int, REALSTATE_PARAM>::iterator i_zhi =g_MAP_REALSTATE_PARAM.begin();
//    for(;i_zhi!=g_MAP_REALSTATE_PARAM.end();++i_zhi)
//        int aa=g_MAP_REALSTATE_PARAM[0].addr;//
//        int bb=g_MAP_REALSTATE_PARAM[1].addr;
//        int cc=g_MAP_REALSTATE_PARAM[2].addr;
//        int dd=g_MAP_REALSTATE_PARAM[3].addr;
    while (addr.size() < 6)
    {
        addr.prepend("0");
    }


     s1=addr.mid(0,2);
     s2=addr.mid(2,2);
     s3=addr.mid(4,2);



      if (index==0)
      {
//        awaken_aa();//唤醒帧
//        sleep(10000);
        SScus();//575
        i_dex=0;
      }
      else if (index==1)
      {
          TScus();
          i_dex=1;
      }
      else if (index==2)
      {
          XBcus();
          i_dex=2;
      }
      else if (index==3)
      {
          D_chicus();
          i_dex=3;
      }
      else if (index==4)
      {
          D_chcus();
          i_dex=4;
      }
      else if (index==5)
      {
          JDcus();
          i_dex=5;
      }
      else if (index==6)
      {
           DLcus();
           i_dex=6;
      }
      else if (index==7)
      {
          JDiancus();
          i_dex=7;
      }

}
//单台设备写参数
void MainWindow::on_write_single()
{
    int index = ui->tabWidget_caiji->currentIndex();

//    QMap<int, QTableWidget*>::iterator t_iter = tables.find(index);
//    QMap<int, PFUNC>::iterator f_iter = caijiFuncPtrs.find(index);//对蓝牙笔发送过来的地址关联给各表格
//    (this->*(f_iter.value()))();


//    QTableWidget *table = t_iter.value();
    int rowNo = ui->treeWidget_3->currentIndex().row();

    QString addr =ui->treeWidget_3->currentItem()->text(0); //获取当前的读取 的地址
    while (addr.size() < 6)
    {
        addr.prepend("0");
    }


    QString sx1=addr.mid(0,2);
    QString sx2=addr.mid(2,2);
    QString sx3=addr.mid(4,2);


    /*组报文（写）*/
    if (index==1)//改地址,调试地址写参数    以10进制的int变量直接赋值给buf数组的元素 自动变成16进制  ？？没弄懂
    {

        int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数

        QString addr1 =ui->tableWidget_debugAddr->item(in_row, 2)->text();//第二列
        QString sx4=addr1.mid(0,2);
        QString sx5=addr1.mid(2,2);
        QString sx6=addr1.mid(4,2);
        char buf[24]={0x18,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x0c,0x02,0x02,0x02,0x1e,0x00,0x00,0x00,0x00,0x00,0x22,0x22,0x22,0x52,0x16};
        buf[1]=sx1.toInt(&ok,16);//获取地址
        buf[2]=sx2.toInt(&ok,16);
        buf[3]=sx3.toInt(&ok,16);
        buf[16]=sx1.toInt(&ok,16);
        buf[17]=sx2.toInt(&ok,16);
        buf[18]=sx3.toInt(&ok,16);
        buf[19]=sx4.toInt(&ok,16);
        buf[20]=sx5.toInt(&ok,16);
        buf[21]=sx6.toInt(&ok,16);
        addrsum=0;
        for (int i=0;i<buf[9]+2;i++)
        {
            addrsum+=(uchar)buf[i+8];//不加uchar会有负数
            qDebug()<<addrsum;
        }
        buf[22]=addrsum&0xff;
        serial->write(buf,24);
        QString snd(QString::fromUtf8("发送: "));
        for (int i = 0;i < buf[0]; i++)
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            snd.append(stemp);

        }
        ui->textEdit->append(snd);
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(10000, this, SLOT(timerOut()));

    }
    else if (index==2)//两个字节需转化拆开
    {

        int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
        int  saddr1 =ui->tableWidget_xiaobiao->item(in_row, 2)->text().toInt(&ok,10);//从界面输取十进制
        QString addr1=QString::number(saddr1,16);//转换16进制
        while (addr1.size() < 4)
        {
            addr1.prepend("0");//自动补全零
        }
        int addr1_1=addr1.mid(0,2).toInt(&ok,16);//转化为十进制
        int addr1_2=addr1.mid(2,2).toInt(&ok,16);
        int saddr2 =ui->tableWidget_xiaobiao->item(in_row, 3)->text().toInt(&ok,10);
        QString addr2=QString::number(saddr2,16);
        while (addr2.size() < 4)
        {
            addr2.prepend("0");
        }
        QString addr2_1=addr2.mid(0,2);
        QString addr2_2=addr2.mid(2,2);
        int saddr3 =ui->tableWidget_xiaobiao->item(in_row, 4)->text().toInt(&ok,10);
        QString addr3=QString::number(saddr3,16);
        while (addr3.size() < 4)
        {
            addr3.prepend("0");
        }
        QString addr3_1=addr3.mid(0,2);
        QString addr3_2=addr3.mid(2,2);
        int saddr4 =ui->tableWidget_xiaobiao->item(in_row, 5)->text().toInt(&ok,10);
        QString addr4=QString::number(saddr4,16);
        while (addr4.size() < 4)
        {
            addr4.prepend("0");
        }
        QString addr4_1=addr4.mid(0,2);
        QString addr4_2=addr4.mid(2,2);
        int saddr5 =ui->tableWidget_xiaobiao->item(in_row, 6)->text().toInt(&ok,10);
        QString addr5=QString::number(saddr5,16);
        while (addr5.size() < 4)
        {
            addr5.prepend("0");
        }
        QString addr5_1=addr5.mid(0,2);
        QString addr5_2=addr5.mid(2,2);
        int  saddr6 =ui->tableWidget_xiaobiao->item(in_row, 7)->text().toInt(&ok,10);
        QString addr6=QString::number(saddr6,16);
        while (addr6.size() < 4)
        {
            addr6.prepend("0");
        }
        QString addr6_1=addr6.mid(0,2);
        QString addr6_2=addr6.mid(2,2);
        int saddr7 =ui->tableWidget_xiaobiao->item(in_row, 8)->text().toInt(&ok,10);
        QString addr7=QString::number(saddr7,16);
        while (addr7.size() < 4)
        {
            addr7.prepend("0");
        }
        QString addr7_1=addr7.mid(0,2);
        QString addr7_2=addr7.mid(2,2);
        int saddr8 =ui->tableWidget_xiaobiao->item(in_row, 9)->text().toInt(&ok,10);
        QString addr8=QString::number(saddr8,16);
        while (addr8.size() < 4)
        {
            addr8.prepend("0");
        }
        QString addr8_1=addr8.mid(0,2);
        QString addr8_2=addr8.mid(2,2);
        int saddr9 =ui->tableWidget_xiaobiao->item(in_row, 10)->text().toInt(&ok,10);
        QString addr9=QString::number(saddr9,16);
        while (addr9.size() < 4)
        {
            addr9.prepend("0");
        }
        QString addr9_1=addr9.mid(0,2);
        QString addr9_2=addr9.mid(2,2);
        int  saddr10 =ui->tableWidget_xiaobiao->item(in_row, 11)->text().toInt(&ok,10);
        QString addr10=QString::number(saddr10,16);
        while (addr10.size() < 4)
        {
            addr10.prepend("0");
        }
        QString addr10_1=addr10.mid(0,2);
        QString addr10_2=addr10.mid(2,2);
        int saddr11 =ui->tableWidget_xiaobiao->item(in_row, 12)->text().toInt(&ok,10);
        QString addr11=QString::number(saddr11,16);
        while (addr11.size() < 4)
        {
            addr11.prepend("0");
        }
        int addr11_1=addr11.mid(0,2).toInt(&ok,16);
        int addr11_2=addr11.mid(2,2).toInt(&ok,16);
        int saddr12 =ui->tableWidget_xiaobiao->item(in_row, 13)->text().toInt(&ok,10);
        QString addr12=QString::number(saddr12,16);
        while (addr12.size() < 4)
        {
            addr12.prepend("0");
        }
        QString addr12_1=addr12.mid(0,2);
        QString addr12_2=addr12.mid(2,2);

        int saddr13 =ui->tableWidget_xiaobiao->item(in_row, 14)->text().toInt(&ok,10);
        QString addr13=QString::number(saddr13,16);
        while (addr13.size() < 4)
        {
            addr13.prepend("0");
        }
        QString addr13_1=addr13.mid(0,2);
        QString addr13_2=addr13.mid(2,2);

        int saddr14 =ui->tableWidget_xiaobiao->item(in_row, 15)->text().toInt(&ok,10);
        QString addr14=QString::number(saddr14,16);
        while (addr14.size() < 4)
        {
            addr14.prepend("0");
        }
        QString addr14_1=addr14.mid(0,2);
        QString addr14_2=addr14.mid(2,2);

            char buf[49]={0x31,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x25,0x02,0x02,0x02,0x4f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16};
        buf[1]=sx1.toInt(&ok,16);
        buf[2]=sx2.toInt(&ok,16);
        buf[3]=sx3.toInt(&ok,16);
        buf[16]=sx1.toInt(&ok,16);
        buf[17]=sx2.toInt(&ok,16);
        buf[18]=sx3.toInt(&ok,16);
        buf[19]=addr1_2;
        buf[20]=addr1_1;
        buf[21]=addr2_2.toInt(&ok,16);
        buf[22]=addr2_1.toInt(&ok,16);
        buf[23]=addr3_2.toInt(&ok,16);
        buf[24]=addr3_1.toInt(&ok,16);
        buf[25]=addr4_2.toInt(&ok,16);
        buf[26]=addr4_1.toInt(&ok,16);
        buf[27]=addr5_2.toInt(&ok,16);
        buf[28]=addr5_1.toInt(&ok,16);
        buf[29]=addr6_2.toInt(&ok,16);
        buf[30]=addr6_1.toInt(&ok,16);
        buf[31]=addr7_2.toInt(&ok,16);
        buf[32]=addr7_1.toInt(&ok,16);
        buf[33]=addr8_2.toInt(&ok,16);
        buf[34]=addr8_1.toInt(&ok,16);
        buf[35]=addr9_2.toInt(&ok,16);
        buf[36]=addr9_1.toInt(&ok,16);
        buf[37]=addr10_2.toInt(&ok,16);
        buf[38]=addr10_1.toInt(&ok,16);
        buf[39]=addr11_2;
        buf[40]=addr11_1;
        buf[41]=addr12_2.toInt(&ok,16);
        buf[42]=addr12_1.toInt(&ok,16);
        buf[43]=addr13_2.toInt(&ok,16);
        buf[44]=addr13_1.toInt(&ok,16);
        buf[45]=addr14_2.toInt(&ok,16);
        buf[46]=addr14_1.toInt(&ok,16);

        addrsum=0;
        for (int i=0;i<buf[9]+2;i++)
        {
            addrsum+=(uchar)buf[i+8];//不加uchar会有负数
            qDebug()<<addrsum;
        }
        qDebug()<<addrsum;
        buf[47]=addrsum&0xff;//校验和





        serial->write(buf,49);//49为数据输出的大小
        QString snd(QString::fromUtf8("发送: "));
        for (int i = 0;i < buf[0]; i++)
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            snd.append(stemp);

        }
        ui->textEdit->append(snd);
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(10000, this, SLOT(timerOut()));

          }

    else if (index==3)//电池参数
    {
        int in_row=ui->treeWidget_3->currentIndex().row();


    int saddr1 =ui->tableWidget_dianchi->item(in_row, 2)->text().toInt(&ok,10);
    QString addr1=QString::number(saddr1,16);//转换16进制
    while (addr1.size() < 4)
        {
                    addr1.prepend("0");
        }
    int addr1_1=addr1.mid(0,2).toInt(&ok,16);
    int addr1_2=addr1.mid(2,2).toInt(&ok,16);
    int saddr2 =ui->tableWidget_dianchi->item(in_row, 3)->text().toInt(&ok,10);
    QString addr2=QString::number(saddr2,16);//转换16进制
    while (addr2.size() < 4)
    {
        addr2.prepend("0");
    }
    int addr2_1=addr2.mid(0,2).toInt(&ok,16);
    int addr2_2=addr2.mid(2,2).toInt(&ok,16);

    char buf[25]={0x19,0x01,0x01,0x01,0x02,0x02,0x02,0x00,0x7e,0x0d,0x02,0x02,0x02,0x12,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x11,0x16};
    buf[13]=0xd2;
    buf[1]=sx1.toInt(&ok,16);
    buf[2]=sx2.toInt(&ok,16);
    buf[3]=sx3.toInt(&ok,16);
    buf[16]=sx1.toInt(&ok,16);
    buf[17]=sx2.toInt(&ok,16);
    buf[18]=sx3.toInt(&ok,16);
    buf[19]=addr1_1;
    buf[20]=addr1_2;
    buf[21]=addr2_1;
    buf[22]=addr2_2;
    addrsum=0;
    for (int i=0;i<buf[9]+2;i++)
    {
        addrsum+=(uchar)buf[i+8];

    }
    buf[23]=addrsum&0xff;
     qDebug()<<addrsum;
    serial->write(buf,25);
    QString snd(QString::fromUtf8("发送: "));
    for (int i = 0;i < buf[0]; i++)
    {
        QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
        snd.append(stemp);

    }
    ui->textEdit->append(snd);
    m_readBuf.clear();
    ui->centralWidget->setEnabled(false);

    QTimer::singleShot(10000, this, SLOT(timerOut()));
    }
    else if (index==4)//电场突变 1个字节可以直接变
    {
        int in_row=ui->treeWidget->currentIndex().row();//获取点击的当前的行数
        int saddr1 =ui->tableWidget_dianchi->item(in_row, 2)->text().toInt();


        int saddr2 =ui->tableWidget_dianchang->item(in_row, 3)->text().toInt();

        int saddr3 =ui->tableWidget_dianchang->item(in_row, 4)->text().toInt();


        char buf[25]={0x11,0x01,0x01,0x01,0x02,0x02,0x02,0x00,0x7e,0x11,0x02,0x02,0x02,0x11,0x00,0x00,0x01,0x01,0x01,0x11,0x11,0x05,0x00,0x11,0x16};
        buf[1]=sx1.toInt(&ok,16);
        buf[2]=sx2.toInt(&ok,16);
        buf[3]=sx3.toInt(&ok,16);
        buf[16]=sx1.toInt(&ok,16);
        buf[17]=sx2.toInt(&ok,16);
        buf[18]=sx3.toInt(&ok,16);
        buf[19]=saddr1;
        buf[20]=saddr2;
        buf[21]=saddr3;
        addrsum=0;
        for (int i=0;i<buf[9]+2;i++)
        {
            addrsum+=(uchar)buf[i+8];
            qDebug()<<addrsum;
        }
        buf[22]=addrsum&0xff;
        serial->write(buf,24);
        QString snd(QString::fromLocal8Bit("发送: "));
        for (int i = 0;i < buf[0]; i++)
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            snd.append(stemp);

        }
        ui->textEdit->append(snd);
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(10000, this, SLOT(timerOut()));


    }
    else if (index==5)//接地电流突变写入
    {
        int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
// 		int  saddr1 =ui.tableWidget_xiaobiao->item(in_row, 2)->text().toInt();//从界面取所填写的
// 		QString addr1=QString::number(saddr1,16);//转换16进制

        int addr1 =ui->tableWidget_jiedi->item(in_row, 2)->text().toInt();
        int addr2 =ui->tableWidget_jiedi->item(in_row, 3)->text().toInt();
        int addr3 =ui->tableWidget_jiedi->item(in_row, 4)->text().toInt();
        int addr4 =ui->tableWidget_jiedi->item(in_row, 5)->text().toInt();
        int addr5 =ui->tableWidget_jiedi->item(in_row, 6)->text().toInt();

        char buf[26]={0x1a,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x0e,0x02,0x02,0x02,0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x16};

        buf[1]=sx1.toInt(&ok,16);
        buf[2]=sx2.toInt(&ok,16);
        buf[3]=sx3.toInt(&ok,16);
        buf[16]=sx1.toInt(&ok,16);
        buf[17]=sx2.toInt(&ok,16);
        buf[18]=sx3.toInt(&ok,16);
        buf[19]=addr1;
        buf[20]=addr2;
        buf[21]=addr3;
        buf[22]=addr4;
        buf[23]=addr5;
        addrsum=0;
        for (int i=0;i<buf[9]+2;i++)
        {
            addrsum+=(uchar)buf[i+8];
            qDebug()<<addrsum;
        }
        buf[24]=addrsum&0xff;
        serial->write(buf,25);
        QString snd(QString::fromLocal8Bit("发送: "));
        for (int i = 0;i < buf[0]; i++)
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            snd.append(stemp);

        }
        ui->textEdit->append(snd);
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(10000, this, SLOT(timerOut()));
    }
    else if (index==6)//短路电流
    {
        int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
        QString addr1 =ui->tableWidget_duanlu->item(in_row, 2)->text();
        QString addr2 =ui->tableWidget_duanlu->item(in_row, 3)->text();//2个字节
        int addr2_1=addr2.mid(0,2).toInt(&ok,16);
        int addr2_2=addr2.mid(2,2).toInt(&ok,16);
        QString addr3 =ui->tableWidget_duanlu->item(in_row, 4)->text();
        QString addr4 =ui->tableWidget_duanlu->item(in_row, 5)->text();//2个字节
        int  addr4_1=addr4.mid(0,2).toInt(&ok,16);
        int  addr4_2=addr4.mid(2,2).toInt(&ok,16);
        QString addr5 =ui->tableWidget_duanlu->item(in_row, 6)->text();
        char buf[28]={0x1c,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x0e,0x02,0x02,0x02,0x2d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16};

        buf[1]=sx1.toInt(&ok,16);
        buf[2]=sx2.toInt(&ok,16);
        buf[3]=sx3.toInt(&ok,16);
        buf[16]=sx1.toInt(&ok,16);
        buf[17]=sx2.toInt(&ok,16);
        buf[18]=sx3.toInt(&ok,16);
        buf[19]=addr1.toInt(&ok,16);
        buf[20]=addr2_1;
        buf[21]=addr2_2;
        buf[22]=addr3.toInt(&ok,16);
        buf[23]=addr4_1;
        buf[24]=addr4_2;
        buf[25]=addr5.toInt(&ok,16);
        addrsum=0;
        for (int i=0;i<buf[9]+2;i++)
        {
            addrsum+=(uchar)buf[i+8];
            qDebug()<<addrsum;
        }
        buf[26]=addrsum&0xff;
        serial->write(buf,28);
        QString snd(QString::fromLocal8Bit("发送: "));
        for (int i = 0;i < buf[0]; i++)
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            snd.append(stemp);

        }
        ui->textEdit->append(snd);
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(10000, this, SLOT(timerOut()));
    }
    else if (index==7)//节点参数
    {
        int in_row=ui->treeWidget_3->currentIndex().row();//获取点击的当前的行数
        QString addr1 =ui->tableWidget_jiedian->item(in_row, 2)->text();
        int sx1_1=addr1.mid(0,2).toInt(&ok,16);
        int sx1_2=addr1.mid(2,2).toInt(&ok,16);
        int sx1_3=addr1.mid(4,2).toInt(&ok,16);
        QString addr2 =ui->tableWidget_jiedian->item(in_row, 3)->text();//3个字节
        int sx2_1=addr2.mid(0,2).toInt(&ok,16);
        int sx2_2=addr2.mid(2,2).toInt(&ok,16);
        int sx2_3=addr2.mid(4,2).toInt(&ok,16);
        QString addr3 =ui->tableWidget_jiedian->item(in_row, 4)->text();
        int sx3_1=addr3.mid(0,2).toInt(&ok,16);
        int sx3_2=addr3.mid(2,2).toInt(&ok,16);
        int sx3_3=addr3.mid(4,2).toInt(&ok,16);
        QString addr4 =ui->tableWidget_jiedian->item(in_row, 5)->text();//1个字节
        int  sx4_1=addr4.toInt(&ok,16);
        QString addr5 =ui->tableWidget_jiedian->item(in_row, 6)->text();
        int  sx5_1=addr5.toInt(&ok,16);
        QString addr6 =ui->tableWidget_jiedian->item(in_row, 7)->text();
        int  sx6_1;
        if (addr6=="A")
        {
        sx6_1=1;
        }
        else if (addr6=="B")
        {
         sx6_1=2;
        }
        else if (addr6=="C")
        {
         sx6_1=3;
        }

        if (addr1==NULL||addr2==NULL||addr3==NULL||addr4==NULL||addr5==NULL||addr6==NULL)
        {
             QMessageBox::information(NULL, QString("error"), QString(QString::fromLocal8Bit("有字节为空，写入错误 ")));
        }

        else
        {char buf[33]={0x21,0x00,0x00,0x00,0x02,0x02,0x02,0x00,0x7e,0x15,0x02,0x02,0x02,0x6d,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16};
        buf[1]=sx1.toInt(&ok,16);
        buf[2]=sx2.toInt(&ok,16);
        buf[3]=sx3.toInt(&ok,16);
        buf[16]=sx1.toInt(&ok,16);
        buf[17]=sx2.toInt(&ok,16);
        buf[18]=sx3.toInt(&ok,16);
        buf[19]=sx1_1;
        buf[20]=sx1_2;
        buf[21]=sx1_3;
        buf[22]=sx2_1;
        buf[23]=sx2_2;
        buf[24]=sx2_3;
        buf[25]=sx3_1;
        buf[26]=sx3_2;
        buf[27]=sx3_3;
        buf[28]=sx4_1;//中继
        buf[29]=sx5_1;//支路点
        buf[30]=sx6_1;//相位
        addrsum=0;
        for (int i=0;i<buf[9]+2;i++)
        {
            addrsum+=(uchar)buf[i+8];
            qDebug()<<addrsum;
        }
        buf[31]=addrsum&0xff;
        serial->write(buf,33);
        QString snd(QString::fromLocal8Bit("发送: "));
        for (int i = 0;i < buf[0]; i++)
        {
            QString stemp = QString(" %1").arg(buf[i]&0xff, 2, 16, QChar('0'));
            snd.append(stemp);

        }
        ui->textEdit->append(snd);
        m_readBuf.clear();
        ui->centralWidget->setEnabled(false);

        QTimer::singleShot(10000, this, SLOT(timerOut()));
       }
    }
}
void MainWindow::pl_read()//批量读取
{
    int index = ui->tabWidget_caiji->currentIndex();


    s1=pl_addr.mid(0,2);
    s2=pl_addr.mid(2,2);
    s3=pl_addr.mid(4,2);



    if (index==0)
    {
        SScus();
        i_dex=0;
    }
    else if (index==1)
    {
        TScus();
        i_dex=1;
    }
    else if (index==2)
    {
        XBcus();
        i_dex=2;
    }
    else if (index==3)
    {
        D_chicus();
        i_dex=3;
    }
    else if (index==4)
    {
        D_chcus();
        i_dex=4;
    }
    else if (index==5)
    {
        JDcus();
        i_dex=5;
    }
    else if (index==6)
    {
        DLcus();
        i_dex=6;
    }
    else if (index==7)
    {
        JDiancus();
        i_dex=7;
    }

}

//批量写参数
void MainWindow::on_button_write()
{
    int index = ui->tabWidget_caiji->currentIndex();

    QMap<int, QTableWidget*>::iterator t_iter = tables.find(index);
    QMap<int, PFUNC>::iterator f_iter = caijiFuncPtrs.find(index);
    (this->*(f_iter.value()))();//调用相应的函数指针

    QTableWidget *table = t_iter.value();

    /*组报文（写）*/

    //map<int, REALSTATE_PARAM>::iterator i_zhi =g_MAP_REALSTATE_PARAM.begin();
// 	for(;i_zhi!=g_MAP_REALSTATE_PARAM.end();++i_zhi)
// 		qDebug()<<"key:"<<i_zhi->first;
// 	    int aa=i_zhi->first;
//



}
//批量读参数
void MainWindow::on_button_read()
{

    int index = ui->tabWidget_caiji->currentIndex();

    QMap<int, QTableWidget*>::iterator t_iter = tables.find(index);//根据页码来判断
    QMap<int, PFUNC>::iterator f_iter = caijiFuncPtrs.find(index);
    (this->*(f_iter.value()))();//执行指向的函数 second表示map中第二个值

    QTableWidget *table = t_iter.value();//
    //send_bw();



}
//延时函数
void MainWindow::sleep(unsigned int msec)
{
    QTime reachTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<reachTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
void MainWindow::timerOut()
{
    if (ui->centralWidget->isEnabled()) //判断窗口小器件是否被启用
    {
        return;
    }
    else
    {
        QMessageBox::critical(this, QString::fromUtf8("超时"),QString::fromUtf8("超时"));
        ui->centralWidget->setEnabled(true);
    }

}
//汇集单元读写操作。
//void MainWindow::huijiConnects()
//{
//	connect(ui.pushButton_zhishi_write, SIGNAL(clicked()), this, SLOT(on_button_indicator_write_clicked()));//指示器
//	connect(ui.pushButton_zhishi_read, SIGNAL(clicked()), this, SLOT(on_button_indicator_read_clicked()));
//	connect(ui.pushButton_zuwang_write, SIGNAL(clicked()), this, SLOT(on_button_network_write_clicked()));//组网
//	connect(ui.pushButton_zuwang_read, SIGNAL(clicked()), this, SLOT(on_button_network_read_clicked()));
//	connect(ui.pushButton_tiaoshi_write, SIGNAL(clicked()), this, SLOT(on_button_debugaddr_write_clicked()));//调试
//	connect(ui.pushButton_tiaoshi_read, SIGNAL(clicked()), this, SLOT(on_button_debugaddr_read_clicked()));
//	connect(ui.pushButton_lianlu_write, SIGNAL(clicked()), this, SLOT(on_button_linkaddr_write_clicked()));
//	connect(ui.pushButton_lianlu_read, SIGNAL(clicked()), this, SLOT(on_button_linkaddr_read_clicked()));//链路
//}
//设置指示灯的状态颜色，操作成功变绿色，否则为红色。
//void MainWindow::ledState(QPushButton *btnLed, bool success)
//{
//    if(success)
//    {
//        btnLed->setStyleSheet("background-color:rgb(0, 255, 0)");
//    }
//    else
//    {
//        btnLed->setStyleSheet("background-color:rgb(255, 0, 0)");
//    }
//}
/*采集单元*/

//保存实时状态参数
void MainWindow::saveRealStateParam()
{
    g_MAP_REALSTATE_PARAM.clear();
    int rowCount = ui->tableWidget_realState->rowCount();

    for (int i = 0; i < rowCount; i++)
    {
        REALSTATE_PARAM temp;

        temp.no = ui->tableWidget_realState->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_realState->item(i, 1)->text();
        //temp.csaddr= ui.tableWidget_realState->item(i, 2)->text();
        g_MAP_REALSTATE_PARAM.insert(pair<int, REALSTATE_PARAM>(temp.no, temp));

        int rowNo = ui->tableWidget_realState->rowCount();


//        if (rowNo>1)
//        {
//            QTreeWidgetItemIterator it(ui->treeWidget_3);//
//            while (*it)
//            {
//                qDebug()<<(*it)->text(0);
//                afg=(*it)->text(0);
//                // 				if ((*it)->text(0) == "005566")
//                pl_read();

//                // 					(*it)->setSelected(true);
//                ++it;
//            }

//        }
//        else
//        {
//            on_read_single();
//        }

//            QString addr =ui->treeWidget_3->currentItem()->child(0)->text();
//            qDebug()<<"addr="<<addr;

 }
    for (int i=0;i < rowCount; i++)
    {


         pl_addr=g_MAP_REALSTATE_PARAM[i].addr;//批量下发 存在部分问题，是否定时或者等到指示器回复一条报文在下发一条 如此循环
        if (i==0||afg==0)
        {
             pl_read();//批量
        }


    }


}
//保存调试地址参数
void MainWindow::saveDebugAddrParam()
{
    g_MAP_DEBUGADDR_PARAM.clear();
    int rowCount = ui->tableWidget_debugAddr->rowCount();

    for (int i = 0; i < rowCount; i++)
    {
        DEBUGADDR_PARAM temp;

        temp.no = ui->tableWidget_debugAddr->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_debugAddr->item(i, 1)->text().toInt();
        temp.setParam = ui->tableWidget_debugAddr->item(i, 2)->text().toInt();
        temp.exeResult = ui->tableWidget_debugAddr->item(i, 3)->text().toInt();
        g_MAP_DEBUGADDR_PARAM.insert(pair<int, DEBUGADDR_PARAM>(temp.no, temp));
    }
}
//保存电池参数
void MainWindow::saveBattaryParam()
{
    VECT_BATTARY_PARAM g_VECT_BATTARY_PARAM;
    g_VECT_BATTARY_PARAM.clear();
    g_MAP_BATTARY_PARAM.clear();
    int rowCount = ui->tableWidget_dianchi->rowCount();
    for (int i = 0;i<rowCount;i++)
    {
        BATTARY_PARAM temp;
        temp.no = ui->tableWidget_dianchi->item(i,0)->text().toInt();
        temp.addr = ui->tableWidget_dianchi->item(i,1)->text().toInt();
        temp.LVD_alarm = ui->tableWidget_dianchi->item(i,2)->text().toInt();
        temp.cycle_alarm = ui->tableWidget_dianchi->item(i,3)->text().toInt();
        g_VECT_BATTARY_PARAM.push_back(temp);
        g_MAP_BATTARY_PARAM.insert(make_pair(temp.no, temp));
    }
}
//保存校表参数
void MainWindow::saveMeterParam()
{
    g_MAP_METER_PARAM.clear();
    int rowCount = ui->tableWidget_xiaobiao->rowCount();

    for (int i = 0; i < rowCount; i++)
    {
        METER_PARAM temp;
        temp.no = ui->tableWidget_xiaobiao->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_xiaobiao->item(i, 1)->text().toInt();

        int index = 0;
        for (int j = 2; j < ui->tableWidget_xiaobiao->columnCount();)
        {
            METER_SINGLE tempSin;
            tempSin.stdValue = ui->tableWidget_xiaobiao->item(i, j++)->text().toInt();
            tempSin.adpValue = ui->tableWidget_xiaobiao->item(i, j++)->text().toInt();
            tempSin.realValue = ui->tableWidget_xiaobiao->item(i, j++)->text().toInt();
            temp.meters.insert(pair<int, METER_SINGLE>(++index, tempSin));
        }

        g_MAP_METER_PARAM.insert(pair<int, METER_PARAM>(temp.addr, temp));
    }
}

//电场突变量参数
void MainWindow::saveEleFieldParam()
{
    g_MAP_ELEFIELD_PARAM.clear();
    int rowCount = ui->tableWidget_dianchang->rowCount();

    for(int i = 0; i < rowCount; i++)
    {
        ELEFIELD_PARAM temp;
        temp.no = ui->tableWidget_dianchang->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_dianchang->item(i, 1)->text().toInt();
        temp.eleHoldValue = ui->tableWidget_dianchang->item(i, 2)->text().toInt();
        temp.eleHoldReal = ui->tableWidget_dianchang->item(i, 3)->text().toInt();
// 		temp.downTime = ui.tableWidget_dianchang->item(i, 4)->text().toInt();
// 		temp.downTimeReal = ui.tableWidget_dianchang->item(i, 5)->text().toInt();
        //temp.downRatio = ui.tableWidget_dianchang->item(i, 6)->text().toInt();
        //temp.downRatioReal = ui.tableWidget_dianchang->item(i, 7)->text().toInt();
// 		temp.readParam = ui.tableWidget_dianchang->item(i, 8)->text().toInt();
// 		temp.writeParam = ui.tableWidget_dianchang->item(i, 9)->text().toInt();

        g_MAP_ELEFIELD_PARAM.insert(pair<int, ELEFIELD_PARAM>(temp.addr, temp));
    }
}

//接地电流突变量参数
void MainWindow::saveGroundCurParam()
{
    g_MAP_GROUNDCUR_PARAM.clear();
    int rowCount = ui->tableWidget_jiedi->rowCount();

    for(int i = 0; i < rowCount; i++)
    {
        GROUNDCUR_PARAM temp;
        temp.no = ui->tableWidget_jiedi->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_jiedi->item(i, 1)->text().toInt();
        temp.triMode = ui->tableWidget_jiedi->item(i, 2)->text().toInt();
        temp.groMinEle = ui->tableWidget_jiedi->item(i, 3)->text().toInt();
        temp.downRatio = ui->tableWidget_jiedi->item(i, 4)->text().toInt();
        temp.downTime = ui->tableWidget_jiedi->item(i, 5)->text().toInt();
        temp.groCurBre = ui->tableWidget_jiedi->item(i, 6)->text().toInt();
        g_MAP_GROUNDCUR_PARAM.insert(pair<int, GROUNDCUR_PARAM>(temp.addr, temp));
    }
}
//短路电流突变量参数
void MainWindow::saveShortCurParam()
{
    g_MAP_SHORTCUR_PARAM.clear();
    int rowCount = ui->tableWidget_duanlu->rowCount();

    for (int i = 0; i < rowCount; i++)
    {
        SHORTCUR_PARAM temp;
        temp.no = ui->tableWidget_duanlu->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_duanlu->item(i, 1)->text().toInt();
        temp.quickLimit = ui->tableWidget_duanlu->item(i, 2)->text().toInt();
        temp.quickDelay = ui->tableWidget_duanlu->item(i, 3)->text().toInt();
        temp.overLimit = ui->tableWidget_duanlu->item(i, 4)->text().toInt();
        temp.overDelay = ui->tableWidget_duanlu->item(i, 5)->text().toInt();
        temp.invoParam = ui->tableWidget_duanlu->item(i, 6)->text().toInt();
        g_MAP_SHORTCUR_PARAM.insert(pair<int, SHORTCUR_PARAM>(temp.addr, temp));
    }
}
//节点信息参数
void MainWindow::saveNodeInfoParam()
{
    g_MAP_NODEINFO_PARAM.clear();
    int rowCount = ui->tableWidget_jiedian->rowCount();

    for (int i = 0; i < rowCount; i++)
    {
        NODEINFO_PARAM temp;
        temp.no = ui->tableWidget_jiedian->item(i, 0)->text().toInt();
        temp.addr = ui->tableWidget_jiedian->item(i, 1)->text().toInt();
        temp.neiNodeOne = ui->tableWidget_jiedian->item(i, 2)->text().toInt();
        temp.neiNodeTwo = ui->tableWidget_jiedian->item(i, 3)->text().toInt();
        temp.parNode = ui->tableWidget_jiedian->item(i, 4)->text().toInt();
        temp.nodeNowRelay = ui->tableWidget_jiedian->item(i, 5)->text().toInt();
        temp.subNode = ui->tableWidget_jiedian->item(i, 6)->text().toInt();
        temp.phase = ui->tableWidget_jiedian->item(i, 7)->text().toInt();

        g_MAP_NODEINFO_PARAM.insert(pair<int, NODEINFO_PARAM>(temp.addr, temp));
    }
}
//保存所有采集参数
void MainWindow::saveCaijiParam()
{
    QMap<int, PFUNC>::iterator f_iter;

    for (f_iter = caijiFuncPtrs.begin(); f_iter != caijiFuncPtrs.end(); f_iter++)
    {
        (this->*(f_iter.value()))();
    }
}
//界面配置操作函数。
//void cfgtool::configFun()
//{
//    connect(ui.pushButton_dots, SIGNAL(clicked()), this, SLOT(on_button_dots_clicked()));
//    connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(on_button_save_clicked()));
//    connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(on_button_cancel_clicked()));
//}

void MainWindow::on_system_triggered()
{

    dialog=new SetDialog(this);//指定新窗口的父对象为mainwindow,当父对象关闭时子类也会关闭
    dialog->setModal(false);
    dialog->show();

}
//故障检测
/*---------------------------------------------libModBus------------------------------------------------------------*/
void MainWindow::f_openserial()//打开指定串口
{
        QString         portName;
        unsigned int    uartBaudRate;//是否要改成QString 形式
        unsigned char   uartParity;
        char            *a;
        unsigned int    uartStopbits;
        unsigned int    uartDataBits;
        int             ret = -1;

//      portName="COM2";
//      uartBaudRate=ui->comboBox_btl2->currentIndex();//currentIndex()此属性保存组合框中当前项的索引。
//      uartParity=ui->comboBox_xiaoy2->currentIndex();
//      uartStopbits=ui->comboBox_stop2->currentIndex();
//      uartDataBits=ui->comboBox_data2->currentIndex();
        portName=ui->comportname->currentText();//连接串口
        uartBaudRate=ui->comboBox_btl2->currentText().toInt();
        a=ui->comboBox_xiaoy2->currentText().toLatin1().data();
        uartParity = *a;
        uartStopbits=ui->comboBox_stop2->currentText().toInt();
        uartDataBits=ui->comboBox_data2->currentText().toInt();
        m_pModbus = modbus_new_rtu(portName.toLatin1().data(),
                                           (int)uartBaudRate,
                                           (char)uartParity,
                                           (int)uartDataBits,
                                           (int)uartStopbits);
//        qDebug()<<"123:"<<m_pModbus;
        if(m_pModbus != NULL)
               {

                   modbus_set_debug(m_pModbus, true);
                   ret = modbus_set_slave(m_pModbus, ADDR);//addr为从站地址
                   ret = modbus_connect(m_pModbus);//建立连接
                   ui->btOpen->setStyleSheet("background-color: white;");//按钮的背景是红色
//                   qDebug()<<ret;
               }

           else
           {
               QMessageBox::critical( NULL, "连接Modbus设备失败",
                                            "请打开Modbus串口！",
                                            QMessageBox::Yes,
                                            QMessageBox::Yes);
           }

       if(ret==-1)
        {
               QMessageBox::critical( NULL, "串口被占用！",
                                 "串口被占用！",
                                 QMessageBox::Yes,
                                 QMessageBox::Yes);
        }
  }


void MainWindow::f_closeserial()//关闭串口
{
   ui->btOpen->setStyleSheet("QPushButton{background-color: skyblue;");

   if(m_pModbus != NULL)
      {
          modbus_close(m_pModbus);
          modbus_free(m_pModbus);
      }
}

// 故障模拟               按下向从机输入相应的的电流值
void MainWindow::on_btfaultsimulation_clicked()
{
       unsigned int    regStartAddr = 0;
       unsigned int    regOffset = 0;
//     unsigned int    regCount = 0;
//     unsigned int    regNum = 0;
//     unsigned int*   pRegData = NULL;
       uint32_t        regData[1024];
//     QString         appDebugStr;

    if (m_pModbus != NULL)
   {

           regStartAddr = 40001;
           regOffset =  0;
           regData[0] = 100;
//         regCount = 0x01;
//         regNum = 0x02;

           int row = ui->tab_FaultDisplay->rowCount();//表格的行数
           for(int i=0;i<row;i++)
           {
               ui->tab_FaultDisplay->setItem(i,2,new QTableWidgetItem("短路(永久)"));
               ui->tab_FaultDisplay->setItem(i,3,new QTableWidgetItem(QIcon("://image/delete1.bmp"),"故障"));
               ui->tab_FaultDisplay->setItem(i,4,new QTableWidgetItem("0"));
               sleep(1000);

           }

           modbus_write_register(m_pModbus, (int) regStartAddr, regData[0]);
//           modbus_write_registers(m_pModbus, (int)regStartAddr, regNum, (uint16_t *)regData);//写多个寄存器
           sleep(1000);
           regData[0] = 50;
           modbus_write_register(m_pModbus, (int) regStartAddr, regData[0]);
           sleep(1000);
           regData[0] = 0;
           modbus_write_register(m_pModbus, (int) regStartAddr, regData[0]);




//
               }

}

//上电恢复
void MainWindow::on_btpowerrecovery_clicked()
{
    unsigned int    regStartAddr = 0;
//       unsigned int    lastRegStartAddr = 0;
    unsigned int    regOffset = 0;

//       unsigned int*   pRegData = NULL;
    uint16_t        regData[1024];


    int row = ui->tab_FaultDisplay->rowCount();
    for(int i=0;i<row;i++)
    {
        ui->tab_FaultDisplay->setItem(i,2,new QTableWidgetItem(""));
        ui->tab_FaultDisplay->setItem(i,3,new QTableWidgetItem(QIcon("://image/322.bmp"),"正常"));
        ui->tab_FaultDisplay->setItem(i,4,new QTableWidgetItem("20"));

    }

 if (m_pModbus != NULL)
{
     regStartAddr = 40001;
     regOffset =  0;
     regData[0] = 20;
     modbus_write_register(m_pModbus, (int) regStartAddr, regData[0]);
}
}
////----------------------------监听

//void MainWindow::stopthread()
//{
//    //停止线程
//    threadA->quit();
//    //回收子进程的资源
//    threadA->wait();
//    qDebug()<<"线程结束";
//}
