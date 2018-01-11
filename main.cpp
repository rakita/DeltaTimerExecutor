#include <iostream>
#include <iomanip>
#include "DeltaTimerExecutor.h"

using namespace std;
using namespace chrono;

void func()
{
    cout<<" item is executed"<<endl;
}

//test1 start stop!
void test1()
{
    dr::TDeltaTimerExecutor dte;
    dte.RegLogFunction([](int _level, const std::string& _msg) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout<<"["<<std::put_time(std::localtime(&now_c), "%c")<<"]["<<_level<<"] "<<_msg<<std::endl;
    });
    dte.Start();

    //dr::TDeltaTimerExecutor::TimePoint timepoint = system_clock::now()+seconds(6);
    dte.Enque(seconds(4),&func);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    dte.Stop();
}


int main()
{
    test1();
    cout << "Hello World!" << endl;
    return 0;
}
