#include "DeltaTimerExecutor.h"
#include <iostream>

namespace dr
{

//log levels/
//1 Notification


using namespace std;

size_t TDeltaTimerExecutor::s_IdPool=0;

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


size_t TDeltaTimerExecutor::Enque(Delta _delay, runFunction _func)
{
    std::unique_lock<std::mutex> lk(m_mtx);
    SItem newItem;
    newItem.func=_func;
    newItem.id=++s_IdPool;
    Delta deltaSum= chrono::seconds(0);
    bool isInserted=false;
    bool isInsertedFirst=false;
    for(std::list<SItem>::iterator it=m_Queue.begin(); it != m_Queue.end(); ++it)
    {
        deltaSum+=(*it).delta;
        if(deltaSum<_delay) continue;
        isInserted=true;
        isInsertedFirst=it==m_Queue.begin();
        Delta rescheduledDelta=deltaSum-_delay;
        newItem.delta= (*it).delta-rescheduledDelta;
        (*it).delta=rescheduledDelta;
        m_Queue.insert(it,newItem);
        if(isInsertedFirst) m_StartTime=std::chrono::system_clock::now();
        break;
    }
    if(!isInserted)
    {
        isInsertedFirst=m_Queue.empty();
        newItem.delta=_delay-deltaSum;
        m_Queue.push_back(newItem);
        if(isInsertedFirst) m_StartTime=std::chrono::system_clock::now();
    }
    if(isInsertedFirst) m_cv.notify_all();
}

size_t TDeltaTimerExecutor::Enque(TimePoint _timePoint, runFunction _func)
{
    //Enque(std::chrono::time_point()-_timePoint, _func);
}

void TDeltaTimerExecutor::Remove(size_t _ID)
{

}

void TDeltaTimerExecutor::run()
{
    log(3,"Thread is running");
    Delta sleep=Delta(Delta::max());
    while(m_Running)
    {
        {
            std::unique_lock<std::mutex> lk(m_mtx);
            log(4,"Waiting for event");

            m_cv.wait_for(lk,sleep);
            if(m_Queue.empty()) continue;
            SItem& item = m_Queue.front();
            auto timestamp = std::chrono::system_clock::now()-m_StartTime;
            Delta currentDelta = chrono::duration_cast< Delta>(timestamp);
            if(currentDelta>= item.delta)
            {
                log(5,"Executing item:" + (std::string)item);
                (*(item.func))();
                m_Queue.pop_front();
                m_StartTime=std::chrono::system_clock::now();
            }
            else
            {
                sleep=item.delta-currentDelta;
            }

        }
    }
}

}
