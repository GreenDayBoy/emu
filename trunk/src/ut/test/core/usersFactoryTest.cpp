#include <gtest/gtest.h>

#include <core/user/usersFactory.hpp>
#include <env/core/user/userStub.hpp>

class usersFactoryTest_t: public ::testing::Test {
public:
    usersFactoryTest_t():
      factory_(maxNumOfUsers_) {}

    void SetUp() {}
    void TearDown() {}

    static const size_t maxNumOfUsers_ = 10;
    eMU::core::user::factory_t<eMU::ut::env::core::user::userStub_t> factory_;
};

TEST_F(usersFactoryTest_t, construct) {
    eMU::ut::env::core::user::userStub_t *user = NULL;

    for(size_t i = 0; i < maxNumOfUsers_; ++i) {
        user = factory_.construct();

        ASSERT_THAT(user, ::testing::NotNull());
    }

    // maxNumOfUsers_ was reached - there are no free space for next user.
    user = factory_.construct();
    ASSERT_THAT(user, ::testing::IsNull());
}

TEST_F(usersFactoryTest_t, destroy) {
    eMU::ut::env::core::user::userStub_t *user = NULL;

    for(size_t i = 0; i < maxNumOfUsers_; ++i) {
        user = factory_.construct();

        ASSERT_THAT(user, ::testing::NotNull());
    }

    // Save last created user pointer.
    // We will need it to destroy user.
    eMU::ut::env::core::user::userStub_t *lastUser = user;

    // Make sure that maxNumOfUsers_ was reached and there are no free id for user available.
    user = factory_.construct();
    ASSERT_THAT(user, ::testing::IsNull());

    // Destroy the last created user.
    // Now we should have space for one new user.
    factory_.destroy(*lastUser);

    user = factory_.construct();
    ASSERT_THAT(user, ::testing::NotNull());

    // Make sure that maxNumOfUsers_ was reached again.
    user = factory_.construct();
    ASSERT_THAT(user, ::testing::IsNull());
}