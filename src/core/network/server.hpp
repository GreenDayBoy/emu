#pragma once

#include <boost/noncopyable.hpp>
#include <glog/logging.h>

#include <core/common/usersFactory.hpp>
#include <core/transactions/manager.hpp>
#include <core/network/tcp/connectionsManager.hpp>
#include <core/protocol/packetsExtractor.hpp>

#include <common/asio.hpp>

namespace eMU
{
namespace core
{
namespace network
{

template<typename User>
class Server: boost::noncopyable
{
public:
    class UnknownMessageException: public core::common::Exception {};

    Server(asio::io_service& ioService,
           uint16_t port,
           size_t maxNumberOfUsers):
        connectionsManager_(ioService, port),
        usersFactory_(maxNumberOfUsers)
    {
           connectionsManager_.setAcceptEventCallback(std::bind(&Server::onAccept, this, std::placeholders::_1));
           connectionsManager_.setReceiveEventCallback(std::bind(&Server::onReceive, this, std::placeholders::_1, std::placeholders::_2));
           connectionsManager_.setCloseEventCallback(std::bind(&Server::onClose, this, std::placeholders::_1));
           connectionsManager_.setGenerateConnectionHashCallback(std::bind(&Server::generateConnectionHash, this));
    }

    virtual ~Server() {}

    bool startup()
    {
        connectionsManager_.queueAccept();
        return this->onStartup();
    }

    void cleanup()
    {
        this->onCleanup();
    }

    virtual bool onStartup() = 0;
    virtual void onCleanup() = 0;
    virtual void onAccept(size_t hash) = 0;

    void onReceive(size_t hash, const eMU::core::network::Payload &payload)
    {
        try
        {
            core::protocol::PacketsExtractor packetsExtractor(payload);
            packetsExtractor.extract();

            const core::protocol::PacketsExtractor::PacketsContainer &packets = packetsExtractor.getPackets();

            for(const auto &packet : packets)
            {
                this->handlePacket(hash, packet);

                if(!transactionsManager_.dequeueAll())
                {
                    LOG(ERROR) << "hash: " << hash << ", some transactions were invalid. Disconnected.";
                    connectionsManager_.disconnect(hash);
                }
            }
        }
        catch(const core::protocol::PacketsExtractor::EmptyPayloadException&)
        {
            LOG(ERROR) << "hash: " << hash << ", received empty payload!";
            connectionsManager_.disconnect(hash);
        }
        catch(const core::protocol::PacketsExtractor::NullPacketSizeException&)
        {
            LOG(ERROR) << "hash: " << hash << ", received packet with null size!";
            connectionsManager_.disconnect(hash);
        }
        catch(const core::protocol::PacketsExtractor::PacketSizeOutOfBoundException&)
        {
            LOG(ERROR) << "hash: " << hash << ", received packet with invalid size!";
            connectionsManager_.disconnect(hash);
        }
        catch(const UnknownMessageException&)
        {
            LOG(ERROR) << "hash: " << hash << ", received unknown message!";
            connectionsManager_.disconnect(hash);
        }
    }

    virtual void onClose(size_t hash) = 0;

protected:
    size_t generateConnectionHash()
    {
        try
        {
            return usersFactory_.create();
        }
        catch(const typename core::common::UsersFactory<User>::MaxNumberOfUsersReachedException&)
        {
            LOG(WARNING) << "Max number of users reached.";
            return 0;
        }
    }

    virtual void handlePacket(size_t hash, const core::network::Payload &packet) = 0;

    core::network::tcp::ConnectionsManager connectionsManager_;
    core::common::UsersFactory<User> usersFactory_;
    core::transactions::Manager transactionsManager_;
};

}
}
}
