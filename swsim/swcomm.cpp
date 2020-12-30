#include "swcomm.h"

#include <QDateTime>

swcomm::swcomm()
{
    m_pSkt = new QTcpSocket(this);

    m_LastReportTime = QDateTime::currentDateTime().toTime_t();

    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    m_RandReportTime = qrand() % (30 * 60) + 30 * 60;

    connect(m_pSkt, SIGNAL(readyRead()), this, SLOT(tcpRsved()));
}

swcomm::~swcomm()
{
    delete m_pSkt;
}

void swcomm::connectTcp(void)
{
    if (!m_pSkt) return;

    m_pSkt->close();
    m_pSkt->waitForDisconnected(2000);
    m_pSkt->connectToHost("61.187.56.152", 8386);
    m_pSkt->waitForConnected(2000);
}

void swcomm::disconnectTcp(void)
{
    if (!m_pSkt) return;

    if (m_pSkt->state() == QAbstractSocket::ConnectedState)
    {
        m_pSkt->close();
        m_pSkt->waitForDisconnected(2000);
    }
}

int swcomm::autoReport()
{
    int ret = -1;
    quint32 nowTime = 0;

    nowTime = QDateTime::currentDateTime().toTime_t();

    if(nowTime - m_LastReportTime >= m_RandReportTime)
    {
        sndData();
        ret = 1;
    }

    return ret;
}

void swcomm::getUintVal(quint32 val, quint8 len, quint8 *pVal)
{
    quint8 i = 0;
    while (len && val)
    {
        pVal[i++] = RTCBIN2BCD(val%100);
        val /= 100;
        len--;
    }
}

void swcomm::genCRC7(quint8* pSrc, uint len)
{
    quint8 i = 0;
    quint8 crc = 0;

    uint m = 0;

    for (m = 0; m < len; m++)
    {
        crc ^= pSrc[m];
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^0xE5;
            else
                crc <<= 1;
        }
    }

    pSrc[m] = crc;
}

void swcomm::tcpRsved(void)
{
    QByteArray ba = m_pSkt->readAll();

    if (ba.size() < 7) return;

    quint8* rx = (quint8*) ba.data();

    if (rx[0] != 0x68 || rx[2] != 0x68) return;

    if (rx[9] == 0x02 && rx[10] == 0xF0)
    {
        QByteArray snd(13, 0);
        quint8* tx = (quint8*) snd.data();
        tx[0] = tx[2] = 0x68;
        tx[1] = 8;
        tx[3] = 0xb0;

        QString str = m_DZId;
        tx[4] = str.mid(0, 2).toInt(0, 16);
        tx[5] = str.mid(2, 2).toInt(0, 16);
        tx[6] = str.mid(4, 2).toInt(0, 16);

        quint16 l2 = str.mid(6, 4).toInt();

        tx[7] = l2&0xFF;
        tx[8] = l2>>8;

        tx[9] = 0x02;
        tx[10] = 0xf0;
        tx[12] = 0x16;

        genCRC7(tx+3, 8);
        m_pSkt->write(snd);
        m_pSkt->waitForBytesWritten(1000);
    }

    if (rx[9] == 0x02 && rx[10] == 0xF2)
    {
        QByteArray snd(13, 0);
        quint8* tx = (quint8*) snd.data();

        memcpy(tx, rx, 13);
        tx[3] = 0xb0;
        genCRC7(tx+3, 8);
        m_pSkt->write(snd);
        m_pSkt->waitForBytesWritten(1000);
    }

    if (rx[9] == 0x11)
    {
        QByteArray snd(25, 0);
        quint8* tx = (quint8*) snd.data();

        memcpy(tx, rx, 25);
        tx[3] = 0xb0;
        genCRC7(tx+3, 20);
        m_pSkt->write(snd);
        m_pSkt->waitForBytesWritten(1000);
    }

    if (rx[9] == 0xC0)
    {
        return;
    }
}

void swcomm::sndData()
{
    if (m_pSkt->state() != QAbstractSocket::ConnectedState)
    {
        disconnectTcp();
        connectTcp();
    }

    QString str = m_DZId;

    m_DZAddr[0] = str.mid(0, 2).toInt(0, 16);
    m_DZAddr[1] = str.mid(2, 2).toInt(0, 16);
    m_DZAddr[2] = str.mid(4, 2).toInt(0, 16);

    quint16 l2 = str.mid(6, 4).toInt();

    m_DZAddr[3] = l2&0xFF;
    m_DZAddr[4] = l2>>8;

    QDateTime dt = QDateTime::currentDateTime();

    quint8 dtm[4];
    dtm[0] = RTCBIN2BCD(dt.time().msec()/1000);
    dtm[1] = RTCBIN2BCD(dt.time().minute());
    dtm[2] = RTCBIN2BCD(dt.time().hour());
    dtm[3] = RTCBIN2BCD(dt.date().day());


    QByteArray snd(25, 0);

    quint8* tx = (quint8*) snd.data();

    tx[0] = tx[2] = 0x68;
    tx[1] = 20;
    tx[3] = 0xb2;
    tx[24] = 0x16;
    tx[9] = 0xC0;

    memcpy(tx+4, m_DZAddr, 5);
    memcpy(tx+18, dtm, 4);

    quint32 val = m_Level*1000;

    getUintVal(val, 4, tx+10);
    genCRC7(tx+3, 0x14);

    m_pSkt->write(snd);
    m_pSkt->waitForBytesWritten(1000);

    //b3
    snd.resize(26);
    tx = (quint8*) snd.data();
    tx[0] = tx[2] = 0x68;
    tx[1] = 21;
    tx[3] = 0xb3;
    tx[25] = 0x16;
    tx[9] = 0xC0;

    memcpy(tx+4, m_DZAddr, 5);
    memcpy(tx+19, dtm, 4);

    val = m_Flow*1000;
    getUintVal(val, 5, tx+10);
    genCRC7(tx+3, 0x15);

    m_pSkt->write(snd);
    m_pSkt->waitForBytesWritten(1000);

    //b4
    snd.resize(24);
    tx = (quint8*) snd.data();
    tx[0] = tx[2] = 0x68;
    tx[1] = 19;
    tx[3] = 0xb4;
    tx[23] = 0x16;
    tx[9] = 0xC0;

    memcpy(tx+4, m_DZAddr, 5);
    memcpy(tx+17, dtm, 4);

    val = m_Voltage*1000;
    getUintVal(val, 3, tx+10);
    genCRC7(tx+3, 0x13);

    m_pSkt->write(snd);
    m_pSkt->waitForBytesWritten(1000);

    //b5
    snd.resize(24);
    tx = (quint8*) snd.data();
    tx[0] = tx[2] = 0x68;
    tx[1] = 19;
    tx[3] = 0xb5;
    tx[23] = 0x16;
    tx[9] = 0xC0;

    memcpy(tx+4, m_DZAddr, 5);
    memcpy(tx+17, dtm, 4);

    val = m_Gatage*100;
    getUintVal(val, 3, tx+10);
    genCRC7(tx+3, 0x13);

    m_pSkt->write(snd);
    m_pSkt->waitForBytesWritten(1000);

    //bd
    snd.resize(24);
    tx = (quint8*) snd.data();
    tx[0] = tx[2] = 0x68;
    tx[1] = 18;
    tx[3] = 0xbd;
    tx[22] = 0x16;
    tx[9] = 0xC0;

    memcpy(tx+4, m_DZAddr, 5);
    memcpy(tx+16, dtm, 4);

    val = m_FlowRate*100;
    getUintVal(val, 2, tx+10);
    genCRC7(tx+3, 0x12);

    m_pSkt->write(snd);
    m_pSkt->waitForBytesWritten(1000);

    QDateTime time = QDateTime::currentDateTime();
    m_LastReportTime = (quint32)time.toTime_t();
}

void swcomm::setDZId(QString dzId)
{
    m_DZId = dzId;
}

void swcomm::setFlow(float flow)
{
    m_Flow = flow;
}

void swcomm::setLevel(float level)
{
    m_Level = level;
}

void swcomm::setFlowRate(float flowRate)
{
    m_FlowRate = flowRate;
}

void swcomm::setGatage(float gatage)
{
    m_Gatage = gatage;
}

void swcomm::setVoltage(float voltage)
{
    m_Voltage = voltage;
}
