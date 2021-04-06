#ifndef PCIEPUBLISH_H
#define PCIEPUBLISH_H
#include "transmsgstruct.h"

void Init();
Point_XYZI * GetLidarData(int &frameSize);
//void GetLidarData(LidarDataStruct &lidardata);

#endif
