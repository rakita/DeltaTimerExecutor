#include "DeltaTimerExecutor.h"
#include <iostream>

namespace dr
{

//log levels/
//1 Notification


using namespace std;

TDeltaTimerExecutor::TDeltaTimerExecutor() :
    m_pRunnable(NULL),
    m_Running(false)
{
}

TDeltaTimerExecutor::~TDeltaTimerExecutor()
{
    Stop();
    log(3,"Terminating TDE object");
}

void TDeltaTimerExecutor::Start()
{
    if(!m_Running) Stop();
    log(1,"Starting DTE");
    m_Running=true;
    m_pRunnable = new std::thread(&TDeltaTimerExecutor::run,this);
}

void TDeltaTimerExecutor::Stop()
{
    if(!m_Running) return;
    log(1,"Stoping DTE");
    m_Running=false;
    m_cv.notify_all();
    m_pRunnable->join();
    delete m_pRunnable;
}


size_t TDeltaTimerExecutor::Enque(duration _delay, runFunction _func)
{
    std::unique_lock<std::mutex> lk(m_mtx);

    m_cv.notify_all();
}

size_t TDeltaTimerExecutor::Enque(timePoint _timePoint, runFunction _func)
{
    //Enque(std::chrono::time_point()-_timePoint, _func);
}

void TDeltaTimerExecutor::Remove(size_t _ID)
{

}

void TDeltaTimerExecutor::run()
{
    log(3,"Thread is running");
    while(m_Running)
    {
        {
            std::unique_lock<std::mutex> lk(m_mtx);
            log(4,"Waiting for event");
            m_cv.wait(lk);
        }
    }
}

}
