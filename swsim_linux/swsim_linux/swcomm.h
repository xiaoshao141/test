#ifndef SWCOMM_H
#define SWCOMM_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>

#define RTCBIN2BCD(val) ((((val)/10)<<4) + ((val)%10))

class swcomm : public QObject
{
public:
    swcomm();
    ~swcomm();

public:
    void connectTcp();

    void disconnectTcp();

    int autoReport();

    void getUintVal(quint32 val, quint8 len, quint8 *pVal);

    void genCRC7(quint8 *pSrc, uint len);

    void sndData();

    void setDZId(QString dzId);

    void setFlow(float flow);

    void setLevel(float level);

    void setFlowRate(float flowRate);

    void setGatage(float gatage);

    void setVoltage(float voltage);

private:
    QString m_DZId;

    float m_Flow;

    float m_Level;

    float m_FlowRate;

    float m_Gatage;

    float m_Voltage;

    QTcpSocket* m_pSkt;

    quint8 m_DZAddr[5];

    quint32 m_LastReportTime;

    quint32 m_RandReportTime;

public slots:
    void tcpRsved();
};

#endif // SWCOMM_H
