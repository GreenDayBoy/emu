#pragma once

#include <core/network/tcp/protocol.hpp>
#include <streaming/readStream.hpp>
#include <streaming/readStreamsExtractor.hpp>
#include <protocols/contexts/server.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace protocols
{

template<typename UserType>
class Server: public core::network::tcp::Protocol
{
public:
    Server(contexts::Server<UserType> &context):
        context_(context) {}

    bool attach(core::network::tcp::Connection::Pointer connection)
    {
        if(context_.getUsersFactory().getObjects().size() > context_.getMaxNumberOfUsers())
        {
            eMU_LOG(warning) << "Max number of users reached.";
            return false;
        }

        UserType &user = context_.getUsersFactory().create(connection);

        eMU_LOG(info) << "User connected, hash: " << user.getHash();

        return true;
    }

    void detach(core::network::tcp::Connection::Pointer connection)
    {
        try
        {
            UserType &user = context_.getUsersFactory().find(connection);

            eMU_LOG(info) << "User closed, hash: " << user.getHash();

            context_.getUsersFactory().destroy(user);
            connection->close();
        }
        catch(const typename core::common::Factory<UserType>::ObjectNotFoundException&)
        {
            eMU_LOG(error) << "Could not find user by connection.";
            connection->close();
        }
    }

    bool dispatch(core::network::tcp::Connection::Pointer connection)
    {
        streaming::ReadStreamsExtractor readStreamsExtractor(connection->getReadPayload());
        if(!readStreamsExtractor.extract())
        {
            eMU_LOG(error) << "Streams extraction failed.";
            return false;
        }

        try
        {
            UserType &user = context_.getUsersFactory().find(connection);

            for(const auto &stream : readStreamsExtractor.getStreams())
            {
                eMU_LOG(info) << "Received, hash: " << user.getHash() << ", stream id: " << stream.getId();

                if(!this->handleReadStream(user, stream))
                {
                    return false;
                }
            }
        }
        catch(const typename core::common::Factory<UserType>::ObjectNotFoundException&)
        {
            eMU_LOG(error) << "Could not find user by connection.";
            return false;
        }

        return true;
    }

protected:
    virtual bool handleReadStream(UserType &user, const streaming::ReadStream &stream) = 0;

private:
    contexts::Server<UserType> &context_;
};

}
}
