#include "Message.h"
#include <string.h>
MessageBase::MessageBase(/* args */)
{
    m_header = 0;
    m_length = 0;
    m_msgType = 0;
    m_dataLength = 0;
    memset(m_data,0,sizeof(m_data));
    m_checkSum = 0;
}

MessageBase::~MessageBase()
{
}
uint32 MessageBase::GetMsgLength()
{
    return MSG_HEAD_FIELD_LEN + MSG_LENGTH_FIELD_LEN + GetLength();
}

uint16 MessageBase::GetHeader()
{
    return m_header;

}
void MessageBase::SetHeader(uint16 header)
{
    m_header = header;
}
uint16 MessageBase::GetLength()
{
    return m_length;

}
void MessageBase::SetLength(uint16 length)
{
    m_length = length;
}
 
 
void* MessageBase::GetData(uint32 &length)
{
    length = m_dataLength;
    return m_data;
}
/**************************************
 * Desription:
 * 功能：设置数据段内容：除消息头，消息长度，标识，检验和之外的所有数据
 * 参数：pBuffer 存放数据的buffer,length 存放数据buffer长度
 * 方法：将消息数据取累加校验，并与校验位比较
 * 返回：实际获取数据长度
 **************************************/
int32 MessageBase::SetData(void *pBuffer,uint32 length)
{
    if(pBuffer == nullptr || length> MSG_MAX_DATA_LEN)
        return 0;

    memcpy(m_data,pBuffer,length);
    m_dataLength = length;
    return m_dataLength;
}
    
uint16 MessageBase::GetMsgType()
{
    return m_msgType;
}

void MessageBase::SetMsgType(uint16 msgType)
{
    m_msgType = msgType;
}

uint32 MessageBase::GetCheckSum()
{
    return m_checkSum;
}
void MessageBase::SetCheckSum(uint32 checkSum)
{
    m_checkSum = checkSum;
}

/**************************************
 * Desription:
 * 功能：计算校验和，累加校验
 * 参数： 
 * 方法：所有数据进行累加 
 * 返回：累加值
 **************************************/
/*
uint32 MessageBase::CheckSum(int8 *pBuffer,int32 length)
{
    uint16 sum = 0;
    for(int32 count = 0;count<length;count++) 
    {
        sum+= pBuffer[count];
    }
    return sum;
}
*/

////////////////////////智能计算平台----接口控制模块//////////////////////////////////////////////////

MPIC_ConfigMessage::MPIC_ConfigMessage(/* args */)
{
    memset(&m_config,0,sizeof(MPIC_Config));
}

MPIC_ConfigMessage::~MPIC_ConfigMessage()
{

}
MPIC_Config MPIC_ConfigMessage::GetConfig()
{
    memcpy(&m_config,m_data,m_dataLength);
    return m_config;

}
void MPIC_ConfigMessage::SetConfig(const MPIC_Config &config)
{
    m_config = config;
    m_dataLength = sizeof(MPIC_Config);
    memcpy(m_data,&m_config,m_dataLength);
}




/////////////////////////////结果级点云数据/////////////////////////////////////////////////

ICMP_PointCloudMessage::ICMP_PointCloudMessage(/* args */)
{
    memset(&m_pointsData,0,sizeof(ICMP_PointsData));
}

ICMP_PointCloudMessage::~ICMP_PointCloudMessage()
{

}

void  ICMP_PointCloudMessage::SetPointsData(ICMP_PointsData pointsData)
{
    m_pointsData = pointsData;

}

ICMP_PointsData ICMP_PointCloudMessage::GetPointsData()
{
    return m_pointsData;

}
 
/////////////////////////////地形探测数据/////////////////////////////////////////////////
MPIC_TopographicInfoMessage::MPIC_TopographicInfoMessage(/* args */)
{
    memset(&m_topographicInfo,0,sizeof(TopographicInfo));

}
MPIC_TopographicInfoMessage::~MPIC_TopographicInfoMessage()
{

}

TopographicInfo MPIC_TopographicInfoMessage::GetTopographicInfo()
{
    memcpy(&m_topographicInfo,m_data,m_dataLength);
    return m_topographicInfo;
}

void MPIC_TopographicInfoMessage::SetTopographicInfo(TopographicInfo info)
{
    m_topographicInfo = info;
    m_dataLength = sizeof(TopographicInfo);
    memcpy(&m_data,&m_topographicInfo,m_dataLength);
}

////////////////////////接口控制模块(IC)-->智能计算平台(MP)：指令响应////////////////////////////////////////////////

ICMP_ResponseMessage::ICMP_ResponseMessage(/* args */)
{
    memset(&m_response,0,sizeof(ICMP_Response));

}
ICMP_ResponseMessage::~ICMP_ResponseMessage()
{


}
void ICMP_ResponseMessage::SetResponse(ICMP_Response response)
{ 
    m_response = response;
    m_dataLength = sizeof(ICMP_Response);
    memcpy(&m_data,&m_response,m_dataLength);
}

ICMP_Response ICMP_ResponseMessage::GetResponse()
{
    memcpy(&m_response,m_data,m_dataLength);
    return m_response;
}

////////////////////接口控制模块(IC)-->智能计算平台(MP)：状态消息/////////////////////////////////////////

ICMP_StatusMessage::ICMP_StatusMessage(/* args */)
{
    memset(&m_status,0,sizeof(ICMP_Status));
}
ICMP_StatusMessage::~ICMP_StatusMessage()
{

}
void ICMP_StatusMessage::SetStatus(ICMP_Status status)
{
    m_status = status;
    m_dataLength = sizeof(ICMP_Status);
    memcpy(&m_data,&m_status,m_dataLength);
}

ICMP_Status ICMP_StatusMessage::GetStatus()
{
    memcpy(&m_status,m_data,m_dataLength);
    return m_status;

}



////////////////////////////////////内部接口////////////////////////////////////////////////////
////////////////////////////////////接口控制模块(IC)----信号处理模块(SP)///////////////////////////
 

 ICSP_ConfigMessage::ICSP_ConfigMessage(/* args */)
 {
     memset(&m_config,0,sizeof(ICSP_Config));

 }
ICSP_ConfigMessage::~ICSP_ConfigMessage()
{

}
void ICSP_ConfigMessage::SetStatus(ICSP_Config m_config)
{
    m_config = m_config;
    m_dataLength = sizeof(ICMP_Status);
    memcpy(&m_data,&m_config,m_dataLength);
}

ICSP_Config ICSP_ConfigMessage::GetStatus()
{
    memcpy(&m_config,m_data,m_dataLength);
    return m_config;

}


///////////////////////////接口控制模块-->智能计算平台：点云数据/////////////////////////////////////////////
SPIC_PointCloudMessage::SPIC_PointCloudMessage(/* args */)
{
    memset(&m_pointsData,0,sizeof(SPIC_PointsData));
}
SPIC_PointCloudMessage::~SPIC_PointCloudMessage()
{

}


void  SPIC_PointCloudMessage::SetPointsData(SPIC_PointsData pointsData)
{
    m_pointsData = pointsData;
}
SPIC_PointsData SPIC_PointCloudMessage::GetPointsData()
{
   return m_pointsData;
}


/////////////////////////目标识别数据消息////////////////////////////////////////////////////////

TargetInfoMessage::TargetInfoMessage(/* args */)
{
    memset(&m_targetHeader,0,sizeof(TargetHeader));

}
TargetInfoMessage::~TargetInfoMessage()
{

}
    
void  TargetInfoMessage::SetTargetHeader(TargetHeader header)
{
    m_targetHeader = header;
}
TargetHeader TargetInfoMessage::GetTargetHeader()
{
    return m_targetHeader;

}
    
void TargetInfoMessage::AddHighTesionLineTarget(HighTensionTarget target)
{
    m_highTesionLineTarget.push_back(target);

}
void TargetInfoMessage::AddNonHighTesionLineTarget(NonHighTensionTarget target)
{
    m_nonHighTesionLineTarget.push_back(target);
}

vector<HighTensionTarget> TargetInfoMessage::GetHighTesionTarget()
{
    return m_highTesionLineTarget;

}
vector<NonHighTensionTarget> TargetInfoMessage::GetNonHighTesionTarget()
{
    return m_nonHighTesionLineTarget;

}


bool TargetInfoMessage::ParseMessage(int8 *pBuffer,uint32 length)  //解析接收的消息
{
    int16 offset = 0;
    //消息头
    memcpy(&m_header,pBuffer+offset,MSG_HEAD_FIELD_LEN);
    offset += MSG_HEAD_FIELD_LEN;

    //消息长度
    memcpy(&m_length,pBuffer+offset,MSG_LENGTH_FIELD_LEN);
    offset += MSG_LENGTH_FIELD_LEN;
    
    //消息标识
    memcpy(&m_msgType,pBuffer+offset,MSG_TYPE_FIELD_LEN);
    offset += MSG_TYPE_FIELD_LEN;
    
    //目标头
    int targetHeaderLen = sizeof(TargetHeader);
    memcpy(&m_targetHeader,pBuffer+offset,targetHeaderLen);
    offset += targetHeaderLen;

    //高压线目标
    for(int count = 0;count<m_targetHeader.highTensionTargetCount;count++)
    {
        HighTensionTarget target;
        int targetLen = sizeof(HighTensionTarget);
        memcpy(&target,pBuffer+offset,targetLen);
        offset += targetLen;
        AddHighTesionLineTarget(target);
    }

    //非高压线目标
    for(int count = 0;count<m_targetHeader.nonHighTessionTargetCount;count++)
    {
        NonHighTensionTarget target;
        int targetLen = sizeof(NonHighTensionTarget);
        memcpy(&target,pBuffer+offset,targetLen);
        offset += targetLen;
        AddNonHighTesionLineTarget(target);
    }
        
    //校验和
    memcpy(&m_checkSum,pBuffer+offset,MSG_CHECK_SUM_FIELD_LEN);
    offset += MSG_CHECK_SUM_FIELD_LEN;
    return true;

}
uint32 TargetInfoMessage::AssembleMessage(int8*pBuffer,uint32 length)//组装消息包
{
    /*
    if(pBuffer == nullptr || length < GetMsgLength()) //buffer 比消息长度小
        return 0;
    
    uint16 offset = 0;

    //消息头
    memcpy(pBuffer+offset,&m_header,MSG_HEAD_FIELD_LEN);
    offset += MSG_HEAD_FIELD_LEN;

    //消息长度
    memcpy(pBuffer+offset,&m_length,MSG_LENGTH_FIELD_LEN);
    offset += MSG_LENGTH_FIELD_LEN;

    //消息类型
    memcpy(pBuffer+offset,&m_msgType,MSG_TYPE_FIELD_LEN);
    offset += MSG_TYPE_FIELD_LEN;

    //目标头
    int targetHeaderLen = sizeof(TargetHeader);
    memcpy(pBuffer+offset,&m_targetHeader,targetHeaderLen);
    offset += targetHeaderLen;


    //高压线目标
    for(auto xx )
    {
        HighTensionTarget target;
        int targetLen = sizeof(HighTensionTarget);
        memcpy(&target,pBuffer+offset,targetLen);
        offset += targetLen;
        AddHighTesionLineTarget(target);
    }

    //非高压线目标
    for(int count = 0;count<m_targetHeader.nonHighTessionTargetCount;count++)
    {
        NonHighTensionTarget target;
        int targetLen = sizeof(NonHighTensionTarget);
        memcpy(&target,pBuffer+offset,targetLen);
        offset += targetLen;
        AddNonHighTesionLineTarget(target);
    }



    //计算校验和
     memcpy(pBuffer+offset,&m_checkSum,GetCheckSumFeildLength());
     offset += GetCheckSumFeildLength();
     
     return offset;
     */
    return 0;

}



Message::Message(/* args */)
{
    m_msgFlag = 0;              //消息头
    m_msglength = 0;            //消息长度 
    memset(m_msgData,0,sizeof(m_msgData));
}
Message::~Message()
{

}

uint32  Message::GetMsgFlag()
{
    return m_msgFlag;
}

void    Message::SetMsgFlag(uint32 flag)
{
    m_msgFlag =  flag;
}

void*   Message::GetData(int32 &length)//获取消息数据
{
    length =  m_msglength;
    return m_msgData;

}

void   Message::SetData(void *pBuffer,int32 length)//设置消息数据
{
    if(pBuffer == nullptr || length> MAX_MSG_LEN)
        return;

    memcpy(m_msgData,pBuffer,length);
    m_msglength = length;
}
