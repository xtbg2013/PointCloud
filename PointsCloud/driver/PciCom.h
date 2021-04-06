#pragma once
#include "../protocol/DataDefine.h"
//#define __SL
#ifdef __SL
#include "pciecommmng/pcietransprocess.h"
#else
#include "inc/PciePublish.h"
#endif
class PciCom
{
public: 
    Point_XYZI m_points[MAX_POINTS_OF_PCID];
public:
    PciCom();
    virtual ~PciCom();
    virtual bool Initialize();
    virtual void Release();
    virtual Point_XYZI*  Read(int &size);
    virtual int  Write(char *pBuffer,int length);
};