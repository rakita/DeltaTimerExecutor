#include "DeltaTimerExecutor.h"
#include <iostream>

namespace dr
{

using namespace std;

TDeltaTimerExecutor::TDeltaTimerExecutor() :
    m_pRunnable(NULL)
{
}

TDeltaTimerExecutor::~TDeltaTimerExecutor()
{
    m_pRunnable->join();
    delete m_pRunnable;
}

void TDeltaTimerExecutor::Start()
{
    m_pRunnable = new std::thread(&TDeltaTimerExecutor::run,this);
}

size_t TDeltaTimerExecutor::Enque(duration _delay, runFunction _func)
{
    m_mtx.lock();
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
    cout<<"Run started"<<endl;
}

}
