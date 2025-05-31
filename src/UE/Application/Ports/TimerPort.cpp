#include "TimerPort.hpp"

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger) : logger(logger, "[TIMER PORT]")
{
}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stoped");
    handler = nullptr;
}

void TimerPort::startTimer(Duration duration)
{

    std::lock_guard<std::mutex> lock(mtx);
    startTime = std::chrono::steady_clock::now();
    timerIsRunning = true;
    shouldStop = false;

    logger.logDebug("Start timer: ", duration.count(), "ms");

    timerThread = std::thread([this, duration]() {
        std::unique_lock<std::mutex> lock(mtx);
        if (cv.wait_for(lock, duration, [this]() { return shouldStop; }))
        {
            logger.logDebug("Timer cancelled before expiration");
            return;
        }
        if (timerIsRunning && handler)
        {
            logger.logInfo("Timer expired, calling handleTimeout");
            handler->handleTimeout();
        }
    });

    timerThread.detach();
}

void TimerPort::stopTimer()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        timerIsRunning = false;
        shouldStop = true;
    }
    cv.notify_all();
    logger.logDebug("Stop timer");
}

} // namespace ue
