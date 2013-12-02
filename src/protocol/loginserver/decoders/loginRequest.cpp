#include <protocol/loginserver/decoders/loginRequest.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace protocol
{
namespace loginserver
{
namespace decoders
{

LoginRequest::LoginRequest(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy1;
    readStream_.readNext<uint32_t>(); // dummy2;

    uint32_t accountIdLength = readStream_.readNext<uint32_t>();
    std::wstring accountId = readStream_.readNextWideString(accountIdLength);
    accountId_ = boost::locale::conv::utf_to_utf<std::string::value_type>(accountId);

    uint32_t passwordLength = readStream_.readNext<uint32_t>();
    std::wstring password = readStream_.readNextWideString(passwordLength);
    password_ = boost::locale::conv::utf_to_utf<std::string::value_type>(password);
}

const std::string& LoginRequest::getAccountId() const
{
    return accountId_;
}

const std::string& LoginRequest::getPassword() const
{
    return password_;
}

}
}
}
}
