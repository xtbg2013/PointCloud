#pragma once
#include <vector>
#include "Message.h"
#include "pciecommmng/pcietransprocess.h"
using namespace std;
class ProtocolTransition
{
public:
    ProtocolTransition();
    virtual ~ProtocolTransition();
    static  uint32 GetMsgMaxLength();                             //获取消息最大长度
    static  bool   CheckMessage(int8 *pBuffer,uint32 length); 
    static  uint32 CheckSum(int8 *pBuffer,int32 length);           //计算校验和
    static  bool   ParseMessage(int8 *pBuffer,uint32 length,MessageBase &msg);  //解析接收的消息
    static  uint32 AssembleMessage(MessageBase msg,int8*pBuffer,uint32 length);//组装消息包

    static  uint32 SplitPointsToUdpFrame(Point_XYZI *pData,int size,vector<MessageBase> &msgVector);   //将点云数据拆分成多个UDP包
};