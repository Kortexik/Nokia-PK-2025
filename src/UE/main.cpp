#include "Application.hpp"
#include "ApplicationEnvironmentFactory.hpp"
#include "Ports/BtsPort.hpp"
#include "Ports/UserPort.hpp"
#include "Ports/TimerPort.hpp"
#include "Application/ISmsDb.hpp"

int main(int argc, char* argv[])
{
    using namespace ue;
    using namespace std::chrono_literals;

    auto appEnv = ue::createApplicationEnvironment(argc, argv);
    auto& logger = appEnv->getLogger();
    auto& tranport = appEnv->getTransportToBts();
    auto& gui = appEnv->getUeGui();
    auto phoneNumber = appEnv->getMyPhoneNumber();

    BtsPort bts(logger, tranport, phoneNumber);
    UserPort user(logger, gui, phoneNumber);
    TimerPort timer(logger);
    SmsDb smsDb;
    Application app(phoneNumber, logger, bts, user, timer,smsDb);
    bts.start(app);
    user.start(app);
    timer.start(app);
    appEnv->startMessageLoop();
    bts.stop();
    user.stop();
    timer.stop();
}

