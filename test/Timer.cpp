#include "Timer.h"

Timer::Timer() : isMeasuring(false) {

}

void Timer::start() {
    if (isMeasuring) {
        throw "Timer is already running. Please stop it before rerun.";
    }
    startTime = clock::now();
    isMeasuring = true;
}


void Timer::stop() {
    clock::time_point stopTime = clock::now();
    lastDuration = stopTime - startTime;
    isMeasuring = false;
}


Timer::clock::duration Timer::getDuration() {
    return lastDuration;
}

double Timer::getMilliseconds() {
    return chrono::duration_cast<chrono::microseconds>(lastDuration).count() / 1000.0;
}


string Timer::getDurationString() {
    auto micro = chrono::duration_cast<chrono::microseconds>(lastDuration);
    return to_string(micro.count());
}

void AverageTimer::reset() {
    numTimes = 0;
    totalDuration = clock::duration::zero();
}

int AverageTimer::getNumTimes() {
    return numTimes;
}

void AverageTimer::start() {
    timer.start();
}

void AverageTimer::stop() {
    timer.stop();
    totalDuration += timer.getDuration();
    numTimes++;

}

AverageTimer::clock::duration AverageTimer::getAverageDuration() {
    return totalDuration / numTimes;
}

string AverageTimer::getAverageDurationString() {
    auto micro = chrono::duration_cast<chrono::microseconds>(getAverageDuration());
    return to_string(micro.count()) + " us";
}

AverageTimer::AverageTimer() : numTimes(0), totalDuration(clock::duration::zero()) {

}
