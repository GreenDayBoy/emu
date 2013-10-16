#pragma once

#ifdef eMU_UT
#include <ut/env/asioStub/ioService.hpp>
#elif eMU_MT
#include <mt/env/asioStub/ioService.hpp>
#else
#include <boost/asio.hpp>
#endif

#include <boost/noncopyable.hpp>
#include <glog/logging.h>

#include <core/common/usersFactory.hpp>
#include <core/transactions/manager.hpp>
#include <core/network/tcp/connectionsManager.hpp>
#include <core/protocol/messagesExtractor.hpp>

namespace eMU
{
namespace core
{
namespace network
{

#ifdef eMU_UT
namespace asio = eMU::ut::env::asioStub;
#elif eMU_MT
namespace asio = eMU::mt::env::asioStub;
#else
namespace asio = boost::asio;
#endif

template<typename User>
class Server: boost::noncopyable
{
public:
    Server(core::network::tcp::ConnectionsManager::Pointer connectionsManager,
           typename core::common::UsersFactory<User>::Pointer usersFactory,
           core::transactions::Manager::Pointer transactionsManager):
        connectionsManager_(connectionsManager),
        usersFactory_(usersFactory),
        transactionsManager_(transactionsManager)
    {
           connectionsManager_->setAcceptEventCallback(std::bind(&Server::onAccept, this, std::placeholders::_1));
           connectionsManager_->setReceiveEventCallback(std::bind(&Server::onReceive, this, std::placeholders::_1, std::placeholders::_2));
           connectionsManager_->setCloseEventCallback(std::bind(&Server::onClose, this, std::placeholders::_1));
           connectionsManager_->setGenerateConnectionHashCallback(std::bind(&Server::generateConnectionHash, this));
    }

    virtual ~Server() {}

    virtual bool startup() = 0;
    virtual void cleanup() = 0;
    virtual void onAccept(size_t hash) = 0;

    void onReceive(size_t hash, const eMU::core::network::Payload &payload)
    {
        try
        {
            core::protocol::MessagesExtractor messagesExtractor(payload);
            messagesExtractor.extract();

            const core::protocol::MessagesExtractor::MessagesContainer &messages = messagesExtractor.messages();

            for(const auto &message : messages)
            {
                this->handleMessage(hash, message);

                if(!transactionsManager_->dequeueAll())
                {
                    LOG(ERROR) << "hash: " << hash << ", some transactions were invalid. Disconnected.";
                    connectionsManager_->disconnect(hash);
                }
            }
        }
        catch(core::protocol::MessagesExtractor::EmptyPayloadException&)
        {
            LOG(ERROR) << "hash: " << hash << ", received empty payload!";
            connectionsManager_->disconnect(hash);
        }
        catch(core::protocol::MessagesExtractor::InvalidMessageHeaderException&)
        {
            LOG(ERROR) << "hash: " << hash << ", invalid message header detected!";
            connectionsManager_->disconnect(hash);
        }
        catch(core::protocol::MessagesExtractor::InvalidMessageSizeException&)
        {
            LOG(ERROR) << "hash: " << hash << ", invalid message size detected!";
            connectionsManager_->disconnect(hash);
        }
    }

    virtual void onClose(size_t hash) = 0;

protected:
    size_t generateConnectionHash()
    {
        try
        {
            User &user = usersFactory_->create();

            return user.hash();
        }
        catch(typename core::common::UsersFactory<User>::MaxNumberOfUsersReachedException&)
        {
            LOG(WARNING) << "Max number of users reached.";
            return 0;
        }
    }

    virtual void handleMessage(size_t hash, const core::network::Payload &payload) = 0;

    core::network::tcp::ConnectionsManager::Pointer connectionsManager_;
    typename core::common::UsersFactory<User>::Pointer usersFactory_;
    core::transactions::Manager::Pointer transactionsManager_;
};

}
}
}
