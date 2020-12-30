#ifndef SWDATA_H
#define SWDATA_H

#include <QString>
#include <QSqlDatabase>

class swdata
{
public:
    swdata();
    ~swdata();

    unsigned int getDZNum(void);

    QString getDZName(unsigned int index_id);

    QString getDZId(unsigned int index_id);

    float getFlow(unsigned int index_id);

    float getLevel(unsigned int index_id);

    float getFlowRate(unsigned int index_id);

    float getGatage(unsigned int index_id);

    float getMinFlow(unsigned int index_id);

    float getVoltage(unsigned int index_id);

    void addDZInfo(unsigned int index_id, QString DZName, QString DZId,
                   float flow, float level, float flow_rate,
                   float gatage, float min_flow, float voltage);

    void delDZInfo(unsigned int index_id);

private:
    QSqlDatabase m_DataBase;

};

#endif // SWDATA_H
