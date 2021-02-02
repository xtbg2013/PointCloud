#ifndef MYSLEEP_H
#define MYSLEEP_H
#include <condition_variable>
#include <mutex>
#include <chrono>

class MySleep
{
public:
    MySleep();
    ~MySleep();

    void WaitByMs();
    void WaitByMs(int iMs);
    void WakeUp();

private:
    std::condition_variable m_waitCond;
    std::mutex m_Mutex;
    bool m_isNeedWake = false;
};
#endif // MYSLEEP_H
