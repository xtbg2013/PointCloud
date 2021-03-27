#include "StatusManager.h"


StatusManager *StatusManager::m_pInstance =  nullptr;

StatusManager::StatusManager(/* args */)
{
    m_rendringControl = RenderingControl::Unknow;
}

StatusManager::~StatusManager()
{
}
 

StatusManager *StatusManager::Instance()
{
    if(m_pInstance == nullptr)
        m_pInstance = new StatusManager();
    return m_pInstance;

}

RenderingControl StatusManager::GetRendringControl()
{
    return m_rendringControl;
}
bool StatusManager::CallMFD(const MFD_Page &page)
{
    static int renderingCount = 1;
    if(page.page == LASER_PAGE)
    {
       
        m_rendringControl = static_cast<RenderingControl>((renderingCount) % (LASER_RENDERING_TYPE_COUNT +1));
        if(renderingCount == LASER_RENDERING_TYPE_COUNT)
        {
            renderingCount = 1;
        }
        else
        {
            renderingCount++;
        }
        
        return true;
    }
    return false;
}

bool StatusManager::ProcessMFD(const MFD_Control &control)
{
    bool result = false;
    switch (control.validMfd.keyValid)
    {
    case 0:  //N/A
        break;
    case 1:  //MFD3有效
        result = CallMFD(control.mfd3);
        break;
    case 2:  //MFD4有效
        result = CallMFD(control.mfd4);
        break;
    default:
        break;
    }
    return result;

}