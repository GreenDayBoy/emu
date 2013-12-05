#include <loginserver/transactions/gameserversListRequestTransaction.hpp>
#include <core/network/payload.hpp>
#include <protocol/loginserver/gameserversListRequest.hpp>
#include <protocol/loginserver/gameserversListResponse.hpp>
#include <protocol/loginserver/messageIds.hpp>

#include <ut/env/core/network/tcp/connectionsManagerMock.hpp>
#include <ut/env/loginserver/gameserversListMock.hpp>

#include <gtest/gtest.h>
#include <boost/locale.hpp>

using ::testing::_;
using ::testing::ReturnRef;
using ::testing::SaveArg;

using eMU::protocol::ReadStream;
using eMU::core::network::Payload;
using eMU::protocol::loginserver::GameserversListRequest;
using eMU::protocol::loginserver::GameserversListResponse;
using eMU::protocol::loginserver::GameserversInfoContainer;
using eMU::loginserver::transactions::GameserversListRequestTransaction;
using eMU::ut::env::core::network::tcp::ConnectionsManagerMock;
using eMU::ut::env::loginserver::GameserversListMock;
namespace MessageIds = eMU::protocol::loginserver::MessageIds;

class GameserversListRequestTransactionTest: public ::testing::Test
{
protected:
    GameserversListRequestTransactionTest():
        hash_(0x54327),
        request_(ReadStream(GameserversListRequest().getWriteStream().getPayload())),
        transaction_(hash_, connectionsManager_, gameserversList_, request_) {}

    size_t hash_;
    ConnectionsManagerMock connectionsManager_;
    GameserversListMock gameserversList_;
    GameserversListRequest request_;
    GameserversListRequestTransaction transaction_;
};


TEST_F(GameserversListRequestTransactionTest, handle)
{
    GameserversInfoContainer servers = {{1, "eMU_TEST1", "127.0.0.1", 55557},
                                        {2, "eMU_TEST2", "127.0.0.2", 55557}};

    EXPECT_CALL(gameserversList_, getServers()).WillOnce(ReturnRef(servers));

    Payload payload;
    EXPECT_CALL(connectionsManager_, send(hash_, _)).WillOnce(SaveArg<1>(&payload));
    transaction_.handle();

    ReadStream readStream(payload);
    ASSERT_EQ(MessageIds::kGameserversListResponse, readStream.getId());

    GameserversListResponse response(readStream);
    ASSERT_EQ(servers.size(), response.getServers().size());

    ASSERT_EQ(servers[0].code_, response.getServers()[0].code_);
    ASSERT_EQ(servers[0].name_, response.getServers()[0].name_);

    ASSERT_EQ(servers[1].code_, response.getServers()[1].code_);
    ASSERT_EQ(servers[1].name_, response.getServers()[1].name_);
}
