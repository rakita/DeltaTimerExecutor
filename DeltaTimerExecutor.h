#ifndef DELTATIMEREXECUTOR_H
#define DELTATIMEREXECUTOR_H

#include <condition_variable>
#include <thread>
#include <chrono>
#include <list>
#include <string>
#include <functional>
#include <cmath>

namespace dr
{

class TDeltaTimerExecutor
{
public:
    typedef void (*runFunction)();
    typedef void (*TLogger)(int,const std::string&);
    using duration = std::chrono::duration<int, std::ratio<1, 100000000>>;
    using timePoint = std::chrono::time_point<std::chrono::system_clock,std::chrono::microseconds>;

    TDeltaTimerExecutor();
    ~TDeltaTimerExecutor();

    size_t Enque(duration _delay, runFunction _func);
    size_t Enque(timePoint _timePoint, runFunction _func);

    void Remove(size_t _ID);

    void Start();
    void Stop();
    void ReschedulingRestart() {}

    void RegLogFunction(TLogger _Logger) { log=_Logger;}

private:
    void run();
    struct SItem
    {
        size_t id;
        runFunction func;
        duration delta;
    };
    TLogger log;
    std::mutex m_mtx;
    std::thread* m_pRunnable;
    std::condition_variable m_cv;
    std::mutex m_cvMtx;
    std::list<SItem> m_Queue;
    bool m_Running;
};

}
#endif // DELTATIMEREXECUTOR_H
