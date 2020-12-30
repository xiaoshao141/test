#ifndef STRUCT_H
#define STRUCT_H


#include <vector>
#include <map>
using namespace std;
//实时状态结构体
typedef struct
{
    int no;
    QString addr;


}REALSTATE_PARAM;

typedef vector<REALSTATE_PARAM> VECT_REALSTATE_PARAM;
typedef map<int, REALSTATE_PARAM> MAP_REALSTATE_PARAM;

//调试地址参数结构体
typedef struct
{
    int no;
    int addr;
    int setParam;//设置参数
    int exeResult;//执行结果
}DEBUGADDR_PARAM;
typedef vector<DEBUGADDR_PARAM> VECT_DEBUGADDR_PARAM;
typedef map<int, DEBUGADDR_PARAM> MAP_DEBUGADDR_PARAM;

//电池参数结构体
typedef struct
{
    int no;
    int addr;
    int LVD_alarm;//低电压告警值
    int cycle_alarm;//告警上报周期
}BATTARY_PARAM;
typedef vector<BATTARY_PARAM>VECT_BATTARY_PARAM;
typedef map<int,BATTARY_PARAM>MAP_BATTARY_PARAM;

//校表参数结构体
typedef struct
{
    int stdValue;//标准值
    int adpValue;//采样值
    int realValue;//实际值
}METER_SINGLE;
typedef struct
{
    int no;
    int addr;
    map<int, METER_SINGLE> meters;//校表点参数设置
}METER_PARAM;
typedef vector<METER_PARAM> VECT_METER_PARAM;
typedef map<int, METER_PARAM> MAP_METER_PARAM;

//电场突变量参数结构体。
typedef struct
{
    int no;
    int addr;
    int eleHoldValue;//电场阈值
    int eleHoldReal;//电场阈值实际值
    int downTime;//下降时间
    int downTimeReal;//下降时间实际值
    int downRatio;//下降比例
    int downRatioReal;//下降比例实际值
    int readParam;//读参数
    int writeParam;//写参数
}ELEFIELD_PARAM;
typedef vector<ELEFIELD_PARAM> VECT_ELEFIELD_PARAM;
typedef map<int, ELEFIELD_PARAM> MAP_ELEFIELD_PARAM;

//接地电流突变量参数结构体。
typedef struct
{
    int no;
    int addr;
    int triMode;//录波触发模式
    int groMinEle;//接地最小电场
    int downRatio;//下降比例
    int downTime;//下降时间
    int groCurBre;//接地电流突变量
}GROUNDCUR_PARAM;
typedef vector<GROUNDCUR_PARAM> VECT_GROUNDCUR_PARAM;
typedef map<int, GROUNDCUR_PARAM> MAP_GROUNDCUR_PARAM;

//短路电流突变量参数结构体。
typedef struct
{
    int no;
    int addr;
    int quickLimit;//速断限值
    int quickDelay;//速断延时
    int overLimit;//过流限值
    int overDelay;//过流延时
    int invoParam;//复归参数
}SHORTCUR_PARAM;
typedef vector<SHORTCUR_PARAM> VECT_SHORTCUR_PARAM;
typedef map<int, SHORTCUR_PARAM> MAP_SHORTCUR_PARAM;

//节点信息参数结构体。
typedef struct
{
    int no;
    int addr;
    int neiNodeOne;//邻节点1
    int neiNodeTwo;//邻节点2
    int parNode;//父节点
    int nodeNowRelay;//节点当前中继
    int subNode;//所在支路节点
    int phase;//相位
}NODEINFO_PARAM;
typedef vector<NODEINFO_PARAM> VECT_NODEINFO_PARAM;
typedef map<int, NODEINFO_PARAM> MAP_NODEINFO_PARAM;
//报文保存结构体
typedef struct
{
    QString baowen;
}BWNO_PARAM;
//typedef map<int, BWNO_PARAM> MAP_BWNO_PARAM;

#endif // STRUCT_H
