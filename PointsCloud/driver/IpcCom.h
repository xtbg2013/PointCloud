#pragma once
/****************************************************************
 * 描述：
 * 通过队列实现进程间通信，主要用于对点云显示的控制
 ***************************************************************/
#include <sys/types.h>
//#include <pcl/point_types.h>
#include "UdpSocket.h"
#include <vector>
using std::vector;
//using namespace pcl;

#define MAX_POINTS 512*1024
#if 0
struct IpcMsg
{
    long msgType;
    unsigned char  systemControl;     //维护自检控制:00：默认  03-维护状态 05-自检状态
    unsigned char  workControl;       //工作模式控制：默认状态：0x00  0x03:收藏 0x05-巡航避障 0x06-盲降辅助评估 0x09-精准着落引导
    unsigned char  laserControl;      //00-保持不变 03-俯仰角增大 05-俯仰角减小 33-方位角增大 35-方位角减小
    unsigned char  renderingControl;  //显示渲染控制：0x00-默认，不作响应 0xFF-模式切换
};

class IpcCom
{
private:
    IpcCom(/* args */);
    key_t m_key;//通信Key
    int m_msgid;//消息队列ID
    static IpcCom *m_pIpcCom;
public:
    
    ~IpcCom();
    static IpcCom *Instance();
    bool Initialize();
    void Release();
    int  Read(IpcMsg &msg);
    int  Write(IpcMsg msg);
};
#endif



struct SM_Point_XYZI
{
    float x;
    float y;
    float z;
    float intensity;
};


//内存共享
//点云数据
struct ShareMemory
{
    bool  isUpdated;
    int   size;   //更新数据长度
    SM_Point_XYZI  buffer[MAX_POINTS];
};
class IpcShareMemory
{
private:
    key_t m_key;//Key
    int m_shmId;//ID
    void *m_pShareMemory;
    static IpcShareMemory *m_pIpcSm;
    IpcShareMemory();
    
    bool Initialize();  //初始化，创建共享内存
    bool Release();     //释放共享内存，但并未删除
public:
    virtual ~IpcShareMemory();
    static IpcShareMemory *Instance();
   // int  Read(vector<PointXYZI> &points);//返回值：点数

    int  Read(void  *pBuffer,int length);//返回值：接收数据总长度 
    int  Write(void *pBuffer,int length);//返回值：发送数据总长度
};




struct IpcMsg
{
    bool isUpdated;
    long msgType;
    unsigned char  systemControl;     //维护自检控制:00：默认  03-维护状态 05-自检状态
    unsigned char  workControl;       //工作模式控制：默认状态：0x00  0x03:收藏 0x05-巡航避障 0x06-盲降辅助评估 0x09-精准着落引导
    unsigned char  laserControl;      //00-保持不变 03-俯仰角增大 05-俯仰角减小 33-方位角增大 35-方位角减小
    unsigned char  renderingControl;  //显示渲染控制：0x00-默认，不作响应 0xFF-模式切换
};

class IpcCom
{
private:
    IpcCom(/* args */);
    key_t m_key;//Key
    int m_shmId;//ID
    void *m_pShareMemory;
    static IpcCom *m_pIpcCom;
    bool Initialize();
    bool Release();
public:
    
    ~IpcCom();
    static IpcCom *Instance();
    
    int  Read(IpcMsg &msg);
    int  Write(IpcMsg msg);
};