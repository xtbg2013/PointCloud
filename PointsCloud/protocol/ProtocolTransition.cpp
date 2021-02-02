#include "ProtocolTransition.h"
#include <string.h>
ProtocolTransition::ProtocolTransition()
{
}

ProtocolTransition::~ProtocolTransition()
{
}

/**************************************
 * Desription:
 * 功能：计算校验和，累加校验
 * 参数： 
 * 方法：所有数据进行累加 
 * 返回：累加值
 **************************************/

uint32   ProtocolTransition::CheckSum(int8 *pBuffer,int32 length)
{
    uint16 sum = 0;
    for(int32 count = 0;count<length;count++) 
    {
        sum+= pBuffer[count];
    }
    return sum;
}

uint32 ProtocolTransition::GetMsgMaxLength()
{
    return (MSG_HEAD_FIELD_LEN + MSG_LENGTH_FIELD_LEN + MSG_TYPE_FIELD_LEN + MSG_MAX_DATA_LEN + MSG_CHECK_SUM_FIELD_LEN);
}

bool ProtocolTransition::CheckMessage(int8 *pBuffer,uint32 length)
{
    //一个完整的包至少包括：包头，长度，校验
    if(pBuffer == nullptr ||(length- MSG_HEAD_FIELD_LEN - MSG_LENGTH_FIELD_LEN - MSG_CHECK_SUM_FIELD_LEN< 0))
        return false;
    if(length > GetMsgMaxLength()) //包长度大于最大长度
        return false;
    uint16  offset = length - MSG_CHECK_SUM_FIELD_LEN;
    int16 meaSum = CheckSum(pBuffer,offset); 
    //最后两字节为校验数据
    int16 srcSum = 0;
    memcpy(&srcSum,pBuffer+offset,MSG_CHECK_SUM_FIELD_LEN);
    return meaSum == srcSum;

}
/**************************************
 * Desription:
 * 功能：解析消息，提取消息个字段，存入各自缓存：
 * 参数：pBuffer 完整的消息,包括消息头，长度，数据，校验, length 消息长度
 * 返回：
 *    true  == 解析成功
 *    false == 解析失败
 **************************************/

bool ProtocolTransition::ParseMessage(int8 *pBuffer,uint32 length,MessageBase &msg)
{
    int16 offset = 0;
    //消息头
    uint16 header = 0;
    memcpy(&header,pBuffer+offset,MSG_HEAD_FIELD_LEN);
    offset += MSG_HEAD_FIELD_LEN;
    msg.SetHeader(header);

    //消息长度
    uint16 len = 0;
    memcpy(&len,pBuffer+offset,MSG_LENGTH_FIELD_LEN);
    offset += MSG_LENGTH_FIELD_LEN;
    msg.SetLength(len);
    
    //消息标识
    uint16 msgType = 0;
    memcpy(&msgType,pBuffer+offset,MSG_TYPE_FIELD_LEN);
    offset += MSG_TYPE_FIELD_LEN;
    msg.SetMsgType(msgType);
    //消息内容

    uint32 dataLength = msg.GetLength() - MSG_TYPE_FIELD_LEN;
    if(dataLength < 0)
        return false;
    msg.SetData(pBuffer+offset,dataLength);
    offset += dataLength;
    

    //校验和
    uint16 checkSum = 0;
    memcpy(&checkSum,pBuffer+offset,MSG_CHECK_SUM_FIELD_LEN);
    offset += MSG_CHECK_SUM_FIELD_LEN;
    msg.SetCheckSum(checkSum);
    return true;
}

uint32 ProtocolTransition::AssembleMessage(MessageBase msg,int8*pBuffer,uint32 length)//组装消息包
{
    if(pBuffer == nullptr || length < msg.GetMsgLength()) //buffer 比消息长度小
        return 0;
    
    uint16 offset = 0;

    //消息头
    uint16 header = msg.GetHeader();
    
    memcpy(pBuffer+offset,&header,MSG_HEAD_FIELD_LEN);
    offset += MSG_HEAD_FIELD_LEN;

    //消息长度
    uint16 len =  msg.GetLength();
    memcpy(pBuffer+offset,&len,MSG_LENGTH_FIELD_LEN);
    offset += MSG_LENGTH_FIELD_LEN;

    //消息类型
    uint16 msgType = msg.GetMsgType();
    memcpy(pBuffer+offset,&msgType,MSG_TYPE_FIELD_LEN);
    offset += MSG_TYPE_FIELD_LEN;

    //数据
    uint32 dataLen = 0;
    void *pData = msg.GetData(dataLen);
    memcpy(pBuffer+offset,pData,dataLen);
    offset += dataLen;

    //计算校验和
    uint16 checkSum = CheckSum(pBuffer,offset);
    memcpy(pBuffer+offset,&checkSum,MSG_CHECK_SUM_FIELD_LEN);
    offset += MSG_CHECK_SUM_FIELD_LEN;
     
    return offset;
}

uint32 ProtocolTransition::SplitPointsToUdpFrame(Point_XYZI *pData,int size,vector<MessageBase> &msgVector)
{
    int allFrames = 0;
    if(size % MAX_POINTS_PER_FRAME != 0){
        allFrames = size / MAX_POINTS_PER_FRAME + 1;
    }
    else{
        allFrames = size / MAX_POINTS_PER_FRAME;
    }


    int frameNo = 0;
    int leftPoints = size;
    int offset = 0;
    while(leftPoints > 0)
    {
        ICMP_PointsData  pointsData = {0};
        pointsData.serialNo = ++frameNo;
        pointsData.allframes = allFrames; 
        pointsData.time = 0;
        pointsData.longitude = 0;   //经度
        pointsData.latitude = 0;    //纬度
        pointsData.navigationAltidute = 0;//导航高度
        pointsData.rollAngle = 0;   //横滚角
        pointsData.pitchAngle = 0;  //俯仰角
        pointsData.headingAngle = 0;//航向角
        pointsData.reserve = 0;
        int copyLength  = 0;
        if(leftPoints >= MAX_POINTS_PER_FRAME)
        {
            copyLength = sizeof(Point_XYZI)*MAX_POINTS_PER_FRAME;
        }
        else
        {
            copyLength = sizeof(Point_XYZI)*leftPoints;
        }
        memcpy(pointsData.points,pData + offset,copyLength);
        offset += copyLength;
        leftPoints -= copyLength;

        MessageBase msg;
        msg.SetHeader(MSG_HEAD);
        msg.SetLength(sizeof(ICMP_PointsData) + MSG_CHECK_SUM_FIELD_LEN2);
        msg.SetMsgType(MSG_ICMP_TYPE_POINTCLOUD);
        msg.SetData((void*)&pointsData,sizeof(ICMP_PointsData));

        msgVector.push_back(msg);
    }
    
    return allFrames;
}