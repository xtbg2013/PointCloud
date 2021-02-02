#include "pcietransprocess.h"
#include "pciedevice.h"
#include <thread>
#include <string.h>
#include <iostream>
#include <fstream>


PcieTransProcess::PcieTransProcess()
{

}

PcieTransProcess * PcieTransProcess::Instance()
{
    static PcieTransProcess pcieTransProcess;
    return &pcieTransProcess;
}

bool PcieTransProcess::OnInit()
{
    bool Ret = false;

    Ret = PcieDevice::Instance()->PcieOpen();
    if(Ret)
    {
        PcieRecvProcessThread();
    }

    return Ret;
}

Point_XYZI * PcieTransProcess::GetLidarData(int & frameSize)
{
	Point_XYZI * ptrPointDataTemp;

    m_lidarDataSleep.WaitByMs();
	m_regRWLock.ReadByAtomic();	
	frameSize = m_recFrameTotal;
	ptrPointDataTemp = m_cloudPointAry;
    //ptrPointDataTemp = m_cloudPointSave;
	m_regRWLock.ReleaseAtomic();

    return ptrPointDataTemp;
}

void PcieTransProcess::SetLidarData()
{
   // m_regRWLock.WriteByAtomic();
    //memcpy(m_cloudPointSave, m_cloudPointAry, sizeof(Point_XYZI) * m_recFrameTotal);
    //m_regRWLock.ReleaseAtomic();

    m_lidarDataSleep.WakeUp();
}

void PcieTransProcess::OnRecvPcieData()
{
    int readLen = 0;
    int packetNum = 0;
    TransMsg transferMsg;

    memset(m_pcieRxBuf, 0x0, PCIE_COMM_BUF_MAX);

    readLen = PcieDevice::Instance()->PcieRead(m_pcieRxBuf, sizeof(int));
    std::cout<<"receive data from pcie datalen = " <<readLen<<std::endl;

    if(readLen > 0)
    {
        memcpy(&transferMsg, m_pcieRxBuf, MSG_DATA_NAME_LEN_MAX + sizeof(uint64_t) + sizeof(int) * 2);
        if((strcmp(PCIE_LIDAR_RAW_DATA_TAG, transferMsg.cMsgDataName) == 0) )
        {      
            if(transferMsg.packetIndex == 1)
            {
                memset(m_cloudPointAry, 0, sizeof(Point_XYZI) * MAX_LIDAR_POINT_NUM);
                m_recFrameTotal = 0;
            }

            packetNum = transferMsg.packetSize;
            if(transferMsg.packetIndex < packetNum)
            {
                m_recFrameTotal += transferMsg.uiDataLen / sizeof(Point_XYZI);
                memcpy(m_cloudPointAry + (transferMsg.packetIndex - 1) * MAX_TRANS_POINT_NUM, m_pcieRxBuf + MSG_DATA_NAME_LEN_MAX + sizeof(uint64_t) + sizeof(int) * 2, transferMsg.uiDataLen);
            }
            else
            {
                m_recFrameTotal += transferMsg.uiDataLen / sizeof(Point_XYZI);
                std::cout<<"receive LIDAR_RAW_DATA FrameTotalen = " <<m_recFrameTotal<<std::endl;
                memcpy(m_cloudPointAry + (packetNum - 1) * MAX_TRANS_POINT_NUM, m_pcieRxBuf + MSG_DATA_NAME_LEN_MAX + sizeof(uint64_t) + sizeof(int) * 2, transferMsg.uiDataLen);         
                SetLidarData();
            }
        }
        else
        {
            std::cout<<"Pcie receive data type or length not right"<<transferMsg.cMsgDataName<<std::endl;
        }
    }
}

void PcieTransProcess::PcieRecvProcessFun()
{
    int ret = 0;
    fd_set rfds;

    int iHandleId = PcieDevice::Instance()->GetPcieFd();
    struct timeval waitTime;

    while(true)
    {
        FD_ZERO(&rfds);
        FD_SET(iHandleId, &rfds);

        waitTime.tv_sec = 0;
        waitTime.tv_usec = 500 * 1000; //等待的超时时间暂定为500ms
        ret = select(iHandleId + 1, &rfds, NULL, NULL, &waitTime);
        if(ret < 0)
        {
            std::cout<<"pcie select error: "<<std::endl;
            m_recFrameTotal = 0;
            continue;
        }
        else if(ret == 0)
        {
            std::cout<<"pcie select timeout:"<<std::endl;
            m_recFrameTotal = 0;
            continue;
        }

        if(FD_ISSET(iHandleId, &rfds))
            OnRecvPcieData();
    }
}

void PcieTransProcess::PcieRecvProcessThread()
{
    std::thread PcieRevProcessThread(&PcieTransProcess::PcieRecvProcessFun, this);
    PcieRevProcessThread.detach();
}
