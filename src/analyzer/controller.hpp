#pragma once

#include <analyzer/server.hpp>
#include <analyzer/view.hpp>
#include <analyzer/stream/fields/field.hpp>
#include <protocol/writeStream.hpp>

#include <map>

namespace eMU
{
namespace analyzer
{

class Controller
{
public:
    Controller(asio::io_service &ioService);

    void onAccept(User &user);
    void onReceive(User &user);
    void onClose(User &user);
    void loadReadStream(const std::string &userId, const std::string &streamId);
    void disconnectUser(const std::string &userId);

    void send(const std::string &userId, const std::string &streamId, const stream::fields::FieldsContainer &fields);
    void send(const std::string &userId, std::string hexDump);
    std::string generateStreamPreview(const std::string &streamId, const stream::fields::FieldsContainer &fields) const;

    Server& getServer();
    View& getView();

private:
    protocol::WriteStream generateWriteStream(const std::string &streamId, const stream::fields::FieldsContainer &fields) const;

    Server server_;
    View view_;
};

}
}
