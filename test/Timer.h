//
// Created by adam on 02.01.16.
//

#ifndef DIVIDE_CONQUER_FRAMEWORK_TIMER_H
#define DIVIDE_CONQUER_FRAMEWORK_TIMER_H


#include <chrono>
#include <string>

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

class AverageTimer {
    typedef std::chrono::steady_clock clock;
public:
    AverageTimer();
    void reset();

    void start();

    void stop();

    int getNumTimes();

    string getAverageDurationString();

    clock::duration getAverageDuration();

private:
    Timer timer;
    clock::duration totalDuration;
    int numTimes;
};

#endif //DIVIDE_CONQUER_FRAMEWORK_TIMER_H
