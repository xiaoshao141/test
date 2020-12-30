#include "addequipment.h"
#include "ui_addequipment.h"

addequipment::addequipment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addequipment)
{
    ui->setupUi(this);
    addr = 0;
    isOk = false;
    ui->lineEdit->setText("");
    connect(ui->YesButton,    SIGNAL(clicked()), this, SLOT(YesButton_clicked()));
    connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(CancelButton_clicked()));
    connect(ui->AddButton,    SIGNAL(clicked()), this, SLOT(AddButton_clicked()));
}

addequipment::~addequipment()
{
    delete ui;
}


void addequipment::on_YesButton_clicked()
{
    addr = ui->lineEdit->text().toUInt(&isOk, 16);

    if (addr > 0xffffff)
    {
        addr = 0;
    }

    isOk = true;
    ui->lineEdit->setText("");
    this->hide();
}

void addequipment::on_CancelButton_clicked()
{
    ui->lineEdit->setText("");
    this->hide();
}

void addequipment::on_AddButton_clicked()
{

    addr = ui->lineEdit->text().toUInt(&isOk, 16);
    if (addr > 0xffffff)
    {
        addr = 0;
    }

    plisOk = true;

    ui->lineEdit->setText("");
    this->hide();
}
