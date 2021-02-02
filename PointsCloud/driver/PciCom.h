#pragma once
#include "pciecommmng/pcietransprocess.h"
#include "../protocol/DataDefine.h"
class PciCom
{
private: 
    Point_XYZI m_points[MAX_POINTS_OF_PCID];
public:
    PciCom();
    virtual ~PciCom();
    virtual bool Initialize();
    virtual void Release();
    virtual Point_XYZI*  Read(int &size);
    virtual int  Write(char *pBuffer,int length);
};