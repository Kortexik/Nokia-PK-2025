#include "TimerPort.hpp"

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

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

    startTime = std::chrono::steady_clock::now();
    timerIsRunning = true;
    logger.logDebug("Start timer: ", duration.count(), "ms");
    timerThread = std::thread([this, duration]() {
        std::this_thread::sleep_for(duration);
        if (timerIsRunning && handler) {
            logger.logInfo("Timer expired, calling handleTimeout");
            handler->handleTimeout();
        }
    });
    timerThread.detach();
}

void TimerPort::stopTimer()
{
    logger.logDebug("Stop timer");
    timerIsRunning = false;
}

}
