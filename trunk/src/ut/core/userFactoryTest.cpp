#include <gtest/gtest.h>
#include "../../core/userFactory.hpp"
#include "../../core/connection.hpp"
#include "userStub.hpp"
#include "socketMock.hpp"
#include "ioServiceStub.hpp"

namespace eMUNetwork = eMU::core::network;
namespace eMUNetworkUT = eMU::ut::network;
namespace eMUUser = eMU::core::user;

class userFactoryTest_t: public ::testing::Test {
public:
    userFactoryTest_t():
      factory_(maxNumOfUsers_) {}

    void SetUp() {}

    void TearDown() {}

    static const size_t maxNumOfUsers_ = 10;
    eMUUser::factory_t<eMUNetworkUT::userStub_t> factory_;
};

TEST_F(userFactoryTest_t, construct) {
    eMUNetworkUT::userStub_t *user = NULL;

    for(size_t i = 0; i < maxNumOfUsers_; ++i) {
        user = factory_.construct();

        ASSERT_THAT(user, ::testing::NotNull());
    }

    // maxNumOfUsers_ was reached - there are no free space for next user.
    user = factory_.construct();
    ASSERT_THAT(user, ::testing::IsNull());
}

TEST_F(userFactoryTest_t, destroy) {
    eMUNetworkUT::userStub_t *user = NULL;

    for(size_t i = 0; i < maxNumOfUsers_; ++i) {
        user = factory_.construct();

        ASSERT_THAT(user, ::testing::NotNull());
    }

    // Save last created user pointer.
    // We will need it to destroy user.
    eMUNetworkUT::userStub_t *lastUser = user;

    // Make sure that maxNumOfUsers_ was reached and there are no free id for user available.
    user = factory_.construct();
    ASSERT_THAT(user, ::testing::IsNull());

    // Destroy the last created user.
    // Now we should have space for one new user.
    factory_.destroy(lastUser);

    user = factory_.construct();
    ASSERT_THAT(user, ::testing::NotNull());

    // Make sure that maxNumOfUsers_ was reached again.
    user = factory_.construct();
    ASSERT_THAT(user, ::testing::IsNull());
}