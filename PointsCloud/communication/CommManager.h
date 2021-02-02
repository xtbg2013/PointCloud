#pragma once
#include "ICom.h"
class CommManager
{
private:
    ICom *m_pMCComHighSpeed;
    ICom *m_pMCComLowSpeed;   //与智能计算机通信：配置等低速通信
    ICom *m_pLaserCom;        //与激光雷达通信
    ICom *m_pSPCom;           //与信号处理单元通信
public:
    CommManager(ICom *pMCComHighSpeed,ICom *pMCComLowSpeed,ICom *pLaserCom,ICom *pSPCom);
    ~CommManager();
};

