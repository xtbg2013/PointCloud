#pragma once
#include "../Global.h"
#include "pciecommmng/transmsgstruct.h"

#define REMOTE_VISUAL_IP "192.168.0.12"         //综合视景软件IP
#define REMOTE_SERVICE_IP "192.168.0.110"       //平台服务软件IP 
#define REMOTE_VISUAL_PORT  10086               //综合视景软件通信端口号
#define REMOTE_SERVICE_PORT 10010               //平台服务软件通信端口号


#define UDP_PLATFORM_IP "127.0.0.1"
#define UDP_CONFIG_PORT 80000
#define UDP_DATA_PORT   80001
#define LASER_RS422_PORT     0
#define HIGH_SPEED_DELAY 10  //10ms
#define LOW_SPEED_DELAY  100 //100ms

//MSG COMMON 
#define MSG_MAX_DATA_LEN     0xFFFD

#define MSG_HEAD_FIELD_LEN        2
#define MSG_LENGTH_FIELD_LEN      2
#define MSG_TYPE_FIELD_LEN        1
#define MSG_CHECK_SUM_FIELD_LEN   2
#define MSG_CHECK_SUM_FIELD_LEN2  4
 

#define MSG_SIGNAL_CONTROL_LEN    4 //信号处理控制长度
#define MSG_LASER_CONTROL_LEN     4 //激光雷达控制长度
#define MSG_RENDERING_CONTROL_LEN 1 //显示渲染控制长度
#define MSG_CONFIG_RESERVER_LEN   2 //配置备用长度
#define MSG_TIME_LEN              4 //时间长度
#define MSG_SYSTEM_CONTROL_LEN    1 //系统控制长度
#define MSG_TIMESTAMP_LEN         8 //时间戳长度

#define MSG_MAX_TARGET_COUNT      100 //最大目标数量


#define MSG_HEAD        0Xeb90     //消息头  
//消息标识：智能计算平台->接口控制模块
#define MSG_MPIC_TYPE_CONFIG 0X00
#define MSG_MPIC_TYPE_INS    0X03
//消息标识：接口控制模块->智能计算平台
#define MSG_ICMP_TYPE_TARGET_DATA       0X05   //目标识别数据
#define MSG_ICMP_TYPE_TOPOGRAPHIC_DATA  0X09   //地形探测数据
#define MSG_ICMP_TYPE_CMD_RESP          0X0A   //指令响应信息
#define MSG_ICMP_TYPE_RUNING_STATUS     0X30   //运行状态信息
#define MSG_ICMP_TYPE_POINTCLOUD        0X03   //结果级点云数据：接口控制模块->智能计算平台
//消息标识：信号处理板-->接口控制模块
#define MSG_SPIC_TYPE_POINTCLOUD        0X33   //结果级点云数据：信号处理板-->接口控制模块



//点云属性
#define   TARGET_HIGH_TENSION_LINE  0X00  //高压线
#define   TARGET_HIGH_TENSION_LINE_TOWER 0X03  //高压线塔
#define   TARGET_CHIMNEY            0X05  //烟冲
#define   TARGET_BUILDING           0X06  //建筑物
#define   TARGET_OWER_CRANE         0X09  //塔吊
#define   TARGET_BRIDGE             0X0A  //桥梁
#define   TARGET_SIGNAL_TOWER       0X30  //信号塔
#define   TARGET_VEHICLE            0X33  //车辆
#define   TARGET_GROUND             0X35  //地面
#define   TARGET_OTHER              0X36  //其他

#define   MAX_POINTS_PER_FRAME      0X50  //80个点
#define   MAX_MSG_LEN               0X2800//10KB
#define   MAX_POINTS_OF_PCID        512*1024


/*
  惯导消息字段长度 :智能计算平台->接口控制模块
*/
#define INS_WORD_LEN   4


/*
  配置消息 :智能计算平台(MP)->接口控制模块(IC;)
*/
#pragma pack(1)  //按字节对齐


/*
    点云数据：接口控制模块（IC）->智能计算平台（MP）
*/
struct ICMP_PointsData
{
    uint16  serialNo;   //序号
    uint16  allframes;   //点云数据总帧 
    uint64  time;
    float64 longitude;   //经度
    float64 latitude;    //纬度
    float32 navigationAltidute;//导航高度
    float32 rollAngle;   //横滚角
    float32 pitchAngle;  //俯仰角
    float32 headingAngle;//航向角
    uint32  reserve;
    Point_XYZI   points[MAX_POINTS_PER_FRAME];
};




//着落场地形探测数据
struct TopographicInfo
{
    uint64  timsStamp;              //时间戳
    uint32  maxAltitudeIntercept;   //最大高度差
    uint32  maxAltidudeInterceptError;//最大高度差均方根误差
    int32   toprographicAngle;      //地形坡度角度
    int32   toprographicAngleError; //地形坡度均方根误差
    int8    sufaceRoughness;        //地表粗糙度
    int32   reserve;
};

//故障状态
struct ErrorStatus
{
    uint8  signalProcessModule;
    uint8  interfaceControlModule;
    uint8  laserModule;
    uint8  reserve;
};
//指令响应
struct ICMP_Response
{
    uint32 time;
    uint8  systemCtrlResp;      //系統控制响应
    uint8  systemStatus;        //系统状态
    uint8  signalProcessResp;   //信号处理控制响应
    //SignalControl  signalStatus;//信号处理模块状态
    uint8  laserResp;           //激光雷达控制响应
    //LaserControl  laserStatus;  //激光雷达状态
    uint8  displayControlResp;  //显示渲染控制响应
    uint8  displayControlStatus;//显示渲染状态
    ErrorStatus errorStatus;    //故障状态信息
    uint8 reserve;
};
//状态数据
struct ICMP_Status
{
    uint32 time;
    uint8  systemStatus;          //系统状态
    //SignalControl  signalStatus;  //信号处理状态
    //LaserControl  laserStatus;    //激光雷达状态
    uint8  displayControlStatus;  //显示渲染状态
    ErrorStatus errorStatus;      //故障状态
    uint8 reserve;
};

///////////////////////内部接口//////////////////////////////////////
////////////////接口控制模块 - 信号处理模块/////////////////////////////
struct ICSP_Config
{
    uint32 time;
    uint8  systemControl;
    //SignalControl  signalCtrl;
};


//目标识别信息
struct SPIC_TargetInfo
{
    uint64  timsStamp;   //时间戳
    uint16  serialNumber;//序号
    float64 longitude;   //经度
    float64 latitude;    //纬度
    float32 navigationAltidute;//导航高度
    float32 rollAngle;   //横滚角
    float32 pitchAngle;  //俯仰角
    float32 headingAngle;//航向角
    float32 reserve;
    uint8   targetCount; //目标数量
};



//指令响应
struct SPIC_Response
{
    uint32 time;
    uint8  systemCtrlResp;  //系統控制响应
    uint8  systemStatus;    //系统状态
    uint8  signalProcessResp;
   // SignalControl  signalStatus;
    uint8  errorStatus;
    uint8 reserve;
};
//状态数据
struct SPIC_Status
{
    uint32 time;
    uint8  systemStatus;
    //SignalControl  signalStatus;
    uint8 reserve;
};
///////////////////////内部接口//////////////////////////////////////
////////////////接口控制模块（IC） - 激光处理模块（LS）/////////////////////////////

struct ICLS_Config
{
    uint32 time;
    uint8  systemControl;
    //LaserControl  laserCtrl;
    uint16 reserve;
};


struct LSIC_Response
{
    uint32 time;
    uint8  systemCtrlResp;  //系統控制响应
    uint8  systemStatus;    //系统状态
    uint8  laserResp;
    //LaserControl  laserStatus;
    uint8 errorStatus;
    uint8 reserve;
};
//状态数据
struct LSIC_Status
{
    uint32 time;
    uint8  systemStatus;
    uint8  errorStatus;
    uint8 reserve;
};

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define MSG_LENGH_RESERVE     42
#define MSG_FLAG_LEN          4
#define MSG_LENGTH_LEN        2
#define MSG_MIN_LEN           48
#define MSG_TYPE_PAGE_CTL     0X00300025        //MFD控制消息
#define MSG_TYPE_BIT          0X00300026        //维护自检消息
#define MSG_TYPE_SET_TIME     0X00300027        //授时消息
#define MSG_TYPE_TARGET       0X00320001        //目标识别消息
#define MSG_TYPE_LASER        0X00320002        //目标识别消息
#define MSG_TYPE_VERSION      0X00320003        //软件版本消息
#define MSG_TYPE_PAGE_STATUS  0X00320004    //页面切换状态

enum class IpcMsgType
{
    configMessage
};

//valid MFD 0-N/A  1-MFD3有效  2-MFD4有效
enum class MFD
{
    NA = 0,
    MFD3, 
    MFD4
};
//Page control
struct MFD_Valid
{
    int8 keyValid;//0-N/A  1-MFD3有效  2-MFD4有效
    int8 reserve;
};

struct MFD_Page
{
    int8 page;
    int8 subPage;
};

struct MFD_Control
{
    int8      reserve[MSG_LENGH_RESERVE];
    MFD_Valid validMfd;
    MFD_Page  mfd3;
    MFD_Page  mfd4;
};

//BIT信息 0-N/A 1-正常 2-自检 3-维护
enum class BitStatus
{
    NA = 0,
    Normal, 
    Bit,
    Maintain
};
struct MFD_IC_BIT
{
    int8 bitCtl;   
    int8 reserve2;
};

//目标识别信息状态
struct TargetHeader
{
    uint64  timsStamp;   //时间戳
    float64 longitude;   //经度
    float64 latitude;    //纬度
    float32 navigationAltidute;//导航高度
    float32 headingAngle;//横向角
    float32 pitchAngle;  //俯仰角
    float32 rollAngle;   //横滚角
    float32 reserve1;
    uint8   highTensionTargetCount;   //高压线目标个数
    uint8   nonHighTessionTargetCount;//非高压线目标个数
};
 
//目标置信度向量
struct ConfidenceVector
{
    uint8  highTensionLine;       //高压线
    uint8  highTensionLineTower;  //高压线塔
    uint8  chimney;               //烟冲
    uint8  building;              //建筑物
    uint8  crane;                 //塔吊
    uint8  bridge;                //桥梁
    uint8  vehicle;               //车辆
    uint8  signalTower;           //信号塔
    uint8  other;
    uint8  reserve[7];
};
//高压线目标信息
struct HighTensionTarget
{
    uint8   type;                //目标类型
    uint8   confidenceLevel;     //目标置信度
    ConfidenceVector confidenceVector;//目标置信度向量

    int16   leftAzimuthAngle;    //高压线左端方位角
    int16   leftPitchAngle;      //高压线左端俯仰角
    int16   leftDistance;        //高压线左端距离
    int16   rightAzimuthAngle;   //高压线右端方位角
    int16   rightPitchAngle;     //高压线右端俯仰角
    int16   rightDistance;       //高压线右端距离

    int16   leftAzimuthAngleError; //左端方位角均方根误差
    int16   leftPitchAngleError;   //左端俯仰角均方根误差
    int16   leftDistanceError;    //左端距离均方根误差
    int16   rightAzimuthAngleError; //左端方位角均方根误差
    int16   rightPitchAngleError;   //左端俯仰角均方根误差
    int16   rightDistanceError;    //左端距离均方根误差

/*
    int32   leftAzimuthAngle;    //高压线左端方位角
    int32   leftPitchAngle;      //高压线左端俯仰角
    int32   leftDistance;        //高压线左端距离
    int32   rightAzimuthAngle;   //高压线右端方位角
    int32   rightPitchAngle;     //高压线右端俯仰角
    int32   rightDistance;       //高压线右端距离

    int32   leftAzimuthAngleError; //左端方位角均方根误差
    int32   leftPitchAngleError;   //左端俯仰角均方根误差
    int32   leftDistanceError;    //左端距离均方根误差
    int32   rightAzimuthAngleError; //左端方位角均方根误差
    int32   rightPitchAngleError;   //左端俯仰角均方根误差
    int32   rightDistanceError;    //左端距离均方根误差
    */
};

//非高压线目标信息 
struct NonHighTensionTarget
{
    uint8   type;                //目标类型
    uint8   confidenceLevel;     //目标可信度
    ConfidenceVector confidenceVector;//目标置信度向量
    float32   zenithParam1;        //目标最高点参数1
    float32   zenithParam2;        //目标最高点参数2
    float32   zenithParam3;        //目标最高点参数3
    float32   zenithParam1Error;        //目标最高点参数1均方根误差
    float32   zenithParam2Error;         //目标最高点参数2均方根误差
    float32   zenithParam3Error;         //目标最高点参数3均方根误差
    
    float32   centerParam1;        //中心位置参数1
    float32   centerParam2;        //中心位置参数2
    float32   centerParam3;        //中心位置参数3
    float32   centerParam1Error;   //中心位置参数1均方根误差
    float32   centerParam2Error;   //中心位置参数2均方根误差
    float32   centerParam3Error;   //中心位置参数3均方根误差

    float32   outlineHeight;       //轮廓高度
    float32   outlineWidth;        //轮廓宽度
    float32   outlineDepth;        //轮廓深度
    float32   outlineHeightError;  //轮廓高度均方根误差
    float32   outlineWidthError;   //轮廓宽度均方根误差
    float32   outlineDepthError;   //轮廓宽度均方根误差

    float32   outlineAngleParam1;       //轮廓角度参数1
    float32   outlineAngleParam2;       //轮廓角度参数2
    float32   outlineAngleParam3;       //轮廓角度参数3
    float32   outlineAngleParam1Error;  //轮廓角度参数1均方根误差
    float32   outlineAngleParam2Error;  //轮廓角度参数2均方根误差
    float32   outlineAngleParam3Error;  //轮廓角度参数3均方根误差
};

//激光雷达状态
struct LaserStatus
{
    int8 laserWorkStatus;    //激光雷达工作状态
    int8 laserErrorStatus;   //激光雷达故障状态
};

//软件版本
struct SoftVersion
{
    int8 major;
    int8 minor;
    int8 build;
    int8 reserve1;
    int8 year1;  //个位
    int8 year2;  //十位
    int8 year3;  //百位
    int8 year4;  //千位
    int8 day1;   //日个位
    int8 day2;   //日十位
    int8 month1; //月百位
    int8 month2; //月千位
};
#pragma pack()

