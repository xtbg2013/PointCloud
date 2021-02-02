#ifndef MYATOMIC_H
#define MYATOMIC_H
#include <atomic>
#include <mutex>

/*
该原子锁对象，不会引起线程的调度，类似于自旋锁，
适用于数据量不大，访问时间在 ns 和 us 级别 的共享数据区
*/
class MyAtomic
{
public:
    MyAtomic();
    void ReadByAtomic();  //使用原子锁进行读操作
    void WriteByAtomic(); //使用原子锁进行写操作
    void ReleaseAtomic();  //读或写操作完成后，释放原子锁

private:
    std::atomic<uint8_t> m_iAccessState; //标识数据被访问的状态(是否正被读写中)
    //std::mutex m_iAccessMutex;
};

#endif // MYATOMIC_H
