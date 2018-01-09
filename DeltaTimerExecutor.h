#ifndef DELTATIMEREXECUTOR_H
#define DELTATIMEREXECUTOR_H

#include <condition_variable>
#include <thread>
#include <chrono>
#include <list>

namespace dr
{

class TDeltaTimerExecutor
{
public:
    typedef void (*runFunction)();
    using duration = std::chrono::duration<int, std::ratio<1, 100000000>>;
    using timePoint = std::chrono::time_point<std::chrono::system_clock,std::chrono::microseconds>;

    TDeltaTimerExecutor();
    ~TDeltaTimerExecutor();

    size_t Enque(duration _delay, runFunction _func);
    size_t Enque(timePoint _timePoint, runFunction _func);

    void Remove(size_t _ID);

    void Start();
    void Stop() {}
    void ReschedulingRestart() {}

private:
    void run();
    struct SItem
    {
        size_t id;
        runFunction func;
        duration delta;
    };
    std::mutex m_mtx;
    std::thread* m_pRunnable;
    std::condition_variable m_cv;
    std::list<SItem> m_Queue;
};

}
#endif // DELTATIMEREXECUTOR_H
