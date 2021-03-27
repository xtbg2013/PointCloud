#pragma once
#include "ICom.h"
#include "PciCom.h"
#include <pthread.h>
#include "ProtocolTransition.h"
#include <queue>
using std::queue;
/*********************************************
 * 采集点云任务，并将数据通过UDP传送至智能平台
 *********************************************/

struct PointsBuffer
{
    Point_XYZI pData[MAX_POINTS_OF_PCID];
    int  size;
    bool isUpdated;
};

class TaskManager
{
private:
    bool  m_bTerminate;
    pthread_t m_udpCtrlThread;
    pthread_t m_pcieThread;
 
    PointsBuffer m_pointsBuffer;
     
public:
    PciCom m_pciCom;
    ICom   *m_pLaserCom;
    ICom   *m_pUdpDataCom;    //向平台传输点云等实时数据
    ICom   *m_pUdpControlCom; //接收平台控制信息，并响应

protected:
    static void *UdpCtrlThread(void *ptr);
    static void *PcieComThread(void *ptr);
public:
    void SendToPlatform(Point_XYZI *pData,int size);
    void SetPointsBuffer(Point_XYZI *pData,int size);
    void ProcessRecvMessage(int8 *pBuffer,int32 length);
public:
    TaskManager();
    virtual ~TaskManager();
    bool InitilizeCom();
    bool Launch();
    void Terminate(bool bTerminate);
    bool IsTerminate();
};

