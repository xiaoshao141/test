#include "switchangels.h"
#include "ui_switchangels.h"

#include "mainwindow.h"

switchangels::switchangels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::switchangels)
{
    ui->setupUi(this);

    init();

    connect(ui->btHeManual2, SIGNAL(clicked()), this, SLOT(btnHeManual2_clicked()));
    connect(ui->btFenManual2, SIGNAL(clicked()), this, SLOT(btnFenManual2_clicked()));
//    connect(ui->btStopManual2, SIGNAL(clicked()), this, SLOT(btnStopManual2_clicked()));
//    connect(ui->btDHeManual2, SIGNAL(clicked()), this, SLOT(btnDHeManual2_clicked()));
//    connect(ui->btDFenManual2, SIGNAL(clicked()), this, SLOT(btnDFenManual2_clicked()));
//    connect(ui->btDStopManual2, SIGNAL(clicked()), this, SLOT(btnDStopManual2_clicked()));

    connect(m_timer,SIGNAL(timeout()), this, SLOT(m_timerSlot()));
    m_timer->start(1000);
}

switchangels::~switchangels()
{
    delete ui;
}

void switchangels::init()
{
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
//                                  "stop: 0 #BBDCE8,stop: 0.3 #5A93FC,stop: 1 #4A7BD2);");
}

void switchangels::btnFenManual2_clicked()
{
    g_pMainWindow->handSwStopClear();
    g_pMainWindow->handSwOpenClear();
    g_pMainWindow->handSwClose();
}

void switchangels::btnHeManual2_clicked()
{
    g_pMainWindow->handSwCloseClear();
    g_pMainWindow->handSwStopClear();
    g_pMainWindow->handSwOpen();
}

//void switchangels::btnStopManual2_clicked()
//{
//    g_pMainWindow->handSwOpenClear();
//    g_pMainWindow->handSwCloseClear();
//    g_pMainWindow->handSwStop();
//}

//void switchangels::btnDHeManual2_clicked()
//{
//    g_pMainWindow->handGswStopClear();
//    g_pMainWindow->handGswOpenClear();
//    g_pMainWindow->handGswClose();
//}

//void switchangels::btnDFenManual2_clicked()
//{
//    g_pMainWindow->handGswCloseClear();
//    g_pMainWindow->handGswStopClear();
//    g_pMainWindow->handGswOpen();
//}

//void switchangels::btnDStopManual2_clicked()
//{
//    g_pMainWindow->handGswOpenClear();
//    g_pMainWindow->handGswCloseClear();
//    g_pMainWindow->handGswStop();
//}

void switchangels::m_timerSlot()
{
    //更新八个状态
    update_wdState();
}

void switchangels::update_wdState()
{
    if(g_wdAState)
        ui->labferrtingA->setPixmap(QPixmap("://stataPhotoes/A相合闸.png"));
    else
        ui->labferrtingA->setPixmap(QPixmap("://stataPhotoes/A相分闸.png"));

    if(g_wdBState)
        ui->labferrtingB->setPixmap(QPixmap("://stataPhotoes/B相合闸.png"));
    else
        ui->labferrtingB->setPixmap(QPixmap("://stataPhotoes/B相分闸.png"));

    if(g_wdCState)
        ui->labferrtingC->setPixmap(QPixmap("://stataPhotoes/C相合闸.png"));
    else
        ui->labferrtingC->setPixmap(QPixmap("://stataPhotoes/C相分闸.png"));

    if(g_wdPortAState)
        ui->labportA->setPixmap(QPixmap("://stataPhotoes/A相合闸.png"));

    else
        ui->labportA->setPixmap(QPixmap("://stataPhotoes/A相分闸.png"));

    if(g_wdPortBState)
        ui->labportB->setPixmap(QPixmap("://stataPhotoes/B相合闸.png"));
    else
        ui->labportB->setPixmap(QPixmap("://stataPhotoes/B相分闸.png"));

    if(g_wdPortCState)
        ui->labportC->setPixmap(QPixmap("://stataPhotoes/C相合闸.png"));
    else
        ui->labportC->setPixmap(QPixmap("://stataPhotoes/C相分闸.png"));

    if(g_wdHelpState)
        ui->labelauxiliary->setPixmap(QPixmap("://stataPhotoes/辅助开关合闸.png"));
    else
        ui->labelauxiliary->setPixmap(QPixmap("://stataPhotoes/辅助开关分闸.png"));

    if(g_wdTotalState)
        ui->labZong->setPixmap(QPixmap("://stataPhotoes/主断口合闸2.png"));
    else
        ui->labZong->setPixmap(QPixmap("://stataPhotoes/主断口分闸2.png"));
}
