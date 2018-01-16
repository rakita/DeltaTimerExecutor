#include <iostream>
#include <iomanip>
#include "DeltaTimerExecutor.h"
#include <list>

using namespace std;
using namespace chrono;

std::list<time_point<system_clock, system_clock::duration>> exeList;

void func()
{
    try{
        time_point<system_clock, system_clock::duration> tp = exeList.front();
        exeList.pop_front();
        system_clock::duration dur = system_clock::now()-tp;
        if(dur+milliseconds(1) > milliseconds(2))
        {
            cout<<"ERRRRORRRR"<<date::format("%S",dur)<<endl;
        }
    } catch(exception e)
    {
        cout<<"Exception:"<<e.what()<<endl;
    }
}

//test1 start stop!
void test1()
{
    dr::TDeltaTimerExecutor dte;
    dte.RegLogFunction([](int _level, const std::string& _msg) {
        //auto now = std::chrono::system_clock::now();
        //auto now_c = std::chrono::system_clock::to_time_t(now);
        //std::cout<<"["<<std::put_time(std::localtime(&now_c), "%c")<<"]["<<_level<<"] "<<_msg<<std::endl;
    });
    dte.Start();

    dte.Enque(seconds(3),&func);
    exeList.push_back(system_clock::now() + seconds(3));
    milliseconds dur=milliseconds(0);
    for(int i=0; i<1000; ++i)
    {
        dur += milliseconds(rand() % 100+1);
        exeList.push_back(system_clock::now() + seconds(3)+dur);
        dte.Enque(seconds(3)+dur,&func);
    }

    std::this_thread::sleep_for(seconds(30));

    dte.Stop();
}

void test2()
{
    dr::TDeltaTimerExecutor dte;
    dte.RegLogFunction([](int _level, const std::string& _msg) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout<<"["<<std::put_time(std::localtime(&now_c), "%c")<<"]["<<_level<<"] "<<_msg<<std::endl;
    });
    dte.Start();

    exeList.push_back(system_clock::now() + seconds(4));
    exeList.push_back(system_clock::now() + seconds(8));
    exeList.push_back(system_clock::now() + seconds(10));
    exeList.push_back(system_clock::now() + seconds(12));
    exeList.push_back(system_clock::now() + seconds(14));
    dte.Enque(seconds(4),&func);
    //std::this_thread::sleep_for(std::chrono::seconds(5));


    dte.Enque(seconds(10),&func);
    dte.Enque(seconds(12),&func);
    dte.Enque(seconds(14),&func);
    std::this_thread::sleep_for(std::chrono::hours(10));

    dte.Stop();
}


int main()
{
    test1();
    cout << "Hello World!" << endl;
    return 0;
}
