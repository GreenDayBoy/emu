#include <gtest/gtest.h>
#include <core/user/userIdGenerator.hpp>

class userIdGeneratorTest_t: public ::testing::Test {
public:
    userIdGeneratorTest_t():
      idGenerator_(maxNumOfIds_) {}

    void SetUp() {}
    void TearDown() {}

    static const int16 maxNumOfIds_ = 10;
    eMU::core::user::idGenerator_t idGenerator_;
};

TEST_F(userIdGeneratorTest_t, get) {
    int16 id = eMU::core::user::invalidId_c;

    for(int16 i = 0; i < maxNumOfIds_; ++i) {
        id = idGenerator_.get();

        ASSERT_EQ(i, id);
    }

    id = idGenerator_.get();
    EXPECT_EQ(eMU::core::user::invalidId_c, id);
}

TEST_F(userIdGeneratorTest_t, insert) {
    for(int16 i = 0; i < maxNumOfIds_; ++i) {
        idGenerator_.get();
    }

    idGenerator_.insert(9);

    int16 id = idGenerator_.get();
    EXPECT_EQ(9, id);

    // Make sure that we have no free ids again.
    id = idGenerator_.get();
    EXPECT_EQ(eMU::core::user::invalidId_c, id);
}