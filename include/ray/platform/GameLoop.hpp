#pragma once

#include <ray/platform/Stopwatch.hpp>
#include <ray/platform/Window.hpp>
#include <vector>
#include <thread>

namespace ray { namespace platform {

    using fps = uint16_t;
    constexpr fps operator""_fps ( unsigned long long int i ) { return fps(i); }
    
    class GameLoop
    {
        using u16 = uint16_t;
        using u32 = uint32_t;
        using u64 = uint64_t;
        using f32 = float;
    public:

        GameLoop(Window &window, fps targetFPS, bool vSync=true) 
            : mWindow(window), mFrameTimesSum(1_sec), mFrameCount(0), mTargetFPS(targetFPS), mLastFrameTimeIndex(0)
        {
            window.swapInterval(vSync ? 1 : 0);
            mFrameTimes.resize(targetFPS, 1_sec/mTargetFPS);            
        }

        void run(std::function<void(void)> doOneFrame)
        {
            while ( !mWindow.shouldClose() )
            {
                mStopwatch.lap();            
                {
                    mWindow.pollEvents();
                    doOneFrame();
                    mWindow.swapBuffers();        
                }
                mFrameTime = mStopwatch.lap();

                mFrameTimesSum += mFrameTime - mFrameTimes[mLastFrameTimeIndex];
                mFrameTimes[mLastFrameTimeIndex] = mFrameTime;
                mLastFrameTimeIndex = (mLastFrameTimeIndex+1) % mFrameTimes.size();
                mFrameCount += 1;
            
                auto waitTime = (1_sec/mTargetFPS) - mFrameTime;
                while(waitTime > 100_usec)
                {
                    std::this_thread::sleep_for(100_usec);
                    waitTime -= 100_usec;
                }
            }
        }        
        
        sec lastFrameTime()           const { return mFrameTime; }
        sec averageFrameTime()        const { return mFrameTimesSum / mTargetFPS; }
        sec targetFrameTime()         const { return 1_sec / mTargetFPS; }
        fps averageFramesPerSeconds() const { return fps((double)mTargetFPS / mFrameTimesSum.count()); }
        fps targetFramesPerSeconds()  const { return mTargetFPS; }
        u64 frameCount()              const { return mFrameCount; }
        
        sec dt()                      const { return targetFrameTime(); }

    private:
        Window &mWindow;
        platform::Stopwatch mStopwatch;
        sec mFrameTime, mFrameTimesSum;
        u64 mFrameCount;        
        u16 mLastFrameTimeIndex;
        fps mTargetFPS;
        std::vector<sec> mFrameTimes;
    };
}}