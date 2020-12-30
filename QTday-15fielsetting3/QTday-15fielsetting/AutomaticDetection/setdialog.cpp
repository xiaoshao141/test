#include "setdialog.h"
#include "ui_setdialog.h"

SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    setWindowTitle("系统配置");
}

SetDialog::~SetDialog()
{
    delete ui;
}
