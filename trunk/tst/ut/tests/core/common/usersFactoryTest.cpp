#include <gtest/gtest.h>

#include <core/common/usersFactory.hpp>
#include <core/common/hashableObject.hpp>

using eMU::core::common::UsersFactory;
using eMU::core::common::HashableObject;

class UsersFactoryTest: public ::testing::Test
{
protected:
    UsersFactoryTest():
        maxNumberOfUsers_(2),
        usersFactory_(maxNumberOfUsers_) {}

    class FakeUser: public HashableObject
    {

    };

    virtual void SetUp()
    {
    }

    size_t maxNumberOfUsers_;
    UsersFactory<FakeUser> usersFactory_;
};

TEST_F(UsersFactoryTest, WhenMaxNumberOfUsersReachedShouldThrowException)
{
    size_t user1 = usersFactory_.create();
    size_t user2 = usersFactory_.create();

    ASSERT_THROW(usersFactory_.create(), UsersFactory<FakeUser>::MaxNumberOfUsersReachedException);

    usersFactory_.destroy(user1);
    usersFactory_.destroy(user2);
}

TEST_F(UsersFactoryTest, WhenUserDestroyedShouldBeErasedFromFactory)
{
    size_t user1 = usersFactory_.create();
    ASSERT_EQ(1, usersFactory_.users().size());

    usersFactory_.destroy(user1);
    ASSERT_EQ(0, usersFactory_.users().size());
}

TEST_F(UsersFactoryTest, WhenInvalidHashWasGivenThenFindShouldThrowException)
{
    size_t user1 = usersFactory_.create();
    size_t user2 = usersFactory_.create();

    ASSERT_THROW(usersFactory_.find(12345), UsersFactory<FakeUser>::UnknownUserException);

    usersFactory_.destroy(user1);
    usersFactory_.destroy(user2);
}

TEST_F(UsersFactoryTest, find)
{
    size_t user1 = usersFactory_.create();
    size_t user2 = usersFactory_.create();

    FakeUser &foundUser1 = usersFactory_.find(user1);
    ASSERT_EQ(user1, foundUser1.getHash());

    FakeUser &foundUser2 = usersFactory_.find(user2);
    ASSERT_EQ(user2, foundUser2.getHash());

    usersFactory_.destroy(user1);
    usersFactory_.destroy(user2);
}

TEST_F(UsersFactoryTest, exists)
{
    size_t user1 = usersFactory_.create();

    ASSERT_TRUE(usersFactory_.exists(user1));
    ASSERT_FALSE(usersFactory_.exists(0x12643));
}
