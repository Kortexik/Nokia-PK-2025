#include "SmsComposeState.hpp"

namespace ue
{
SmsComposeState::SmsComposeState(Context &context) : ConnectedState(context)
{
    context.user.displaySmsCompose();
}

void SmsComposeState::handleAccept()
{
    logger.logInfo("Attempting to send SMS");
    Sms sms = context.user.getSmsComposeData();
    common::PhoneNumber number = sms.getPhoneNumber();
    const std::string text = sms.getText();

    if (!sms.getPhoneNumber().isValid())
    {
        logger.logError("Invalid number!");
    }

    context.smsDb.addSentSms(number, text);
    context.bts.sendSmsMessage(number, text);

    context.setState<ConnectedState>();
}

void SmsComposeState::handleReject()
{
    context.setState<ConnectedState>();
}
} // namespace ue