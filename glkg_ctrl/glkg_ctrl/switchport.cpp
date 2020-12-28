#include "switchport.h"
#include "ui_switchport.h"

#include "mainwindow.h"

switchPort::switchPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::switchPort)
{
    ui->setupUi(this);

    init();

    connect(ui->btHeManual1, SIGNAL(clicked()), this, SLOT(btnHeManual1_clicked()));
    connect(ui->btFenManual1, SIGNAL(clicked()), this, SLOT(btnFenManual1_clicked()));
//    connect(ui->btStopManual1, SIGNAL(clicked()), this, SLOT(btnStopManual1_clicked()));
//    connect(ui->btDHeManual1, SIGNAL(clicked()), this, SLOT(btnDHeManual1_clicked()));
//    connect(ui->btDFenManual1, SIGNAL(clicked()), this, SLOT(btnDFenManual1_clicked()));
//    connect(ui->btDStopManual1, SIGNAL(clicked()), this, SLOT(btnDStopManual1_clicked()));

    connect(m_timer,SIGNAL(timeout()), this, SLOT(m_timerSlot()));
    m_timer->start(1000);
}

switchPort::~switchPort()
{
    delete ui;
}

void switchPort::init()
{
    ui->labportTitle->setAlignment(Qt::AlignCenter);

    ui->labferrtingA->setAlignment(Qt::AlignCenter);
    ui->labferrtingB->setAlignment(Qt::AlignCenter);
    ui->labferrtingC->setAlignment(Qt::AlignCenter);

    ui->labportA->setAlignment(Qt::AlignCenter);
    ui->labportB->setAlignment(Qt::AlignCenter);
    ui->labportC->setAlignment(Qt::AlignCenter);

    ui->labelauxiliary->setAlignment(Qt::AlignCenter);
    ui->labZong->setAlignment(Qt::AlignCenter);

//    ui->labZong->setStyleSheet("background:transparent");
//    ui->label_8->setStyleSheet("background:transparent;font-size:21px;"
//                               "color:black;"
//                               "font-family:Arial");
//    ui->groupBox_4->setStyleSheet("background:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 0.2,"
//                                  "stop: 0 #BBDCE8,stop: 0.3 #5A93FC,stop: 1 #4A7BD2)");
}

void switchPort::btnFenManual1_clicked()
{
    g_pMainWindow->handSwStopClear();
    g_pMainWindow->handSwOpenClear();
    g_pMainWindow->handSwClose();
}

void switchPort::btnHeManual1_clicked()
{
    g_pMainWindow->handSwCloseClear();
    g_pMainWindow->handSwStopClear();
    g_pMainWindow->handSwOpen();
}

//void switchPort::btnStopManual1_clicked()
//{
//    g_pMainWindow->handSwOpenClear();
//    g_pMainWindow->handSwCloseClear();
//    g_pMainWindow->handSwStop();
//}

//void switchPort::btnDHeManual1_clicked()
//{
//    g_pMainWindow->handGswStopClear();
//    g_pMainWindow->handGswOpenClear();
//    g_pMainWindow->handGswClose();
//}

//void switchPort::btnDFenManual1_clicked()
//{
//    g_pMainWindow->handGswCloseClear();
//    g_pMainWindow->handGswStopClear();
//    g_pMainWindow->handGswOpen();
//}

//void switchPort::btnDStopManual1_clicked()
//{
//    g_pMainWindow->handGswOpenClear();
//    g_pMainWindow->handGswCloseClear();
//    g_pMainWindow->handGswStop();
//}

void switchPort::m_timerSlot()
{
    //更新八个状态
    update_ztState();
}

void switchPort::update_ztState()
{
    if(g_ztAState)
        ui->labferrtingA->setPixmap(QPixmap("://stataPhotoes/A相合闸.png"));
    else
        ui->labferrtingA->setPixmap(QPixmap("://stataPhotoes/A相分闸.png"));

    if(g_ztBState)
        ui->labferrtingB->setPixmap(QPixmap("://stataPhotoes/B相合闸.png"));
    else
        ui->labferrtingB->setPixmap(QPixmap("://stataPhotoes/B相分闸.png"));

    if(g_ztCState)
        ui->labferrtingC->setPixmap(QPixmap("://stataPhotoes/C相合闸.png"));
    else
        ui->labferrtingC->setPixmap(QPixmap("://stataPhotoes/C相分闸.png"));

    if(g_ztPortAState)
        ui->labportA->setPixmap(QPixmap("://stataPhotoes/A相合闸.png"));
    else
        ui->labportA->setPixmap(QPixmap("://stataPhotoes/A相分闸.png"));

    if(g_ztPortBState)
        ui->labportB->setPixmap(QPixmap("://stataPhotoes/B相合闸.png"));
    else
        ui->labportB->setPixmap(QPixmap("://stataPhotoes/B相分闸.png"));

    if(g_ztPortCState)
        ui->labportC->setPixmap(QPixmap("://stataPhotoes/C相合闸.png"));
    else
        ui->labportC->setPixmap(QPixmap("://stataPhotoes/C相分闸.png"));

    if(g_ztHelpState)
        ui->labelauxiliary->setPixmap((QPixmap("://stataPhotoes/辅助开关合闸.png")));
    else
        ui->labelauxiliary->setPixmap((QPixmap("://stataPhotoes/辅助开关分闸.png")));

    if(g_ztTotalState)
        ui->labZong->setPixmap(QPixmap("://stataPhotoes/主断口合闸2.png"));
    else
        ui->labZong->setPixmap(QPixmap("://stataPhotoes/主断口分闸2.png"));
}
