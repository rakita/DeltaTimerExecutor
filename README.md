# DeltaTimerExecutor
Simple delta timer class with function:
* Start
* Stop
* Enque
* Remove

This is Hobby project where i wanted to discover newer C++ and implemente something litle compex.

I was supprised how easy was to implement this using C++11 and title negativly how chrono is confusing and not supported as it should ( But there is data library(one .h file) from Howard Hinnant that came to my rescue)

Core mechanism is using condition_variable with try_wait.
I made litle test that stress tested it with 1000 timers that are randomli enque-ed in range of 0-100ms, error that i got on my i5-4590 machine is in range of max 1.5ms.
This is not incremental error becase conditional_value is using time_point for waiting and it will correct it self on every new "delta".
