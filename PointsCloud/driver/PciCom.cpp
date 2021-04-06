#include "PciCom.h"

PciCom::PciCom()
{
      
}

PciCom::~PciCom()
{
     
}

bool PciCom::Initialize()
{
    //return true;
    #ifdef __SL
    return PcieTransProcess::Instance()->OnInit();
    #else
    Init();
    #endif
    return true;
}

void PciCom::Release()
{
     
}

Point_XYZI*  PciCom::Read(int &size)
{
   // size = MAX_POINTS_OF_PCID;
   // return m_points;

   #ifdef __SL
    return PcieTransProcess::Instance()->GetLidarData(size);
    #else
    return GetLidarData(size);

    #endif
    
}
int  PciCom::Write(char *pBuffer,int length)
{
    return 0;
}