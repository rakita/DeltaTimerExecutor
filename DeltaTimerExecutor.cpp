#include "DeltaTimerExecutor.h"
#include <iostream>
#include <iomanip>
#include "date.h"

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
    TimePoint time= chrono::system_clock::now() + _delay;
    return Enque(time,_func);
}

size_t TDeltaTimerExecutor::Enque(TimePoint _timePoint, runFunction _func)
{
    std::unique_lock<std::mutex> lk(m_mtx);
    SItem newItem;
    newItem.func=_func;
    newItem.id=++s_IdPool;
    newItem.time=_timePoint;
    bool isInserted=false;
    bool isInsertedFirst=false;

    for(std::list<SItem>::iterator it=m_Queue.begin(); it != m_Queue.end(); ++it)
    {
        if((*it).time>=_timePoint)
        {
            isInserted=true;
            isInsertedFirst=it==m_Queue.begin();
            m_Queue.insert(it,newItem);
            break;
        }
    }
    if(!isInserted)
    {
        isInsertedFirst=m_Queue.empty();
        m_Queue.push_back(newItem);
        m_StartTime=std::chrono::system_clock::now();
    }
    if(isInsertedFirst) m_cv.notify_all();
    return newItem.id;
}

void TDeltaTimerExecutor::Remove(size_t _ID)
{
    std::unique_lock<std::mutex> lk(m_mtx);
    for(auto it=m_Queue.cbegin(); it != m_Queue.cend();++it)
    {
        if(it->id==_ID)
        {
            m_Queue.erase(it);
            break;
        }
    }
}


void TDeltaTimerExecutor::dPrintDeltaList()
{
    stringstream ss;
    ss<<"Delta list items:"<<endl;
    for(SItem item : m_Queue)
    {
        ss<<(string)item<<endl;
    }
    log(5,ss.str());
}

void TDeltaTimerExecutor::run()
{
    log(3,"Thread is running");
    TimePoint time;
    while(m_Running)
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        //log(4,"Waiting for event");

        if(m_Queue.empty()) time=chrono::system_clock::now()+chrono::hours(1);
        else time = m_Queue.front().time;

        //log(4,"Wait until:" + date::format("%F %T", time));
        m_cv.wait_until(lk,time);

        if(m_Queue.empty()) continue;
        SItem& item = m_Queue.front();
        if(item.time<= chrono::system_clock::now())
        {
            log(5,"Executing item:" + (std::string)item);
            (*(item.func))();
            m_Queue.pop_front();
        }
        //dPrintDeltaList();
    }
}

}
