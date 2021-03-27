#pragma once
#include "../driver/pciecommmng/transmsgstruct.h"
class PointsProcess
{
private:
    /* data */
public:
    PointsProcess(/* args */);
    ~PointsProcess();
    static int SaveToPcd(Point_XYZI *pData,int size);
};


