#include "dialog.h"
#include "ui_dialog.h"

#include <QDateTime>
#include <QSqlDatabase>

#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->setWindowTitle("添加电站");

    connect(ui->btnAddDZ, SIGNAL(clicked()), this, SLOT(btnAddDZInfo()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::btnAddDZInfo()
{
    unsigned int index_id = 0;

    index_id = g_pSwData->getDZNum();

    QString dzName = ui->dzName->text();

    QString dzId = ui->dzId->text();

    float flow = ui->flow->text().toFloat();

    float level = ui->level->text().toFloat();

    float flowRate = ui->flow_rate->text().toFloat();

    float gatage = ui->gatage->text().toFloat();

    float voltage = ui->voltage->text().toFloat();

    float min_flow = ui->min_flow->text().toFloat();

    g_pSwData->addDZInfo(index_id, dzName, dzId,
                         flow, level, flowRate,
                         gatage, min_flow, voltage);
}
