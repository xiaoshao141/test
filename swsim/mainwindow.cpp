#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"

swcomm* g_pSwComm[512];

swdata* g_pSwData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnAddDZ, SIGNAL(clicked()), this, SLOT(btnAddDZ()));
    connect(ui->btnDelDZ, SIGNAL(clicked()), this, SLOT(btnDelDZ()));
    connect(ui->btnRefresh, SIGNAL(clicked()), this, SLOT(btnRefreshDZ()));
    connect(ui->btnRunCtrl, SIGNAL(clicked()), this, SLOT(btnRunCtrl()));

    g_pSwData = new swdata();

    m_AddDzDialog = new Dialog();

    m_RunCtrlFlag = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDZTable()
{
    ui->tblDZInfo->setRowCount(g_pSwData->getDZNum());

    for(unsigned int rowCount=0; rowCount<g_pSwData->getDZNum(); rowCount++)
    {
        // 电站名称
        ui->tblDZInfo->setItem(rowCount, 0, new QTableWidgetItem(g_pSwData->getDZName(rowCount)));
        ui->tblDZInfo->item(rowCount, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 0)->setFlags(Qt::ItemIsEnabled);

        // 电站代码
        ui->tblDZInfo->setItem(rowCount, 1, new QTableWidgetItem(g_pSwData->getDZId(rowCount)));
        ui->tblDZInfo->item(rowCount, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 1)->setFlags(Qt::ItemIsEnabled);

        // 流量数据
        ui->tblDZInfo->setItem(rowCount, 2, new QTableWidgetItem(QString::number(g_pSwData->getFlow(rowCount))));
        ui->tblDZInfo->item(rowCount, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 2)->setFlags(Qt::ItemIsEnabled);

        // 水位数据
        ui->tblDZInfo->setItem(rowCount, 3, new QTableWidgetItem(QString::number(g_pSwData->getLevel(rowCount))));
        ui->tblDZInfo->item(rowCount, 3)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 3)->setFlags(Qt::ItemIsEnabled);

        // 流速数据
        ui->tblDZInfo->setItem(rowCount, 4, new QTableWidgetItem(QString::number(g_pSwData->getFlowRate(rowCount))));
        ui->tblDZInfo->item(rowCount, 4)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 4)->setFlags(Qt::ItemIsEnabled);

        // 闸门开度
        ui->tblDZInfo->setItem(rowCount, 5, new QTableWidgetItem(QString::number(g_pSwData->getGatage(rowCount))));
        ui->tblDZInfo->item(rowCount, 5)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 5)->setFlags(Qt::ItemIsEnabled);

        // 最小下泄流量
        ui->tblDZInfo->setItem(rowCount, 6, new QTableWidgetItem(QString::number(g_pSwData->getMinFlow(rowCount))));
        ui->tblDZInfo->item(rowCount, 6)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 6)->setFlags(Qt::ItemIsEnabled);

        // 电池电压
        ui->tblDZInfo->setItem(rowCount, 7, new QTableWidgetItem(QString::number(g_pSwData->getVoltage(rowCount))));
        ui->tblDZInfo->item(rowCount, 7)->setTextAlignment(Qt::AlignCenter);
        ui->tblDZInfo->item(rowCount, 7)->setFlags(Qt::ItemIsEnabled);
    }
}

void MainWindow::btnRefreshDZ()
{
    updateDZTable();
}

void MainWindow::btnAddDZ()
{
    m_AddDzDialog->setParent(NULL);
    m_AddDzDialog->show();

    updateDZTable();
}

void MainWindow::btnDelDZ()
{
    unsigned int currentRow = 0;

    currentRow = ui->tblDZInfo->currentRow();

    g_pSwData->delDZInfo(currentRow);

    updateDZTable();
}

void MainWindow::btnRunCtrl()
{
    if(m_RunCtrlFlag)
    {
        ui->btnRefresh->setEnabled(true);
        ui->btnAddDZ->setEnabled(true);
        ui->btnDelDZ->setEnabled(true);

        ui->btnRunCtrl->setStyleSheet("border-image: url(:/pics/start.jpg)");

        m_RunCtrlFlag = false;

        m_pDevCommThread->requestInterruption();
        m_pDevCommThread->wait();
        m_pDevCommThread->deleteLater();

        for(unsigned int i=0; i<g_pSwData->getDZNum(); i++)
        {
            delete g_pSwComm[i];
        }
    }
    else
    {
        ui->btnRefresh->setEnabled(false);
        ui->btnAddDZ->setEnabled(false);
        ui->btnDelDZ->setEnabled(false);

        ui->btnRunCtrl->setStyleSheet("border-image: url(:/pics/stop.jpg)");

        m_RunCtrlFlag = true;

        m_pDevCommThread = new DevCommThread();
        m_pDevCommThread->start();
    }
}

DevCommThread::DevCommThread(void)
{

}

void DevCommThread::init(void)
{
    for(unsigned int i=0; i<g_pSwData->getDZNum(); i++)
    {
        g_pSwComm[i] = new swcomm();

        g_pSwComm[i]->setDZId(g_pSwData->getDZId(i));

        g_pSwComm[i]->setFlow(g_pSwData->getFlow(i));

        g_pSwComm[i]->setLevel(g_pSwData->getLevel(i));

        g_pSwComm[i]->setFlowRate(g_pSwData->getFlowRate(i));

        g_pSwComm[i]->setGatage(g_pSwData->getGatage(i));

        g_pSwComm[i]->setVoltage(g_pSwData->getVoltage(i));
    }

    for(unsigned int i=0; i<g_pSwData->getDZNum(); i++)
    {
        g_pSwComm[i]->connectTcp();
    }
}

void DevCommThread::run(void)
{
    int ret = -1;

    init();

    while(!isInterruptionRequested())
    {
        for(unsigned int i=0; i<g_pSwData->getDZNum(); i++)
        {
            ret = g_pSwComm[i]->autoReport();
            if(ret >= 0)
            {
                msleep(60 * 1000);
                break;
            }
        }

        msleep(1000);
    }
}
