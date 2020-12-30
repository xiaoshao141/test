#include "serialport.h"
#include "ui_serialport.h"

serialport::serialport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialport)
{
    ui->setupUi(this);
//在下拉列表中设置波特率
    ui->comboBox_btl->addItem(("9600"),   QSerialPort::Baud9600);
    ui->comboBox_btl->addItem(("19200"),  QSerialPort::Baud19200);
    ui->comboBox_btl->addItem(("38400"),  QSerialPort::Baud38400);
    ui->comboBox_btl->addItem(("115200"), QSerialPort::Baud115200);
    ui->comboBox_btl->setCurrentIndex(3);
//设置数据位数
    ui->comboBox_data->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_data->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_data->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_data->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_data->setCurrentIndex(3);
//设置奇偶校验
    ui->comboBox_xiaoy->addItem(QString::fromUtf8("无"),    QSerialPort::NoParity);
    ui->comboBox_xiaoy->addItem(QString::fromUtf8("偶校验"), QSerialPort::EvenParity);
    ui->comboBox_xiaoy->addItem(QString::fromUtf8("奇校验"), QSerialPort::OddParity);
//设置停止位
    ui->comboBox_stop->addItem(("1"),   QSerialPort::OneStop);
    ui->comboBox_stop->addItem(("1.5"), QSerialPort::OneAndHalfStop);
    ui->comboBox_stop->addItem(("2"),   QSerialPort::TwoStop);
//设置流控制
    ui->comboBox_flow->addItem(QString::fromUtf8("无"),       QSerialPort::NoFlowControl);
    ui->comboBox_flow->addItem(QString::fromUtf8("RTS/CTS"),  QSerialPort::HardwareControl);
    ui->comboBox_flow->addItem(QString::fromUtf8("XON/XOFF"), QSerialPort::SoftwareControl);

    updateSettings();

}
serialport::~serialport()
{
    delete ui;
}

void serialport::updateSettings()
{
//结构体
    settings.baudRate = ui->comboBox_btl->currentText().toInt();
    settings.stringBaudRate = ui->comboBox_btl->currentText();

    settings.dataBits = static_cast<QSerialPort::DataBits> (ui->comboBox_data->currentText().toInt());
    settings.stringDataBits = ui->comboBox_data->currentText();

    settings.parity = static_cast<QSerialPort::Parity> (ui->comboBox_xiaoy->itemData(ui->comboBox_xiaoy->currentIndex()).toInt());
    settings.stringParity = ui->comboBox_xiaoy->currentText();

    settings.stopBits = static_cast<QSerialPort::StopBits> (ui->comboBox_stop->itemData(ui->comboBox_stop->currentIndex()).toInt());
    settings.stringStopBits = ui->comboBox_stop->currentText();

    settings.flowControl = static_cast<QSerialPort::FlowControl> (ui->comboBox_flow->itemData(ui->comboBox_flow->currentIndex()).toInt());
    settings.stringFlowControl = ui->comboBox_flow->currentText();
}
void serialport::accept()
{
    updateSettings();

    QDialog::accept();//隐藏模式对话框并将结果代码设置为“接受”。
}

