#include "PciCom.h"

PciCom::PciCom()
{
     
}

PciCom::~PciCom()
{
}

bool PciCom::Initialize()
{
    return true;
    return PcieTransProcess::Instance()->OnInit();
}

void PciCom::Release()
{
     
}

Point_XYZI*  PciCom::Read(int &size)
{
    size = MAX_POINTS_OF_PCID;
    return m_points;
    return PcieTransProcess::Instance()->GetLidarData(size);
}
int  PciCom::Write(char *pBuffer,int length)
{
    return 0;
}