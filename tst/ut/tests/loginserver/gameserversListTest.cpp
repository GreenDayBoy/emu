#include <gtest/gtest.h>
#include <loginserver/gameserversList.hpp>
#include <core/common/xmlReader.hpp>
#include <core/common/exception.hpp>

using eMU::loginserver::GameserversList;
using eMU::protocol::loginserver::GameserverInfo;
using eMU::protocol::loginserver::GameserversInfoContainer;
using eMU::core::common::XmlReader;

class GameserversListTest: public ::testing::Test
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
        GameserverInfo info = {};

        info.address_ = "localhost";
        info.code_ = 0;
        info.name_ = "eMU_Test";
        info.port_ = 55901;
        sampleServers_.push_back(info);

        info.address_ = "127.0.0.1";
        info.code_ = 20;
        info.name_ = "eMU_Test2";
        info.port_ = 55902;
        sampleServers_.push_back(info);
    }

    void compareGameserverInfo(const GameserverInfo &left,
                               const GameserverInfo &right)
    {
        EXPECT_EQ(left.address_, right.address_);
        EXPECT_EQ(left.code_, right.code_);
        EXPECT_EQ(left.name_, right.name_);
        EXPECT_EQ(left.port_, right.port_);
    }

    void SetUp() {}

    void initialize()
    {
        prepareXmlContent();
        prepareSampleServers();
        gameserversList_.initialize(xmlReader_);
    }

    std::string xmlContent_;
    XmlReader xmlReader_;

    GameserversInfoContainer sampleServers_;
    GameserversList gameserversList_;
};

TEST_F(GameserversListTest, initialize)
{
    initialize();

    ASSERT_EQ(sampleServers_.size(), gameserversList_.getServers().size());

    for(size_t i = 0; i < sampleServers_.size(); ++i)
    {
        compareGameserverInfo(sampleServers_[i], gameserversList_.getServers()[i]);
    }
}

TEST_F(GameserversListTest, hasGameserver)
{
    initialize();

    EXPECT_TRUE(gameserversList_.hasGameserver(0));
    EXPECT_TRUE(gameserversList_.hasGameserver(20));

    EXPECT_FALSE(gameserversList_.hasGameserver(21));
    EXPECT_FALSE(gameserversList_.hasGameserver(3));
}

TEST_F(GameserversListTest, getGameserverInfo)
{
    initialize();

    const GameserverInfo &serverInfo = gameserversList_.getGameserverInfo(20);

    compareGameserverInfo(sampleServers_[1], serverInfo);
}
