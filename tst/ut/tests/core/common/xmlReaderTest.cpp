#include <gtest/gtest.h>

#include <core/common/xmlReader.hpp>

namespace common = eMU::core::common;

class XmlReaderTest: public ::testing::Test
{
protected:
    void prepareContent()
    {
        content_ = "<servers> \
                        <server code=\"21\" name=\"eMU_Test\" address=\"localhost\" port=\"55901\" /> \
                        <server code=\"22\" name=\"eMU_Test2\" address=\"127.0.0.1\" port=\"55902\" /> \
                   </servers>";
    }

    void checkRecord(uint16_t code, const std::string &name, const std::string &address, uint16_t port)
    {
        EXPECT_EQ(code, xmlReader_.get<uint16_t>("server", "code"));
        EXPECT_EQ(name, xmlReader_.get<std::string>("server", "name"));
        EXPECT_EQ(address, xmlReader_.get<std::string>("server", "address"));
        EXPECT_EQ(port, xmlReader_.get<uint16_t>("server", "port"));
    }

    common::XmlReader xmlReader_;
    std::string content_;
};

TEST_F(XmlReaderTest, parseCorrectContent)
{
    prepareContent();

    xmlReader_.parse(content_, "servers");
    ASSERT_FALSE(xmlReader_.end());
    checkRecord(21, "eMU_Test", "localhost", 55901);

    xmlReader_.next();
    ASSERT_FALSE(xmlReader_.end());
    checkRecord(22, "eMU_Test2", "127.0.0.1", 55902);

    xmlReader_.next();
    ASSERT_TRUE(xmlReader_.end());

    xmlReader_.next();
    ASSERT_TRUE(xmlReader_.end());

    xmlReader_.clear();
}

TEST_F(XmlReaderTest, exceptionShouldBeThrownWhenContentIsEmpty)
{
    bool exceptionThrown = false;

    try
    {
        xmlReader_.parse(content_, "servers");
    }
    catch(const common::XmlReader::EmptyXmlContentException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(XmlReaderTest, exceptionShouldBeThrownWhenFirstNodeDoesNotMatch)
{
    prepareContent();

    bool exceptionThrown = false;

    try
    {
        xmlReader_.parse(content_, "ssservverrs");
    }
    catch(const common::XmlReader::NotMatchedXmlNodeException&)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);
}

TEST_F(XmlReaderTest, parseContentWithoutRecords)
{
    content_ = "<servers></servers>";
    xmlReader_.parse(content_, "servers");

    ASSERT_TRUE(xmlReader_.end());

    xmlReader_.clear();
}

TEST_F(XmlReaderTest, getShouldReturnDefaultValueWhenContentDoesNotHaveAtributes)
{
    content_ = "<servers><server /></servers>";
    xmlReader_.parse(content_, "servers");

    ASSERT_FALSE(xmlReader_.end());

    EXPECT_EQ("default", xmlReader_.get<std::string>("server", "name", "default"));
    EXPECT_EQ("", xmlReader_.get<std::string>("server", "name"));

    EXPECT_EQ(0, xmlReader_.get<int32_t>("server", "code"));
    EXPECT_EQ(45, xmlReader_.get<int32_t>("server", "code", 45));

    xmlReader_.clear();
}
