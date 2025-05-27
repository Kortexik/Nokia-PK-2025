#pragma once
#include "ITransport.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <QAbstractSocket>
#include <memory>

class QTcpSocket;
class QNetworkSession;

namespace common
{
class MultiLineConfig;
}

namespace ue
{

class Transport : public QObject, public ITransport
{
    Q_OBJECT

 public slots:
    void connectToServer();

 public:
    Transport(common::MultiLineConfig &configuration, common::ILogger &logger);
    ~Transport();
    void registerMessageCallback(MessageCallback messageCallback) override;
    void registerDisconnectedCallback(DisconnectedCallback disconnectedCallback) override;
    bool sendMessage(BinaryMessage message) override;
    std::string addressToString() const override;

 private slots:
    bool sendMessageSlot(const QByteArray &message);
 signals:
    bool sendMessageSignal(const QByteArray &message);

 private:
    void readData();
    void handleError(QAbstractSocket::SocketError socketError);
    void handleClosingConnection();
    //    void connectToServer();
    bool isConnected() const;
    common::PrefixedLogger logger;
    int port;
    std::string server;
    std::unique_ptr<QTcpSocket> socket;
    std::unique_ptr<QNetworkSession> session;
    MessageCallback messageCallback;
    DisconnectedCallback disconnectedCallback;
};

} // namespace ue
