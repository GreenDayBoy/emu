#include <gtest/gtest.h>

#include <core/common/xmlReader.hpp>

using eMU::core::common::XmlReader;

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

    void checkRecord(XmlReader &xmlReader, uint16_t code, const std::string &name, const std::string &address, uint16_t port)
    {
        EXPECT_EQ(code, xmlReader.get<uint16_t>("server", "code"));
        EXPECT_EQ(name, xmlReader.get<std::string>("server", "name"));
        EXPECT_EQ(address, xmlReader.get<std::string>("server", "address"));
        EXPECT_EQ(port, xmlReader.get<uint16_t>("server", "port"));
    }

    std::string content_;
};

TEST_F(XmlReaderTest, parseCorrectContent)
{
    prepareContent();

    XmlReader xmlReader(content_);
    ASSERT_TRUE(xmlReader.parse("servers"));

    ASSERT_FALSE(xmlReader.end());
    checkRecord(xmlReader, 21, "eMU_Test", "localhost", 55901);

    xmlReader.next();
    ASSERT_FALSE(xmlReader.end());
    checkRecord(xmlReader, 22, "eMU_Test2", "127.0.0.1", 55902);

    xmlReader.next();
    ASSERT_TRUE(xmlReader.end());

    xmlReader.next();
    ASSERT_TRUE(xmlReader.end());

    xmlReader.clear();
}

TEST_F(XmlReaderTest, WhenContentIsEmptyThenParseShouldReturnFalse)
{
    XmlReader xmlReader("");
    ASSERT_FALSE(xmlReader.parse("servers"));
}

TEST_F(XmlReaderTest, WhenFirstNodeDoesNotMatchThenParseShouldReturnFalse)
{
    prepareContent();

    XmlReader xmlReader(content_);
    ASSERT_FALSE(xmlReader.parse("ssservverrs"));
}

TEST_F(XmlReaderTest, parseContentWithoutRecords)
{
    content_ = "<servers></servers>";

    XmlReader xmlReader(content_);
    ASSERT_TRUE(xmlReader.parse("servers"));

    ASSERT_TRUE(xmlReader.end());

    xmlReader.clear();
}

TEST_F(XmlReaderTest, getShouldReturnDefaultValueWhenContentDoesNotHaveAtributes)
{
    content_ = "<servers><server /></servers>";

    XmlReader xmlReader(content_);
    ASSERT_TRUE(xmlReader.parse("servers"));

    ASSERT_FALSE(xmlReader.end());

    EXPECT_EQ("default", xmlReader.get<std::string>("server", "name", "default"));
    EXPECT_EQ("", xmlReader.get<std::string>("server", "name"));

    EXPECT_EQ(0, xmlReader.get<int32_t>("server", "code"));
    EXPECT_EQ(45, xmlReader.get<int32_t>("server", "code", 45));

    xmlReader.clear();
}
