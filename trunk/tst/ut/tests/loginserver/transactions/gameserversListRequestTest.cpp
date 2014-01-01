#include <loginserver/transactions/gameserversListRequest.hpp>
#include <loginserver/user.hpp>
#include <core/network/payload.hpp>
#include <streaming/loginserver/gameserversListRequest.hpp>
#include <streaming/loginserver/gameserversListResponse.hpp>
#include <streaming/loginserver/streamIds.hpp>

#include <ut/env/core/network/tcp/connectionMock.hpp>
#include <ut/env/loginserver/gameserversListMock.hpp>

#include <gtest/gtest.h>
#include <boost/locale.hpp>

using ::testing::_;
using ::testing::ReturnRef;
using ::testing::SaveArg;

using eMU::streaming::ReadStream;
using eMU::core::network::Payload;
using eMU::streaming::loginserver::GameserversListRequest;
using eMU::streaming::loginserver::GameserversListResponse;
using eMU::streaming::loginserver::GameserversInfoContainer;
using eMU::loginserver::User;
using eMU::ut::env::core::network::tcp::ConnectionMock;
using eMU::ut::env::loginserver::GameserversListMock;
namespace streamIds = eMU::streaming::loginserver::streamIds;

class GameserversListRequestTransactionTest: public ::testing::Test
{
protected:
    GameserversListRequestTransactionTest():
        connection_(new ConnectionMock()),
        user_(connection_),
        request_(ReadStream(GameserversListRequest().getWriteStream().getPayload())),
        transaction_(user_, gameserversList_, request_) {}

    ConnectionMock::Pointer connection_;
    User user_;
    GameserversListMock gameserversList_;
    GameserversListRequest request_;
    eMU::loginserver::transactions::GameserversListRequest transaction_;
};


TEST_F(GameserversListRequestTransactionTest, handle)
{
    GameserversInfoContainer servers = {{1, "eMU_TEST1", "127.0.0.1", 55557},
                                        {2, "eMU_TEST2", "127.0.0.2", 55557}};

    EXPECT_CALL(gameserversList_, getServers()).WillOnce(ReturnRef(servers));

    Payload payload;
    EXPECT_CALL(*connection_, send(_)).WillOnce(SaveArg<0>(&payload));
    transaction_.handle();

    ReadStream readStream(payload);
    ASSERT_EQ(streamIds::kGameserversListResponse, readStream.getId());

    GameserversListResponse response(readStream);
    ASSERT_EQ(servers.size(), response.getServers().size());

    ASSERT_EQ(servers[0].code_, response.getServers()[0].code_);
    ASSERT_EQ(servers[0].name_, response.getServers()[0].name_);

    ASSERT_EQ(servers[1].code_, response.getServers()[1].code_);
    ASSERT_EQ(servers[1].name_, response.getServers()[1].name_);
}
