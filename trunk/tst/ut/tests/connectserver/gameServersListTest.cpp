#include <gtest/gtest.h>
#include <connectserver/gameServersList.hpp>
#include <core/common/xmlReader.hpp>
#include <core/common/exception.hpp>

class GameServersListTest: public ::testing::Test
{
protected:
    void prepareXmlContent()
    {
        xmlContent_ = "<servers> \
                            <server code=\"0\" name=\"eMU_Test\" address=\"localhost\" port=\"55901\" /> \
                            <server code=\"20\" name=\"eMU_Test2\" address=\"127.0.0.1\" port=\"55902\" /> \
                        </servers>";

        xmlReader_.parse(xmlContent_, "servers");
    }

    void prepareSampleServers()
    {
        eMU::connectserver::GameServersList::GameServerInfo info = {};

        info.address_ = "localhost";
        info.code_ = 0;
        info.load_ = 0;
        info.name_ = "eMU_Test";
        info.port_ = 55901;
        sampleServers_.push_back(info);

        info.address_ = "127.0.0.1";
        info.code_ = 20;
        info.load_ = 0;
        info.name_ = "eMU_Test2";
        info.port_ = 55902;
        sampleServers_.push_back(info);
    }

    void compareGameServerInfo(const eMU::connectserver::GameServersList::GameServerInfo &left,
                               const eMU::connectserver::GameServersList::GameServerInfo &right)
    {
        EXPECT_EQ(left.address_, right.address_);
        EXPECT_EQ(left.code_, right.code_);
        EXPECT_EQ(left.load_, right.load_);
        EXPECT_EQ(left.name_, right.name_);
        EXPECT_EQ(left.port_, right.port_);
    }

    void SetUp() {}

    void initialize()
    {
        prepareXmlContent();
        prepareSampleServers();
        gameServersList_.initialize(xmlReader_);
    }

    std::string xmlContent_;
    eMU::core::common::XmlReader xmlReader_;

    eMU::connectserver::GameServersList::GameServersListContainer sampleServers_;
    eMU::connectserver::GameServersList gameServersList_;
};

TEST_F(GameServersListTest, initialize)
{
    initialize();

    ASSERT_EQ(sampleServers_.size(), gameServersList_.list().size());

    for(size_t i = 0; i < sampleServers_.size(); ++i)
    {
        compareGameServerInfo(sampleServers_[i], gameServersList_.list()[i]);
    }
}

TEST_F(GameServersListTest, updateLoad)
{
    initialize();

    gameServersList_.updateGameServerLoad(0, 95);
    gameServersList_.updateGameServerLoad(20, 35);

    EXPECT_EQ(95, gameServersList_.list()[0].load_);
    EXPECT_EQ(35, gameServersList_.list()[1].load_);
}

TEST_F(GameServersListTest, updateLoadWithInvalidCodeShouldDoNothing)
{
    initialize();

    gameServersList_.updateGameServerLoad(1, 30);

    EXPECT_EQ(0, gameServersList_.list()[0].load_);
    EXPECT_EQ(0, gameServersList_.list()[1].load_);
}

TEST_F(GameServersListTest, hasGameServer)
{
    initialize();

    EXPECT_TRUE(gameServersList_.hasGameServer(0));
    EXPECT_TRUE(gameServersList_.hasGameServer(20));

    EXPECT_FALSE(gameServersList_.hasGameServer(21));
    EXPECT_FALSE(gameServersList_.hasGameServer(3));
}

TEST_F(GameServersListTest, getGameServerInfo)
{
    initialize();

    const eMU::connectserver::GameServersList::GameServerInfo &serverInfo = gameServersList_.getGameServerInfo(20);

    compareGameServerInfo(sampleServers_[1], serverInfo);
}
