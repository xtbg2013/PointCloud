#ifndef TRANSMSGSTRUCT_H
#define TRANSMSGSTRUCT_H
#include <cstdint>
#include <string>

#define MSG_DATA_NAME_LEN_MAX 256
#define MSG_PACKET_LEN_MAX 3 * 1024 * 1024 //3M

#define CMD_MAX_LEN 128

struct Point_XYZI
{
    float x;
    float y;
    float z;
    float intensity;
};

struct LidarDataStruct
{
    Point_XYZI *m_Point_XYZ;
    int recFrameTotal;
};


struct TransMsg
{
    char cMsgDataName[MSG_DATA_NAME_LEN_MAX];
    uint64_t uiDataLen;
    int packetSize;
    int packetIndex;
    char msgdata_buf[0];
};

struct COMMCmdMsg
{
    int  cmd;
};

enum PCIE_CMD
{
    PRESTART_TRANS_MSG = 0x80001,
    PRESTART_TRANS_RECV_MSG,
    END_PCIE_TARNSMSG
};

TransMsg* CreateOneTransMsg(std::string strMsgName, uint64_t uiDataLen,int packetSize, int packetIndex, void *ptrDataBuffHead);

#endif // TRANSMSGSTRUCT_H
