#ifndef DELTATIMEREXECUTOR_H
#define DELTATIMEREXECUTOR_H

#include <condition_variable>
#include <thread>
#include <chrono>
#include <list>
#include <string>
#include <functional>
#include <cmath>
#include "date.h"

namespace dr
{

using namespace std;
using namespace chrono;

class TDeltaTimerExecutor
{
public:
    typedef void (*runFunction)();
    typedef void (*TLogger)(int,const std::string&);
    using Delta = std::chrono::milliseconds;
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    TDeltaTimerExecutor();
    ~TDeltaTimerExecutor();

    size_t Enque(Delta _delay, runFunction _func);
    size_t Enque(TimePoint _timePoint, runFunction _func);

    void Remove(size_t _ID);

    void Start();
    void Stop();
    void ReschedulingRestart() {}

    void RegLogFunction(TLogger _Logger) { log=_Logger;}

private:
    void dPrintDeltaList();
    void run();
    struct SItem
    {
        size_t id;
        runFunction func;
        TimePoint time;
        std::string delta() { return date::format("%T", time- system_clock::now());}
        operator std::string() const { return "id:" + std::to_string(id) +
                    " delta:" + date::format("%T",time - system_clock::now())
                    + " Time[" + date::format("%F %T", time) + "]";}
    };
    TLogger log;
    std::mutex m_mtx;
    std::thread* m_pRunnable;
    std::condition_variable m_cv;
    std::mutex m_cvMtx;
    std::list<SItem> m_Queue;
    bool m_Running;
    static size_t s_IdPool;
    TimePoint m_StartTime;
};

}
#endif // DELTATIMEREXECUTOR_H
