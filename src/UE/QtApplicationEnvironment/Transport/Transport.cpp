#include "Transport.hpp"
#include "Config/MultiLineConfig.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"
#include <QTcpSocket>
#include <QtNetwork>
#include <functional>
#include <string>

namespace ue
{

Transport::Transport(common::MultiLineConfig &configuration, common::ILogger &loggerBase)
    : logger(loggerBase, "[TRANSPORT]"), port(configuration.getNumber("port", 8181)),
      server(configuration.getString("server", "localhost")), socket(new QTcpSocket())
{
    logger.logDebug("Selected configuration ", server, ":", port);

    QNetworkConfigurationManager manager{};
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        logger.logDebug("Session needed");
        session.reset(new QNetworkSession(manager.defaultConfiguration()));
        QObject::connect(session.get(), &QNetworkSession::opened, [this]() { this->connectToServer(); });

        session->open();
    }
    else
    {
        connectToServer();
    }
    void (QTcpSocket::*errorSignal)(QAbstractSocket::SocketError) = &QTcpSocket::error;
    QObject::connect(socket.get(), errorSignal, [this](auto socketError) { this->handleError(socketError); });
    QObject::connect(socket.get(), &QTcpSocket::readyRead, [this]() { this->readData(); });
    QObject::connect(socket.get(), &QAbstractSocket::disconnected,
                     std::bind(&Transport::handleClosingConnection, this));

    connect(this, SIGNAL(sendMessageSignal(QByteArray)), this, SLOT(sendMessageSlot(QByteArray)), Qt::QueuedConnection);
}

void Transport::connectToServer()
{
    socket->connectToHost(server.data(), port);
}

bool Transport::sendMessageSlot(const QByteArray &message)
{
    if (not isConnected())
    {
        logger.logError("Could not send message, connection not established");
        return false;
    }
    logger.logDebug("Send message of size: ", message.size());
    socket->write(message);
    socket->flush();
    return true;
}

bool Transport::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

Transport::~Transport()
{
    logger.logDebug("Bye");
}

void Transport::registerMessageCallback(MessageCallback newMessageCallback)
{
    this->messageCallback = newMessageCallback;
}

void Transport::registerDisconnectedCallback(ITransport::DisconnectedCallback disconnectedCallback)
{
    this->disconnectedCallback = disconnectedCallback;
}

bool Transport::sendMessage(BinaryMessage message)
{
    common::OutgoingMessage sizeEncoder;
    sizeEncoder.writeNumber<BinaryMessage::SizeType>(message.value.size());
    BinaryMessage size = sizeEncoder.getMessage();

    QByteArray array{};
    array.append(reinterpret_cast<char *>(size.value.data()), size.value.size());
    array.append(reinterpret_cast<char *>(message.value.data()), message.value.size());
    return emit sendMessageSignal(array);
}

std::string Transport::addressToString() const
{
    if (not isConnected())
    {
        return "NotConnected";
    }
    return socket->peerAddress().toString().toStdString() + "-" + std::to_string(socket->peerPort());
}

void Transport::readData()
{
    const std::size_t sizeSize = sizeof(BinaryMessage::SizeType);
    quint64 bytesAvailable;
    while ((bytesAvailable = socket->bytesAvailable()) >= sizeSize)
    {
        BinaryMessage sizeEncoded{BinaryMessage::Value(sizeSize)};
        socket->read(reinterpret_cast<char *>(sizeEncoded.value.data()), sizeSize);
        common::IncomingMessage sizeDecoder(sizeEncoded);
        BinaryMessage::SizeType messageLength = sizeDecoder.readNumber<BinaryMessage::SizeType>();

        if (bytesAvailable < sizeSize + messageLength)
        {
            logger.logError("Wrong size: ", std::size_t(messageLength), " - available bytes: ", bytesAvailable);
            continue;
        }

        BinaryMessage message{BinaryMessage::Value(messageLength)};
        socket->read(reinterpret_cast<char *>(message.value.data()), messageLength);
        if (messageCallback)
        {
            messageCallback(std::move(message));
        }
    }
}

void Transport::handleError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        logger.logError("Connection lost");
        break;
    case QAbstractSocket::HostNotFoundError:
        logger.logError("Host not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        logger.logError("Connection refused by peer");
        break;
    default:
        logger.logError(socket->errorString().toStdString());
    }
    // QTimer::singleShot(10000,[this](){this->connectToServer();});
    QTimer::singleShot(10000, this, SLOT(connectToServer()));
}

void Transport::handleClosingConnection()
{
    if (disconnectedCallback)
    {
        logger.logInfo("Connection lost!");
        disconnectedCallback();
    }
    else
    {
        logger.logError("Connection lost! - application not interested!");
    }
}

} // namespace ue
