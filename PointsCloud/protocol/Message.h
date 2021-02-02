#pragma once
#include "DataDefine.h"
#include <vector>
using std::vector;

class MessageBase
{
protected:
    uint16 m_header;               //消息头
    uint16 m_length;               //消息长度：除消息头，消息长度外，后续所有数据 
    uint16 m_msgType;              //消息类型：标识
    uint32 m_checkSum;             //校验和
    uint16 m_dataLength;           //消息数据段长度：不包含消息头，消息长度，标识，检验和
    int8   m_data[MAX_MSG_LEN];    //消息数据段内容
public:
    MessageBase(/* args */);
    virtual ~MessageBase();
   
    uint16  GetHeader();
    void    SetHeader(uint16 header);
    uint16  GetLength();
    void    SetLength(uint16 length);
    uint16  GetMsgType();
    void    SetMsgType(uint16 msgType);
    void*   GetData(uint32 &length);//获取消息数据
    int32   SetData(void *pBuffer,uint32 length);//设置消息数据
    uint32  GetCheckSum();
    void    SetCheckSum(uint32 checkSum);
    uint32  GetMsgLength();         //获取消息实际长度
};

////////////////////////智能计算平台(MP)----接口控制模块(IC)//////////////////////////////////////////////////
//智能计算平台-->接口控制模块：配置消息
class MPIC_ConfigMessage:public MessageBase
{
private:
    MPIC_Config   m_config;
public:
    MPIC_ConfigMessage(/* args */);
    virtual ~MPIC_ConfigMessage();
    MPIC_Config GetConfig();
    void        SetConfig(MPIC_Config config);
};

//接口控制模块-->智能计算平台：点云数据
class ICMP_PointCloudMessage:public MessageBase
{
private:
    ICMP_PointsData m_pointsData;
public:
    ICMP_PointCloudMessage(/* args */);
    virtual ~ICMP_PointCloudMessage();
 //   virtual uint32 GetCheckSumFeildLength();
    void  SetPointsData(ICMP_PointsData pointsData);
    ICMP_PointsData GetPointsData();
};

//接口控制模块<-->智能计算平台：目标识别数据

class TargetInfoMessage:public MessageBase
{
private:
    TargetHeader m_targetHeader;                             //目标头
    vector<HighTensionTarget>     m_highTesionLineTarget;    //高压线目标
    vector<NonHighTensionTarget>  m_nonHighTesionLineTarget; //非高压线目标
public:
    TargetInfoMessage(/* args */);
    virtual ~TargetInfoMessage();
    
    void  SetTargetHeader(TargetHeader header);
    TargetHeader GetTargetHeader();
    
    void AddHighTesionLineTarget(HighTensionTarget target);
    void AddNonHighTesionLineTarget(NonHighTensionTarget target);

    vector<HighTensionTarget> GetHighTesionTarget();
    vector<NonHighTensionTarget> GetNonHighTesionTarget();

    virtual bool   ParseMessage(int8 *pBuffer,uint32 length);  //解析接收的消息
    virtual uint32 AssembleMessage(int8*pBuffer,uint32 length);//组装消息包
};


//智能计算平台-->接口控制模块：惯导数据
class MPIC_InsMessage:public MessageBase
{
public:
    MPIC_InsMessage(/* args */);
    virtual ~MPIC_InsMessage();
  //  int8*   GetInsData(uint32 &length);//获取导航数据
};


 

//接口控制模块-->智能计算平台：地形探测数据
class MPIC_TopographicInfoMessage:public MessageBase
{
private:
    TopographicInfo m_topographicInfo; //地形探测数据
public:
    MPIC_TopographicInfoMessage(/* args */);
    virtual ~MPIC_TopographicInfoMessage();
    void SetTopographicInfo(TopographicInfo info);
    TopographicInfo GetTopographicInfo();
};


//接口控制模块-->智能计算平台：指令响应
class ICMP_ResponseMessage:public MessageBase
{
private:
    ICMP_Response m_response; //响应数据
public:
    ICMP_ResponseMessage(/* args */);
    virtual ~ICMP_ResponseMessage();
    void SetResponse(ICMP_Response response);
    ICMP_Response GetResponse();
};

//接口控制模块-->智能计算平台：状态消息
class ICMP_StatusMessage:public MessageBase
{
private:
    ICMP_Status m_status; //状态消息数据
public:
    ICMP_StatusMessage(/* args */);
    virtual ~ICMP_StatusMessage();
    void SetStatus(ICMP_Status status);
    ICMP_Status GetStatus();
};


////////////////////////////////////内部接口////////////////////////////////////////////////////
////////////////////////////////////接口控制模块(IC)----信号处理模块(SP)///////////////////////////
/*
    配置项数据：接口控制模块(IC)---->信号处理模块(SP)
*/

class ICSP_ConfigMessage:public MessageBase
{
private:
    ICSP_Config m_config; //状态消息数据
public:
    ICSP_ConfigMessage(/* args */);
    virtual ~ICSP_ConfigMessage();
    void SetStatus(ICSP_Config m_config);
    ICSP_Config GetStatus();
};

/*
    点云数据：信号处理模块(SP)---->接口控制模块(IC)
*/
//接口控制模块-->智能计算平台：点云数据
class SPIC_PointCloudMessage:public MessageBase
{
private:
    SPIC_PointsData m_pointsData;
public:
    SPIC_PointCloudMessage(/* args */);
    virtual ~SPIC_PointCloudMessage();
    void  SetPointsData(SPIC_PointsData pointsData);
    SPIC_PointsData GetPointsData();
};
 