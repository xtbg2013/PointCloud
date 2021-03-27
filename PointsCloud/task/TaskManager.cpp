#include "TaskManager.h"
//#include "glog/logging.h"
#include "UdpSocket.h"
#include "PciCom.h"
#include "SerialCom.h"
#include "IpcCom.h"
#include "DataDefine.h"
#include "ProtocolTransition.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <sys/time.h>
#include "PointsProcess.h" 
#include "StatusManager.h"
#include <string.h>
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
    m_udpCtrlThread = 0;
    m_pcieThread    = 0;
   
    m_pUdpControlCom = new UdpClentSocket(UDP_PLATFORM_IP,UDP_CONFIG_PORT);
    m_pUdpDataCom    = new UdpClentSocket(UDP_PLATFORM_IP,UDP_DATA_PORT);
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
    bool result = m_pciCom.Initialize();
    if(!result)
    {
       // LOG(ERROR) << "Initialize pcie communication error";
    }
    

    result =  m_pUdpDataCom->Initialize();
    if(!result)
    {
       // LOG(ERROR) << "Initialize data UDP com with platform error"; 	   
    }

    result =  m_pUdpControlCom->Initialize();
    if(!result)
    {
        //LOG(ERROR) << "Initialize control UDP com with platform error"; 	   
    }
    //LOG(INFO) << "Initialize  communication success"; 
    
    return result;
}


void TaskManager::ProcessRecvMessage(int8 *pBuffer,int32 length)
{
    Message msg;
    ProtocolParse parser;
    if(parser.ParseMessage(pBuffer,length,msg)) 
    {
        int32 length = 0;
        MFD_Control *pControl = static_cast<MFD_Control*>(msg.GetData(length));
        if(pControl != nullptr)
        {
            if(StatusManager::Instance()->ProcessMFD(*pControl))
            {
                IpcMsg icpMsg;
                icpMsg.renderingControl = (unsigned char)StatusManager::Instance()->GetRendringControl();
                //icpMsg.msgType =  static_cast<long>(IpcMsgType::configMessage);
                //icpMsg.laserControl = configInfo.laserControl;
                //icpMsg.renderingControl = configInfo.renderingControl;
                //icpMsg.workControl = configInfo.workControl;
                //icpMsg.systemControl = configInfo.systemControl;
              //  IpcCom::Instance()->Write(icpMsg);
                printf("%d\n",icpMsg.renderingControl);
                 
            }
        }
    }
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
            pTask->ProcessRecvMessage(data,length);
           
        }
		usleep(LOW_SPEED_DELAY);
	}
	return 0;
}

void TaskManager::SendToPlatform(Point_XYZI *pData,int size)
{
    vector<MessageBase> msgVector;
    ProtocolTransition::SplitPointsToUdpFrame(pData,size,msgVector);
    cout<<"All UDP frames = "<<msgVector.size()<<"-------"<< GetCurTime()<<endl; 
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


 
void TaskManager::SetPointsBuffer(Point_XYZI *pData,int size)
{
    if(!m_pointsBuffer.isUpdated)
    {
        memcpy(m_pointsBuffer.pData,pData,size*sizeof(Point_XYZI));
        m_pointsBuffer.size = size;
        m_pointsBuffer.isUpdated = true;
    }
}

void *TaskManager::PcieComThread(void *ptr)
{
    long long allFrame = 0;
    TaskManager *pTask = (TaskManager *)ptr;
    
    int leftFrame = 0;
    while(!pTask->IsTerminate())
    { 
        bool isValid = false;
        int size = 0;
        Point_XYZI* pData = pTask->m_pciCom.Read(size);
        isValid = (pData != nullptr && size > 0 && size <= MAX_POINTS_OF_PCID);
        if(isValid)
        {
            
            cout<<"All frames: "<< ++allFrame <<"  points size: "<<size<<"-------"<< GetCurTime()<<endl;
            int length = IpcShareMemory::Instance()->Write(pData,size*sizeof(Point_XYZI));
            if(length > 0)
            {
                cout<<"更新点云数据："<<length/sizeof(Point_XYZI)<<endl;
            }
            else
            {
                
                cout<<"丢包："<<++leftFrame<<endl;
            }
            
            pTask->SendToPlatform(pData,size);
            
        }
    }
    return 0;
}

bool TaskManager::Launch()
{
    bool result = true;
    int ret = 0;
    ret = pthread_create(&m_pcieThread, NULL, PcieComThread, this);
    if(ret)
    {
        //LOG(ERROR) << "Create PCIE com task unsuccess";
        result = false;
    }
    else
    {
        //LOG(INFO) << "Create PCIE com task success";
    }   
   
    ret = pthread_create(&m_udpCtrlThread, NULL, UdpCtrlThread, this);
    if(ret) {
        //LOG(ERROR) << "Create UDP control task unsuccess";
        result = false;
    }
    else{
        //LOG(INFO) << "Create UDP control task success";
    }   
    
    return result;
}