#include "SmsListState.hpp"
#include "ConnectedState.hpp"
#include "ViewSmsContentState.hpp"

namespace ue
{
    SmsListState::SmsListState(Context &context) : ConnectedState(context)
    {
        context.user.displaySmsList(context.smsDb.getAllSms());
        context.user.showNewSms(context.smsDb.getUnreadCount() > 0);
    }

    void SmsListState::handleReject()
    {
        logger.logInfo("Back to ConnectedState from SmsListState");
        context.setState<ConnectedState>();
    }

    void SmsListState::handleMenuSelection(unsigned int index)
    {
        std::vector<Sms>& messages = context.smsDb.getAllSms();
        int messageIndex = messages.size() - index - 1;

        if (messageIndex < 0)
        {
            logger.logError("Incorrect SMS index");
            return;
        }

        Sms& sms = messages[messageIndex];
        if (sms.getDirection() == Sms::Direction::IN && sms.getStatus() == Sms::Status::UNREAD)
        {
            logger.logInfo("SMS of index " + std::to_string(messageIndex) + " status set to READ from UNREAD");
            sms.setStatus(Sms::Status::READ);
        }

        context.setState<ViewSmsContentState>(sms);
    }
}