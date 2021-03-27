#include "IpcCom.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>

#include <stdio.h>
 
//#include<sys/types.h>
#include<sys/stat.h>
//#include <fcntl.h>
 
 
//#include "glog/logging.h"
#if 0

IpcCom *IpcCom::m_pIpcCom =  nullptr;
IpcCom::IpcCom(/* args */)
{
    m_key = 0;
    m_msgid = 0;
}

IpcCom::~IpcCom()
{
}

IpcCom *IpcCom::Instance()
{
    if(m_pIpcCom ==  nullptr)
    {
        m_pIpcCom = new IpcCom();
    }
    return m_pIpcCom;

}
bool IpcCom::Initialize()
{
    m_key = ftok("/home",'6');

    m_msgid = msgget(m_key,IPC_CREAT|0777);
    
    if(m_msgid < 0)
    {   
       // LOG(INFO) <<"IPC msgget error!";
        return false;
    }   
    return true;
}
void IpcCom::Release()
{
    msgctl(m_msgid,IPC_RMID,NULL);
    if(m_pIpcCom != nullptr)
    {
        delete m_pIpcCom;
        m_pIpcCom  = nullptr;
    }
}

int  IpcCom::Read(IpcMsg &msg)
{
	return msgrcv(m_msgid,&msg,sizeof(msg)-sizeof(msg.msgType),0,IPC_NOWAIT);
}

int  IpcCom::Write(IpcMsg msg)
{
	return msgsnd(m_msgid,&msg,sizeof(msg)-sizeof(msg.msgType),0);
}
#endif

IpcShareMemory *IpcShareMemory::m_pIpcSm = nullptr;
IpcShareMemory::IpcShareMemory(/* args */)
{
    m_key = 0;
    m_shmId = 0;
    m_pShareMemory = nullptr;
    Initialize();
    
}

IpcShareMemory::~IpcShareMemory()
{
    Release();
}

IpcShareMemory *IpcShareMemory::Instance()
{
    if(m_pIpcSm ==  nullptr)
    {
        m_pIpcSm = new IpcShareMemory();
    }
    return m_pIpcSm;

}

bool IpcShareMemory::Initialize()
{
    m_key = ftok("/home",'5');
    m_shmId = shmget(m_key, sizeof(ShareMemory), 0666|IPC_CREAT);
    if(m_shmId < 0)
    {   
        return false;
    }   

    m_pShareMemory = shmat(m_shmId, 0, 0);
    if(m_pShareMemory == nullptr)
    {
        return false;
    }
    memset(m_pShareMemory,0,sizeof(ShareMemory));
    return true;
}

int  IpcShareMemory::Write(void *pBuffer,int length)
{
    ShareMemory *pShareMemeory = (ShareMemory*)m_pShareMemory;
    if(!pShareMemeory->isUpdated)
    {
        if(pBuffer == nullptr || length > static_cast<int>(sizeof(pShareMemeory->buffer)))
        {
            return -1;
        }
        memcpy(pShareMemeory->buffer,pBuffer,length);
        pShareMemeory->size = length/sizeof(SM_Point_XYZI);
        pShareMemeory->isUpdated = true;
        return length;
    }
    return 0;
}


int  IpcShareMemory::Read(void *pBuffer,int length)
{
    ShareMemory *pShareMemeory = (ShareMemory*)m_pShareMemory;
    if(pShareMemeory->isUpdated)
    {
        int smLength = pShareMemeory->size * sizeof(SM_Point_XYZI);
        if(pBuffer == nullptr)
        {
            return -1;
        }
        int readLen = 0;
        if(smLength >= length)
        {
            memcpy(pBuffer,pShareMemeory->buffer,length);
            readLen = length;
        }
        else
        {
            memcpy(pBuffer,pShareMemeory->buffer,smLength);
            readLen = smLength;
        }
        
        memset(pShareMemeory->buffer,0,smLength);
        pShareMemeory->size = 0;
        pShareMemeory->isUpdated = false;
        return readLen;
    }
    return 0;
}


/*
int  IpcShareMemory::Read(vector<PointXYZI> &points)
{
    
    ShareMemory *pShareMemeory = (ShareMemory*)m_pShareMemory;
    points.clear();
    if(pShareMemeory->isUpdated)
    {
        int smLength = pShareMemeory->dataLength;
        int pointLen = 16;

        PointXYZI point;
        for(int i = 0;i<smLength;)
        {
           
            memcpy(point.data_c，pShareMemeory->buffer[i],pointLen);
            points.push_back(point);
            i+=pointLen;
        }
        
       
        
       
        
        
       
       // memset(pShareMemeory->buffer,0,smLength);

        //for(int i = 0;i<pShareMemeory->size;i++)
        {
          // points.push_back(pShareMemeory->buffer[i]);
        }
        
       // int size = pShareMemeory->size;
       // pShareMemeory->size = 0;
        //pShareMemeory->isUpdated = false;
        return 0;
    }
    
    return 0;

}
*/



bool IpcShareMemory::Release()
{
   // shmdt(m_pShareMemory) != -1;
    //shmctl(m_shmId, IPC_RMID, 0) != -1;

    shmdt(m_pShareMemory);
   // shmctl(m_shmId, IPC_RMID, 0);
    return true;
}






IpcCom *IpcCom::m_pIpcCom =  nullptr;
IpcCom::IpcCom(/* args */)
{
    m_key = 0;
    m_shmId = 0;
    m_pShareMemory = nullptr;
    Initialize();
}

IpcCom::~IpcCom()
{
}

IpcCom *IpcCom::Instance()
{
    if(m_pIpcCom ==  nullptr)
    {
        m_pIpcCom = new IpcCom();
    }
    return m_pIpcCom;

}
bool IpcCom::Initialize()
{
    m_key = ftok("/home",'2');
    m_shmId = shmget(m_key, sizeof(IpcMsg), 0666|IPC_CREAT);
    if(m_shmId < 0)
    {   
        return false;
    }   

    m_pShareMemory = shmat(m_shmId, 0, 0);
    if(m_pShareMemory == nullptr)
    {
        return false;
    }
    memset(m_pShareMemory,0,sizeof(IpcMsg));
    return true;
}
bool IpcCom::Release()
{
    shmdt(m_pShareMemory);
   // shmctl(m_shmId, IPC_RMID, 0);
    return true;
}

int  IpcCom::Read(IpcMsg &msg)
{
	IpcMsg *pShareMemeory = (IpcMsg*)m_pShareMemory;
    if(pShareMemeory->isUpdated)
    {
        int smLength = sizeof(IpcMsg);
        memcpy(&msg,pShareMemeory,smLength);
        pShareMemeory->isUpdated = false;
        return smLength;
    }
    return 0;
}

int  IpcCom::Write(IpcMsg msg)
{
	IpcMsg *pShareMemeory = (IpcMsg*)m_pShareMemory;
    if(!pShareMemeory->isUpdated)
    { 
        memcpy(pShareMemeory,&msg,sizeof(IpcMsg));
        pShareMemeory->isUpdated = true;
        return sizeof(IpcMsg);
    }
    return 0;
}

