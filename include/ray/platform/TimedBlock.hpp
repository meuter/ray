#pragma once

#include <ray/platform/Stopwatch.hpp>
#include <ray/platform/Print.hpp>

#define TIMED_BLOCK(x) auto __timedBlock__ ## __COUNTER__ = TimedBlock(x, __FILE__, __LINE__, __FUNCTION__)

namespace ray { namespace platform {

    class TimedBlock
    {
    public:
        TimedBlock(const char *description, const char *file, int line, const char *function) : mDescription(description), mFile(file), mLine(line), mFunction(function) {}
        ~TimedBlock() 
        {
            fprintln("%s:%d:%s: %fms elapsed for %s", mFile, mLine, mFunction, 1000*mStopwatch.lap().count(), mDescription);
        }
    private:
        const char *mFile, *mFunction, *mDescription;
        int mLine;
        Stopwatch mStopwatch;
    };

}}
