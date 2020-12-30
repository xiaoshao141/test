#include "modifyaddress.h"
#include "ui_modifyaddress.h"

ModifyAddress::ModifyAddress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyAddress)
{
    ui->setupUi(this);
    addr = 0;
    isOk = false;
    ui->lineEdit->setText("");
    connect(ui->YesButton,    SIGNAL(clicked()), this, SLOT(YesButton_clicked()));
    connect(ui->CancelButton, SIGNAL(clicked()), this, SLOT(CancelButton_clicked()));
}

ModifyAddress::~ModifyAddress()
{
    delete ui;
}

void ModifyAddress::on_YesButton_clicked()
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

void ModifyAddress::on_CancelButton_clicked()
{
    ui->lineEdit->setText("");
    this->hide();
}
