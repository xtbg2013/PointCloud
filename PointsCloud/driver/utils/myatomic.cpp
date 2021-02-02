#include "myatomic.h"

#define SHARED_DATA_IDLE     (0)
#define SHARED_DATA_READING  (1)
#define SHARED_DATA_WRITING  (2)

MyAtomic::MyAtomic()
{
    std::atomic_init(&m_iAccessState, (unsigned char)SHARED_DATA_IDLE);
    //m_iAccessState.store(SHARED_DATA_IDLE);
}

//使用原子锁进行读操作
void MyAtomic::ReadByAtomic()
{
    uint8_t expDataState = SHARED_DATA_IDLE;
    while(!m_iAccessState.compare_exchange_weak(expDataState, SHARED_DATA_READING))
        expDataState = SHARED_DATA_IDLE;
    //m_iAccessMutex.lock();
}

//使用原子锁进行写操作
void MyAtomic::WriteByAtomic()
{
    uint8_t expDataState = SHARED_DATA_IDLE;
    while(!m_iAccessState.compare_exchange_weak(expDataState, SHARED_DATA_WRITING))
        expDataState = SHARED_DATA_IDLE;
    //m_iAccessMutex.lock();
}

//读或写操作完成后，释放原子锁
void MyAtomic::ReleaseAtomic()
{
    //m_iAccessMutex.unlock();
    m_iAccessState.store(SHARED_DATA_IDLE);
}
