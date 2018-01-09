#include <iostream>
#include "DeltaTimerExecutor.h"

using namespace std;

//test1 start stop!
void test1()
{
    dr::TDeltaTimerExecutor dte;
    dte.Start();
    dte.Stop();
}


int main()
{
    test1();
    cout << "Hello World!" << endl;
    return 0;
}
