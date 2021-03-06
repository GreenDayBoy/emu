#include <streaming/loginserver/loginResponse.hpp>
#include <streaming/loginserver/streamIds.hpp>

namespace eMU
{
namespace streaming
{
namespace loginserver
{

LoginResponse::LoginResponse(const ReadStream &readStream):
    readStream_(readStream)
{
    readStream_.readNext<uint32_t>(); // dummy1
    readStream_.readNext<uint32_t>(); // dummy2
    result_ = readStream_.readNext<LoginResult>();
}

LoginResponse::LoginResponse(LoginResult result):
    writeStream_(streamIds::kLoginResponse)
{
    writeStream_.writeNext<uint32_t>(0); // dummy1
    writeStream_.writeNext<uint32_t>(0); // dummy2
    writeStream_.writeNext<LoginResult>(result);
}

const WriteStream& LoginResponse::getWriteStream() const
{
    return writeStream_;
}

LoginResult LoginResponse::getResult() const
{
    return result_;
}

}
}
}
