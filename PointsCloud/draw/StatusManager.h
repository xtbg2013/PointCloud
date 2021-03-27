#pragma once
#include "../protocol/DataDefine.h"
#define LASER_PAGE 4 //激光雷达画面
#define LASER_RENDERING_TYPE_COUNT 3  //渲染类型数量   
enum class RenderingControl
{
    Unknow,
    ByAltitude,
    ByTargetType,
    ByThreatLevel
};

enum class WorkMode
{
    Unknow,
    ByAltitude,
    ByTargetType,
    ByThreatLevel
};

class StatusManager
{
private:
    RenderingControl m_rendringControl;
    static StatusManager *m_pInstance;
protected:
    bool CallMFD(const MFD_Page &page);
public:
    StatusManager(/* args */);
    ~StatusManager();
    static StatusManager *Instance();
    RenderingControl GetRendringControl();
    bool ProcessMFD(const MFD_Control &control);

    

};


