#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

#include "pciedevice.h"

PcieDevice::PcieDevice()
{

}

PcieDevice * PcieDevice::Instance()
{
    static PcieDevice pcieDevice;
    return &pcieDevice;
}

bool PcieDevice::PcieOpen()
{
    m_pcieFd = open(PCIE_DEVICE_NAME, O_RDWR);
    if (m_pcieFd == -1)
    {
        std::cout<<"Open Pcie Device" <<PCIE_DEVICE_NAME<<"Fault";
        return false;
    }

    return true;
}

int PcieDevice::GetPcieFd()
{
    return m_pcieFd;
}

uint64_t PcieDevice::PcieWrite(void* srcAddr, uint64_t len)
{
    uint64_t numPacket = 0;
    uint64_t numPacketRest = 0;
    uint64_t numRestByte = 0;
    uint64_t Ret = 0;
    uint64_t offSet = 0;

    if (m_pcieFd == -1)
    {
        std::cout<<"Open Pcie Device" <<PCIE_DEVICE_NAME<<"Fault";
        return -1;
    }

    numPacket = len / (ONE_PACKET);
    numPacketRest = numPacket;
    while(numPacketRest)
    {
        Ret += write(m_pcieFd, (srcAddr + (numPacket - numPacketRest) * (ONE_PACKET)), (ONE_PACKET));
        offSet = lseek(m_pcieFd, offSet + (ONE_PACKET), SEEK_SET);
        numPacketRest--;
    }

    numRestByte = (len - numPacket * (ONE_PACKET)) % (ONE_PACKET);
    if (numRestByte > 0)
    {
        Ret += write(m_pcieFd, srcAddr, numRestByte);
    }

    return Ret;
}

uint64_t PcieDevice::PcieRead(void* DstAddr, uint64_t len)
{
    uint64_t Ret = 0;
    Ret = read(m_pcieFd, DstAddr, len);

    return Ret;
}
