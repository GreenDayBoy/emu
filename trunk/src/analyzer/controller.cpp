#include <analyzer/controller.hpp>
#include <protocol/writeStream.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
#include <sstream>

namespace eMU
{
namespace analyzer
{

Controller::Controller(asio::io_service &ioService):
    server_(ioService)
{
    server_.setController(this);
    view_.setController(this);
}

Server& Controller::getServer()
{
    return server_;
}

View& Controller::getView()
{
    return view_;
}

void Controller::onAccept(User &user)
{
    view_.displayUser(boost::lexical_cast<std::string>(user.getHash()));
}

void Controller::onReceive(User &user)
{
    std::vector<std::string> streamHashes;

    for(const auto& stream: user.getReadStreams())
    {
        size_t streamHash = reinterpret_cast<size_t>(&stream);
        streamHashes.push_back(boost::lexical_cast<std::string>(streamHash));
    }

    view_.displayUserReadStreams(boost::lexical_cast<std::string>(user.getHash()), streamHashes);
}

void Controller::onClose(User &user)
{
    view_.removeUserFromDisplay(boost::lexical_cast<std::string>(user.getHash()));
}

void Controller::loadReadStream(const std::string &userId, const std::string &streamId)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(userId));
        User &user = server_.getUsersFactory().find(userHash);

        for(const auto &stream: user.getReadStreams())
        {
            size_t streamHash = reinterpret_cast<size_t>(&stream);

            if(streamId == boost::lexical_cast<std::string>(streamHash))
            {
                std::stringstream streamHex; streamHex << stream;
                view_.displayReadStream(streamHex.str(), stream.getId(), stream.getSize());
                break;
            }
        }
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find userId: " << userId;
    }
}

void Controller::disconnectUser(const std::string &userId)
{
    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(userId));
        User &user = server_.getUsersFactory().find(userHash);

        user.getConnection().disconnect();
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find userId: " << userId;
    }
}

void Controller::send(const std::string &userId, const std::string &streamId, const stream::fields::FieldsContainer &fields)
{
    const protocol::WriteStream &stream = this->generateWriteStream(streamId, fields);

    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(userId));
        User &user = server_.getUsersFactory().find(userHash);

        user.getConnection().send(stream.getPayload());
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find userId: " << userId;
    }
}

protocol::WriteStream Controller::generateWriteStream(const std::string &streamId, const stream::fields::FieldsContainer &fields) const
{
    protocol::WriteStream stream(boost::lexical_cast<uint16_t>(streamId));

    for(const auto field : fields)
    {
        if(field->getType() == stream::fields::Field::kInt8Type)
        {
            stream.writeNext<uint8_t>(boost::lexical_cast<uint16_t>(field->getValue()));
        }
        else if(field->getType() == stream::fields::Field::kInt16Type)
        {
            stream.writeNext<uint16_t>(boost::lexical_cast<uint16_t>(field->getValue()));
        }
        else if(field->getType() == stream::fields::Field::kInt32Type)
        {
            stream.writeNext<uint32_t>(boost::lexical_cast<uint32_t>(field->getValue()));
        }
        else if(field->getType() == stream::fields::Field::kStringType)
        {
            stream.writeNextString(field->getValue());
        }
        else if(field->getType() == stream::fields::Field::kWStringType)
        {
            std::wstring value = boost::locale::conv::utf_to_utf<std::wstring::value_type>(field->getValue());
            stream.writeNextWideString(value);
        }
    }

    return stream;
}

std::string Controller::generateStreamPreview(const std::string &streamId, const stream::fields::FieldsContainer &fields) const
{
    const protocol::WriteStream &stream = this->generateWriteStream(streamId, fields);

    std::stringstream ss; ss << stream;
    return ss.str();
}

void Controller::send(const std::string &userId, std::string hexDump)
{
    hexDump.erase(std::remove(hexDump.begin(), hexDump.end(), ' '));

    core::network::Payload payload;
    payload.setSize(hexDump.length() / 2);

    size_t byte = 0;
    for(size_t i = 0; i < hexDump.length(); i += 2)
    {
        std::string v(hexDump, i, 2);

        std::stringstream ss;
        ss << std::hex << v;
        int16_t dec = 0;
        ss >> dec;

        payload[byte++] = dec;
    }

    try
    {
        User::Hash userHash = User::Hash(boost::lexical_cast<size_t>(userId));
        User &user = server_.getUsersFactory().find(userHash);

        user.getConnection().send(payload);
    }
    catch(const core::common::Factory<User>::ObjectNotFoundException&)
    {
        LOG(ERROR) << "Cound not find userId: " << userId;
    }
}

}
}
