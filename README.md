# DeltaTimerExecutor
Simple delta timer class with function:
* Start
* Stop
* Enque
* Remove

This is a hobby project where I wanted to discover newer C++ and implement something little complex.

I was surprised how easy was to implement this using C++11 and little negatively surprised how Chrono is confusing and feels like not supported as it should be ( But there is date library(one .h file) from Howard Hinnant that came to my rescue)

The core mechanism is using condition_variable with try_wait.
I made a little test that and stress tested it with 1000 timers that are randomly enqueued in range of 0-100ms, an error of time execution that I got on my i5-4590 machine is in range of max 1.5ms.
This is not incremental error because conditional_value is using time_point for waiting and it will correct itself on every new "delta".
