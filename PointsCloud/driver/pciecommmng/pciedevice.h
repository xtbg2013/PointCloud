#ifndef PCIEDEVICE_H
#define PCIEDEVICE_H

#include <cstdint>

#define PCIE_DEVICE_NAME "/dev/fpga_pcie"
#define ONE_PACKET 4 * 1024 * 1024

class PcieDevice
{
public:
    PcieDevice();
    static PcieDevice * Instance();
    bool PcieOpen();
    uint64_t PcieWrite(void* srcAddr, uint64_t len);
    uint64_t PcieRead(void* DstAddr, uint64_t len);
    int GetPcieFd();

private:
    int m_pcieFd = -1;
};

#endif // PCIEDEVICE_H
