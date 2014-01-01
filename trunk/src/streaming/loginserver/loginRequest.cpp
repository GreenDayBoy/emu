#include <streaming/loginserver/loginRequest.hpp>
#include <streaming/loginserver/messageIds.hpp>

#include <boost/locale.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
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

LoginRequest::LoginRequest(const std::wstring &accountId, const std::wstring &password):
    writeStream_(MessageIds::kLoginRequest)
{
    writeStream_.writeNext<uint32_t>(0); // dummy1
    writeStream_.writeNext<uint32_t>(0); // dummy2

    writeStream_.writeNext<uint32_t>(accountId.length());
    writeStream_.writeNextWideString(accountId);

    writeStream_.writeNext<uint32_t>(password.length());
    writeStream_.writeNextWideString(password);
}

const WriteStream& LoginRequest::getWriteStream() const
{
    return writeStream_;
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
