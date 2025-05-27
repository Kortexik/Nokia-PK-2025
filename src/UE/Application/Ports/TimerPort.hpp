#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue
{

class TimerPort : public ITimerPort
{
 public:
    TimerPort(common::ILogger &logger);

    void start(ITimerEventsHandler &handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void stopTimer() override;

 private:
    common::PrefixedLogger logger;
    ITimerEventsHandler *handler = nullptr;

    std::chrono::steady_clock::time_point startTime;
    std::atomic<bool> timerIsRunning{false};
    bool shouldStop{false};
    std::thread timerThread;

    std::mutex mtx;
    std::condition_variable cv;
};

} // namespace ue