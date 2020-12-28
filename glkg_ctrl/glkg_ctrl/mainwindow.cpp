#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowStyle();

    Modbus_RTU();

    g_wdAState = false;
    g_wdBState = false;
    g_wdCState = false;
    g_wdPortAState = false;
    g_wdPortBState = false;
    g_wdPortCState = false;
    g_wdHelpState = false;
    g_wdTotalState = false;

    g_ztAState = false;
    g_ztBState = false;
    g_ztCState = false;
    g_ztPortAState = false;
    g_ztPortBState = false;
    g_ztPortCState = false;
    g_ztHelpState = false;
    g_ztTotalState = false;

    connect(ui->btswitchPort, SIGNAL(clicked()), this, SLOT(on_btswitchPort_clicked()));
    connect(ui->btswitchAngel, SIGNAL(clicked()), this, SLOT(on_btswitchAngel_clicked()));

    connect(m_timer,SIGNAL(timeout()),this,SLOT(on_m_timer_start()));
    m_timer->start(1000);
}

MainWindow::~MainWindow()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
    delete ui;
}

void MainWindow::on_m_timer_start()
{
    readModbus();

    if(g_ztAState && g_ztBState && g_ztCState && \
       g_ztPortAState && g_ztPortBState && g_ztPortCState && \
       g_ztHelpState)
       g_ztTotalState = true;
    else if(!g_ztAState && !g_ztBState && !g_ztCState && \
            !g_ztPortAState && !g_ztPortBState && !g_ztPortCState && \
            !g_ztHelpState)
        g_ztTotalState = false;

    if(g_wdAState && g_wdBState && g_wdCState && \
       g_wdPortAState && g_wdPortBState && g_wdPortCState && \
       g_wdHelpState)
        g_wdTotalState = true;
    else if(!g_wdAState && !g_wdBState && !g_wdCState && \
            !g_wdPortAState && !g_wdPortBState && !g_wdPortCState && \
            !g_wdHelpState)
        g_wdTotalState = false;
}

void MainWindow::setWindowStyle()
{
    QFile file("://qss/QtStyle1.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);
    file.close();
}
void MainWindow::Modbus_RTU()
{
    modbusDevice = new QModbusRtuSerialMaster(this);
    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if (!modbusDevice) {
        statusBar()->showMessage(tr("Could not create Modbus master."), 5000);
        return;
    }

    QString portname;
    portname = MODBUS_PORT_NAME;
    statusBar()->clearMessage();
    if (modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                             portname );
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                             QSerialPort::NoParity);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                             QSerialPort::Baud9600);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                             QSerialPort::Data8);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                             QSerialPort::OneStop);

        modbusDevice->setTimeout(1000);

        statusBar()->showMessage(tr("Connect Success "));

        if (!modbusDevice->connectDevice()) {
            statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        }
    }
    else {
        modbusDevice->disconnectDevice();
    }
}

void MainWindow::handSwOpen()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 0, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handSwClose()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 1, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handSwStop()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 2, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handGswOpen()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 3, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handGswClose()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 4, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handGswStop()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 5, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handSwOpenClear()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 0, 1);

    writeUnit.setValue(0, RELAY_OFF);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handSwCloseClear()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 1, 1);

    writeUnit.setValue(0, RELAY_OFF);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handSwStopClear()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 2, 1);

    writeUnit.setValue(0, RELAY_OFF);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handGswCloseClear()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 4, 1);

    writeUnit.setValue(0, RELAY_OFF);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handGswStopClear()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 5, 1);

    writeUnit.setValue(0, RELAY_OFF);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::handGswOpenClear()
{
    if (!modbusDevice)
        return;
    statusBar()->clearMessage();

    QModbusDataUnit writeUnit = QModbusDataUnit(QModbusDataUnit::Coils, 3, 1);

    writeUnit.setValue(0, RELAY_ON);
    auto *reply = modbusDevice->sendWriteRequest(writeUnit, MODBUS_DEV1_ADDR);
    if(!reply->isFinished())
    {
        connect(reply, &QModbusReply::finished, this, [this, reply]() {
            if(reply->isFinished())
                if (reply->error() == QModbusDevice::ProtocolError) {
                    statusBar()->showMessage(tr("Write response error: %1 (Mobus exception: 0x%2)")
                                             .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                                             5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    statusBar()->showMessage(tr("Write response error: %1 (code: 0x%2)").
                                             arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
            reply->deleteLater();
        });
    }
}

void MainWindow::dev1DataProc()
{
    bool tempZtFen = false;
    bool tempZtHe = false;

    bool tempWdFen = false;
    bool tempWdHe = false;

    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
         const QModbusDataUnit readUnit_reply = reply->result();

         /*@brief 姿态开关信号 */
         tempZtHe = readUnit_reply.value(0);
         tempZtFen = readUnit_reply.value(1);
         if(tempZtFen && !tempZtHe)
         {
             g_ztAState = false;
             g_ztBState = false;
             g_ztCState = false;
         }
         else if(tempZtHe && !tempZtFen)
         {
             g_ztAState = true;
             g_ztBState = true;
             g_ztCState = true;
         }

         /*@brief 微动开关信号 */
         tempWdFen = readUnit_reply.value(2);
         tempWdHe = readUnit_reply.value(3);
         if(tempWdFen && !tempWdHe)
             g_wdAState = false;
         else if(!tempWdFen && tempWdHe)
             g_wdAState = true;

         tempWdFen = readUnit_reply.value(4);
         tempWdHe = readUnit_reply.value(5);
         if(tempWdFen && !tempWdHe)
             g_wdBState = false;
         else if(!tempWdFen && tempWdHe)
             g_wdBState = true;

         tempWdFen = readUnit_reply.value(6);
         tempWdHe = readUnit_reply.value(7);
         if(tempWdFen && !tempWdHe)
             g_wdCState = false;
         else if(!tempWdFen && tempWdHe)
             g_wdCState = true;

         g_wdAState = g_wdCState;
         g_wdBState = g_wdCState;
    }

    reply->deleteLater();
}

void MainWindow::dev2DataProc()
{
    bool tempFZFen = false;
    bool tempFZHe = false;

    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit readUnit_reply1 = reply->result();

        tempFZFen = readUnit_reply1.value(0);
        tempFZHe = readUnit_reply1.value(1);

        /*@brief 辅助开关信号 */
        if(tempFZHe && !tempFZFen)
        {
            g_wdHelpState = true;
            g_ztHelpState = true;
        }
        else if(!tempFZHe && tempFZFen)
        {
            g_wdHelpState = false;
            g_ztHelpState = false;
        }

        /*@brief 主断口开关信号 */
        g_ztPortAState = readUnit_reply1.value(2);
        g_ztPortBState = readUnit_reply1.value(3);
        g_ztPortCState = readUnit_reply1.value(4);

        g_wdPortAState = readUnit_reply1.value(2);
        g_wdPortBState = readUnit_reply1.value(3);
        g_wdPortCState = readUnit_reply1.value(4);
    }

    reply->deleteLater();
}


void MainWindow::readModbus()
{
    if (!modbusDevice)
            return;
    statusBar()->clearMessage();

    QModbusDataUnit readUnit = QModbusDataUnit(QModbusDataUnit::DiscreteInputs, 0, 0x08);
    auto *reply = modbusDevice->sendReadRequest(readUnit, MODBUS_DEV1_ADDR);
    if(reply != NULL)
    {
        if(!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::dev1DataProc);
        else
            delete reply;
    }

    QModbusDataUnit readUnit1 = QModbusDataUnit(QModbusDataUnit::DiscreteInputs, 0, 0x08);
    auto *reply1 = modbusDevice->sendReadRequest(readUnit1, MODBUS_DEV2_ADDR);
    if(reply1 != NULL)
    {
        if(!reply1->isFinished())
            connect(reply1, &QModbusReply::finished, this, &MainWindow::dev2DataProc);
        else
            delete reply1;
    }
}

void MainWindow::on_btswitchPort_clicked()
{
    portwidget->showMaximized();
}

void MainWindow::on_btswitchAngel_clicked()
{
   angelwidget->showMaximized();
}
