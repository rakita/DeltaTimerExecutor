#include <iostream>
#include "DeltaTimerExecutor.h"

using namespace std;


//test1 start stop!
void test1()
{
    dr::TDeltaTimerExecutor dte;
    dte.RegLogFunction([](int _level, const std::string& _msg) { std::cout<<"["<<_level<<"] "<<_msg<<std::endl;});
    dte.Start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    dte.Stop();
}


int main()
{
    test1();
    cout << "Hello World!" << endl;
    return 0;
}
