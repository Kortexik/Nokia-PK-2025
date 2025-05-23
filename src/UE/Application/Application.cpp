#include "Application.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Ports/IUserPort.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer)
    : context{iLogger, bts, user, timer},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleCallRequest(common::PhoneNumber from)
{
    context.state->handleCallRequest(from);
}

void Application::handleCallAccepted(common::PhoneNumber from)
{
    context.state->handleCallAccepted(from);
}

void Application::handleCallDropped(common::PhoneNumber from)
{
    context.state->handleCallDropped(from);    
}

void Application::handleAccept()
{
    context.state->handleAccept();
}

void Application::handleReject()
{
    context.state->handleReject();
}

void Application::handleReceivedCallTalk(const std::string& text)
{
    context.state->handleReceivedCallTalk(text);
}

void Application::handleSendCallTalk(common::PhoneNumber to, const std::string &msg)
{
    context.state->handleSendCallTalk(to, msg);
}

void Application::handleSendCallDropped(common::PhoneNumber from){
    context.state->handleSendCallDropped(from);
}

void Application::handleDial(common::PhoneNumber to)
{
    context.state->handleDial(to);
}

}
