#include "swdata.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QApplication>

swdata::swdata()
{
    m_DataBase = QSqlDatabase::addDatabase("QSQLITE", "swDataDb");
    m_DataBase.setDatabaseName("swdb/swdata.db3");
    if (!m_DataBase.open())
        qDebug() << "Error: Failed to connect database." << m_DataBase.lastError();
    else
        qDebug() << "Succeed to connect database." ;

    /* @brief MySql数据库
        m_DataBase = QSqlDatabase::addDatabase("QMYSQL");
        m_DataBase.setHostName("localhost");
        m_DataBase.setUserName("root");
        m_DataBase.setPassword("cgsr");
        m_DataBase.setDatabaseName("swdata");
        m_DataBase.open();
    */
}

swdata::~swdata()
{

}

unsigned int swdata::getDZNum()
{
    unsigned int dz_num = 0;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select * from swdata");
    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    dz_num = 0;
    while (query.next())
    {
        dz_num++;
    }

    return dz_num;
}

QString swdata::getDZName(unsigned int index_id)
{
    QString dzName;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select dzname from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        dzName = query.value(0).toString();

    return dzName;
}

QString swdata::getDZId(unsigned int index_id)
{
    QString dzId;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select dzid from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        dzId = query.value(0).toString();

    return dzId;
}

float swdata::getFlow(unsigned int index_id)
{
    float flow;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select flow from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        flow = query.value(0).toFloat();

    return flow;
}

float swdata::getLevel(unsigned int index_id)
{
    float level;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select level from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        level = query.value(0).toFloat();

    return level;
}

float swdata::getFlowRate(unsigned int index_id)
{
    float flow_rate;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select flow_rate from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        flow_rate = query.value(0).toFloat();

    return flow_rate;
}

float swdata::getGatage(unsigned int index_id)
{
    float gatage;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select gatage from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    gatage = query.value(0).toFloat();

    return gatage;
}

float swdata::getMinFlow(unsigned int index_id)
{
    float min_flow;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select min_flow from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        min_flow = query.value(0).toFloat();

    return min_flow;
}

float swdata::getVoltage(unsigned int index_id)
{
    float voltage;
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("select voltage from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    if(query.first())
        voltage = query.value(0).toFloat();

    return voltage;
}

void swdata::delDZInfo(unsigned int index_id)
{
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("delete from swdata where id==%d", index_id);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }

    for(unsigned int next_id=index_id; next_id<=getDZNum(); next_id++)
    {
        sqlStr.sprintf("update swdata set id=%d where id==%d", next_id-1, next_id);
        query.exec(sqlStr);
    }
}

void swdata::addDZInfo(unsigned int index_id, QString DZName, QString DZId,
                       float flow, float level, float flow_rate,
                       float gatage, float min_flow, float voltage)
{
    QSqlQuery query(m_DataBase);
    QString sqlStr;

    sqlStr.sprintf("insert into swdata (id, dzname, dzid, flow, level, flow_rate, gatage, min_flow, voltage) values (%d, \'%s\', \'%s\', %f, %f, %f, %f, %f, %f)",
                   index_id, DZName.toUtf8().data(), DZId.toLatin1().data(),
                   flow, level, flow_rate, gatage, min_flow, voltage);

    if (!query.exec(sqlStr))
    {
        qDebug() << query.lastError();
    }
}
