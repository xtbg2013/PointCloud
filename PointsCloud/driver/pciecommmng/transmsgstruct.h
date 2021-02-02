#ifndef TRANSMSGSTRUCT_H
#define TRANSMSGSTRUCT_H
#include <cstdint>
#include <string>

#define MSG_DATA_NAME_LEN_MAX 256
#define MSG_PACKET_LEN_MAX 3 * 1024 * 1024 //3M

#define PCIE_LIDAR_RAW_DATA_TAG "LIDAR_RAW_DATA"

struct Point_XYZI
{
    float x;
    float y;
    float z;
    float intensity;
};

struct TransMsg
{
    char cMsgDataName[MSG_DATA_NAME_LEN_MAX];
    uint64_t uiDataLen;
    int packetSize;
    int packetIndex;
    void * ptrMsgDataBuff;
};

TransMsg CreateOneTransMsg(std::string strMsgName, uint64_t uiDataLen, void *ptrDataBuffHead);

#endif // TRANSMSGSTRUCT_H
