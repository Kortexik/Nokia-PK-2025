#include "Logger.hpp"
#include <thread>
#include <utility>
#include <sstream>

namespace common
{

Logger::Logger(std::ostream& logfile)
    : Logger(
        {
            {"[DEBUG]", {&std::cout, &logfile}},
            {"", {&std::cout, &logfile}},
            {"[ERROR]", {&std::cerr, &logfile}}
        })
{
    static_assert(DEBUG_LEVEL == 0, "In this constructor DEBUG is assumed to be 0");
    static_assert(INFO_LEVEL == 1, "In this constructor INFO is assumed to be 1");
    static_assert(ERROR_LEVEL == 2, "In this constructor ERROR is assumed to be 2");
}

Logger::Logger(std::initializer_list<LevelInfo> streamsForLevels)
    : streamsForLevels(streamsForLevels)
{}

Logger::~Logger()
{}

void Logger::log(Level level, const std::string &message)
{
    auto& levelInfo = streamsForLevels.at(level);
    auto number = ++printoutNumber;
    auto thisThreadId = std::this_thread::get_id();


    std::ostringstream ostr;
    ostr << "#" << number
         << ",tid:" << thisThreadId
         << levelInfo.prefix
         << ":" << message;
    auto messageStr = std::move(ostr).str();

    std::unique_lock<std::mutex> lock(printoutGuard);
    for (auto& stream: levelInfo.streams)
    {
        *stream << messageStr << std::endl;
    }
}

} // namespace ue
