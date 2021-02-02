#include "TaskManager.h"
#include "glog/logging.h"
#include "UdpSocket.h"
#include "PciCom.h"
#include "SerialCom.h"
#include "DataDefine.h"
#include "ProtocolTransition.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <string>
#include <sys/time.h>
using namespace std;

//pthread_mutex_t m_mutex;

string GetCurTime()
{
    timeval curTime;

    gettimeofday(&curTime,nullptr);
    int ms = curTime.tv_usec /1000;
    tm nowTime;
    
 
    char s[80];
    localtime_r(&curTime.tv_sec,&nowTime);
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S ", &nowTime);
    std::string str(s);
    str = str + to_string(ms);
    return str;
}


TaskManager::TaskManager()
{
    m_bTerminate =  false;
    m_udpDataThread = 0;
    m_udpCtrlThread = 0;
    m_pcieThread    = 0;
    m_savePcdThread = 0;
    m_pUdpControlCom = new UdpClentSocket(UDP_PLATFORM_IP,UDP_CONFIG_PORT);
    m_pUdpDataCom    = new UdpClentSocket(UDP_PLATFORM_IP,UDP_DATA_PORT);
    m_pPciCom = new PciCom();
    m_pLaserCom = new SerialCom(LASER_RS422_PORT);
    memset(&m_pointsBuffer,0,sizeof(m_pointsBuffer));
    m_pointsBuffer.isUpdated = false;
   // pthread_mutex_init(&m_mutex,nullptr);
}

TaskManager::~TaskManager()
{
    if(m_pUdpDataCom != nullptr)
    {
        delete m_pUdpDataCom;
        m_pUdpDataCom = nullptr;
    }
    if(m_pUdpControlCom != nullptr)
    {
        delete m_pUdpControlCom;
        m_pUdpControlCom = nullptr;
    }

    if(m_pPciCom != nullptr)
    {
        delete m_pPciCom;
        m_pPciCom = nullptr;
    }
    if(m_pLaserCom != nullptr)
    {
        delete m_pLaserCom;
        m_pLaserCom = nullptr;
    }
}

 
void TaskManager::Terminate(bool bTerminate)
{
    m_bTerminate = bTerminate;
}
bool TaskManager::IsTerminate()
{
    return m_bTerminate;
}
//初始化通信
bool TaskManager::InitilizeCom()
{
    bool result = m_pPciCom->Initialize();
    if(!result)
    {
        LOG(ERROR) << "Initialize pcie communication error";
    }
    

    result =  m_pUdpDataCom->Initialize();
    if(!result)
    {
        LOG(ERROR) << "Initialize data UDP com with platform error"; 	   
    }
    result =  m_pUdpControlCom->Initialize();
    if(!result)
    {
        LOG(ERROR) << "Initialize control UDP com with platform error"; 	   
    }
    LOG(INFO) << "Initialize  communication success"; 
    
    return result;
}

void *TaskManager::UdpDataThread(void *ptr)
{
    TaskManager *pTask = (TaskManager *)ptr;
    char data[10000] = {0};
    while(!pTask->IsTerminate())
	{

        int length = pTask->m_pUdpDataCom->Read(data,sizeof(data));
        if(length>0)
        {
            cout<<"recv ="<<length<<endl;
        
        } 
		usleep(HIGH_SPEED_DELAY);
	}
	
    //LOG(INFO) << "UDP data task has terminated";
	return 0;
}

void *TaskManager::UdpCtrlThread(void *ptr)
{
    TaskManager *pTask = (TaskManager *)ptr;
    char data[1000] = {0};
    while(!pTask->IsTerminate())
	{
        int length = pTask->m_pUdpControlCom->Read(data,sizeof(data));
        if(length > 0 )
        {
            if(ProtocolTransition::CheckMessage(data,length)) //判断消息是否合法
            {
                MessageBase msg;
                ProtocolTransition::ParseMessage(data,length,msg);
                if(msg.GetMsgType() == MSG_MPIC_TYPE_CONFIG)
                {
                   
                }
            }
            //pTask->m_pUdpControlCom->Write(data,length);
        }
		usleep(LOW_SPEED_DELAY);
	}
	
  //  LOG(INFO) << "UDP control task has terminated";
	return 0;
}

void TaskManager::SendToPlatform(Point_XYZI *pData,int size)
{
    vector<MessageBase> msgVector;
    ProtocolTransition::SplitPointsToUdpFrame(pData,size,msgVector);
    char buffer[MAX_MSG_LEN] = {0};
    for(auto it : msgVector)
    { 
        int32 len = ProtocolTransition::AssembleMessage(it,buffer,sizeof(buffer));
        if(len>0)
        {
            m_pUdpDataCom->Write(buffer,len);
        }  
    }
}

int TaskManager::SaveToPcd(Point_XYZI *pData,int size)
{
    static long frameId = 0;
    string strFrameId = to_string(frameId++);
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);//创建自定义点云格式PointXYZI
    cloud->header.frame_id = strFrameId;
    cloud->width = size;
    cloud->height = 1;
    cloud->points.resize(cloud->width * cloud->height);
    for (size_t i = 0; i < cloud->points.size (); ++i) {
        cloud->points[i].x = pData[i].x;
        cloud->points[i].y = pData[i].y;
        cloud->points[i].z = pData[i].z;
        cloud->points[i].intensity = pData[i].intensity;
    }
    string name =  "./pcd/frame"+strFrameId+".pcd"; //
    pcl::io::savePCDFile (name, *cloud);
    return 0;
}
 
void TaskManager::SetPointsBuffer(Point_XYZI *pData,int size)
{
    if(!m_pointsBuffer.isUpdated)
    {
        memcpy(m_pointsBuffer.pData,pData,size*sizeof(Point_XYZI));
        m_pointsBuffer.size = size;
        m_pointsBuffer.isUpdated = true;
    }
}
bool TaskManager::SavePointsToPcd()
{
    bool result = false;
    if(m_pointsBuffer.isUpdated)
    {
        SaveToPcd(m_pointsBuffer.pData,m_pointsBuffer.size);
        m_pointsBuffer.isUpdated = false;
        result = true;
    }
    return result;
}

void *TaskManager::PcieComThread(void *ptr)
{
    long long allFrame = 0;
    TaskManager *pTask = (TaskManager *)ptr;
    while(!pTask->IsTerminate())
    { 
        bool isValid = false;
        int size = 0;
        Point_XYZI* pData = pTask->m_pPciCom->Read(size);
        isValid = (pData != nullptr && size > 0 && size <= MAX_POINTS_OF_PCID);
        if(isValid)
        {
            cout<<"All Frames: "<< ++allFrame <<"  Points size: "<<size<<"-------"<< GetCurTime()<<endl;
            pTask->SetPointsBuffer(pData,size);
            pTask->SendToPlatform(pData,size);
            
        }
        
    }
    return 0;
}

void *TaskManager::SavePcdThread(void *ptr)
{
    TaskManager *pTask = (TaskManager *)ptr;
    while(!pTask->IsTerminate())
    { 
        bool result = pTask->SavePointsToPcd();
        if(!result)
        {
            usleep(10);
        }
    }
    return 0;
}



bool TaskManager::Launch()
{
    bool result = true;
    int ret = pthread_create(&m_pcieThread, NULL, PcieComThread, this);
    if(ret)
    {
        LOG(ERROR) << "Create PCIE com task unsuccess";
        result = false;
    }
    else
    {
        LOG(INFO) << "Create PCIE com task success";
    }   


    ret = pthread_create(&m_savePcdThread, NULL, SavePcdThread, this);
    if(ret)
    {
        LOG(ERROR) << "Create save pcd task unsuccess";
        result = false;
    }
    else
    {
        LOG(INFO) << "Create save pcd task success";
    }   


    /*
    int ret = pthread_create(&m_udpDataThread, NULL, UdpDataThread, this);
    if(ret) {
        LOG(ERROR) << "Create UDP Data task unsuccess";
        result = false;
    }
    else{
        LOG(INFO) << "Create UDP Data task success";
    }   

    ret = pthread_create(&m_udpCtrlThread, NULL, UdpCtrlThread, this);
    if(ret) {
        LOG(ERROR) << "Create UDP control task unsuccess";
        result = false;
    }
    else{
        LOG(INFO) << "Create UDP control task success";
    }   
    */
    return result;
}