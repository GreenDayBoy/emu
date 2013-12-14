#include <analyzer/user.hpp>
#include <protocol/readStreamsExtractor.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace analyzer
{

User::User(core::network::tcp::Connection &connection):
    NetworkUser(connection) {}

void User::parseReadPayload()
{
    try
    {
        protocol::ReadStreamsExtractor readStreamsExtractor(connection_.getReadPayload());
        readStreamsExtractor.extract();

        for(const auto &stream : readStreamsExtractor.getStreams())
        {
            readStreams_.push_back(stream);
        }
    }
    catch(const protocol::ReadStreamsExtractor::EmptyPayloadException&)
    {
        LOG(ERROR) << "Received empty payload! hash: " << this->getHash();
        connection_.disconnect();
    }
    catch(const protocol::ReadStreamsExtractor::EmptyStreamException&)
    {
        LOG(ERROR) << "Received empty stream! hash: " << this->getHash();
        connection_.disconnect();
    }
    catch(const protocol::ReadStreamsExtractor::UnknownStreamFormatException&)
    {
        LOG(ERROR) << "Received stream with unknown format! hash: " << this->getHash();
        connection_.disconnect();
    }
}

std::list<protocol::ReadStream> &User::getReadStreams()
{
    return readStreams_;
}

}
}
