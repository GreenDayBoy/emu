#include <gtest/gtest.h>

#include <core/user/usersFactory.hpp>
#include <env/core/user/userStub.hpp>

class usersFactoryTest_t: public ::testing::Test {
public:
    usersFactoryTest_t():
      factory_(numOfIds_) {}

    void SetUp() {}
    void TearDown() {}

    static const size_t numOfIds_ = 10;
    eMU::core::user::factory_t<eMU::ut::env::core::user::userStub_t> factory_;
};

TEST_F(usersFactoryTest_t, construct_and_destroy) {
    eMU::ut::env::core::user::userStub_t *user = NULL;

    for(size_t i = 0; i < numOfIds_ * 2; ++i) {
        eMU::ut::env::core::user::userStub_t &user = factory_.construct();

        factory_.destroy(user);
    }
}
