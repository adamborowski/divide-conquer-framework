#ifndef DIVIDE_CONQUER_FRAMEWORK_TIMER_H
#define DIVIDE_CONQUER_FRAMEWORK_TIMER_H


#include <chrono>
#include <string>
#include <vector>

using namespace std;

class Timer {
    typedef std::chrono::steady_clock clock;
public:
    Timer();

    void start();

    void stop();

    string getDurationString();

    clock::duration getDuration();

    double getMilliseconds();

private:
    clock::time_point startTime;
    clock::duration lastDuration;
    bool isMeasuring;

};

#endif //DIVIDE_CONQUER_FRAMEWORK_TIMER_H
