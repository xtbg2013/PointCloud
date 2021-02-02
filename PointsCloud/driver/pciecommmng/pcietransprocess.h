#ifndef PCIETRANSPROCESS_H
#define PCIETRANSPROCESS_H
#include <cstdint>
#include "transmsgstruct.h"
#include "myatomic.h"
#include "mysleep.h"

#define PCIE_COMM_BUF_MAX 4 * 1024 * 1024
#define MAX_LIDAR_POINT_NUM 512 * 1024
#define MAX_TRANS_POINT_NUM 160000

class PcieTransProcess
{
public:
    PcieTransProcess();
    static PcieTransProcess * Instance();
    bool OnInit();
    void PcieSentMsgToFpga(TransMsg &transferMsg);
    Point_XYZI *GetLidarData(int & frameSize);

private:
    void SetLidarData();
    void PcieRecvProcessThread();
    void PcieRecvProcessFun();
    void OnRecvPcieData();

private:
    uint8_t m_pcieRxBuf[PCIE_COMM_BUF_MAX] = {0x0};
    Point_XYZI m_cloudPointAry[MAX_LIDAR_POINT_NUM];
    Point_XYZI m_cloudPointSave[MAX_LIDAR_POINT_NUM];
    int m_recFrameTotal = 0;
    MyAtomic m_regRWLock;
    MySleep m_lidarDataSleep;
};














#endif // PCIETRANSPROCESS_H
