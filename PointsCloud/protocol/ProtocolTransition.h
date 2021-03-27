#pragma once
#include <vector>
#include "Message.h"
#include "pciecommmng/pcietransprocess.h"
using namespace std;

class ProtocolParse
{
private:
    /* data */
public:
    ProtocolParse(/* args */);
    ~ProtocolParse();
    virtual bool ParseMessage(int8 *pBuffer,int32 length,Message &msg);  //解析接收的消息
};



class ProtocolTransition
{
public:
    ProtocolTransition();
    virtual ~ProtocolTransition();
    static  uint32 GetAllFrames(uint32 pointCount);     //根据点数量计算总帧数量
    static  void   TranslateICMPPointsDataToUdpMessage(const ICMP_PointsData &data,MessageBase &msg); 

    static  uint32 GetMsgMaxLength();                             //获取消息最大长度
    static  bool   CheckMessage(int8 *pBuffer,uint32 length); 
    static  uint32 CheckSum(int8 *pBuffer,int32 length);           //计算校验和
    static  bool   ParseMessage(int8 *pBuffer,uint32 length,MessageBase &msg);  //解析接收的消息
    static  uint32 AssembleMessage(MessageBase msg,int8*pBuffer,uint32 length);//组装消息包

    static  uint32 SplitPointsToUdpFrame(Point_XYZI *pData,int size,vector<MessageBase> &msgVector);   //将点云数据拆分成多个UDP包
};

