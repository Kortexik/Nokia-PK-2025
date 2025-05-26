#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerDisconnectedCallback([this](){handleDisconnected();});
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0U;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            std::string message = reader.readRemainingText();
            handler->handleSmsReceived(from, message);
            break;
        }

        case common::MessageId::CallRequest:
        {
          handler->handleCallRequest(from);
          break;
        }
        case common::MessageId::CallAccepted:
        {
          handler->handleCallAccepted(from);
          break;
        }
        case common::MessageId::CallDropped:
        {
          handler->handleCallDropped(from);
          break;
        }
        case common::MessageId::CallTalk:
        {
          std::string text = reader.readRemainingText();
          handler->handleReceivedCallTalk(text);
          break;
        }

        default:
            logger.logError("unknown message: ", msgId, ", from: ", from);

        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}


void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::handleDisconnected() const {
    handler->handleDisconnected();
}

void BtsPort::sendCallAccepted(common::PhoneNumber to) {
  logger.logDebug("sendCallAccept to: ", to);
  common::OutgoingMessage msg(common::MessageId::CallAccepted, phoneNumber, to);
  transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallDrop(common::PhoneNumber to) {
  logger.logDebug("sendCallDrop to: ", to);
  common::OutgoingMessage msg(common::MessageId::CallDropped, phoneNumber, to);
  transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber to)
{
    logger.logDebug("sendCallRequest to: ", to);
    common::OutgoingMessage msg(common::MessageId::CallRequest, phoneNumber, to);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallTalk(common::PhoneNumber to, const std::string &text) {
    logger.logInfo("Number " + to_string(to) + ":  " + text);
    common::OutgoingMessage msg{common::MessageId::CallTalk,phoneNumber,to};
    msg.writeText(text);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendSmsMessage(common::PhoneNumber to, const std::string &text)
{
    logger.logInfo("Sending SMS message to: " + common::to_string(to));
    common::OutgoingMessage msg {common::MessageId::Sms,phoneNumber,to};
    msg.writeText(text);
    transport.sendMessage(msg.getMessage());
}
}
