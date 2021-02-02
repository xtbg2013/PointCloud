#include "mysleep.h"

MySleep::MySleep()
{

}

MySleep::~MySleep()
{

}

//睡眠等待指定 ms，中途可被唤醒
void MySleep::WaitByMs(int iMs)
{
    m_isNeedWake = false;
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_waitCond.wait_for(lock, std::chrono::milliseconds(iMs), [this]{return m_isNeedWake;});
}

void MySleep::WaitByMs()
{
    m_isNeedWake = false;
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_waitCond.wait(lock, [this]{return m_isNeedWake;});
}

void MySleep::WakeUp()
{
    m_isNeedWake = true;
    m_waitCond.notify_one();
}

