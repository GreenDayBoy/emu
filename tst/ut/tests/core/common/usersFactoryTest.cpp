#include <gtest/gtest.h>

#include <core/common/usersFactory.hpp>
#include <core/common/hashableObject.hpp>
#include <core/common/exceptions.hpp>

namespace common = eMU::core::common;

class UsersFactoryTest: public ::testing::Test
{
protected:
    UsersFactoryTest():
        maxNumberOfUsers_(2),
        usersFactory_(maxNumberOfUsers_) {}

    class FakeUser: public common::HashableObject
    {

    };

    virtual void SetUp()
    {
    }

    size_t maxNumberOfUsers_;
    common::UsersFactory<FakeUser> usersFactory_;
};

TEST_F(UsersFactoryTest, WhenMaxNumberOfUsersReachedShouldThrowException)
{
    FakeUser &user1 = usersFactory_.create();
    FakeUser &user2 = usersFactory_.create();

    bool exceptionThrown = false;
    try
    {
        usersFactory_.create();
    }
    catch(common::exceptions::MaxNumberOfUsersReachedException &exception)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);

    usersFactory_.destroy(user1.hash());
    usersFactory_.destroy(user2.hash());
}

TEST_F(UsersFactoryTest, WhenUserDestroyedShouldBeErasedFromFactory)
{
    FakeUser &user1 = usersFactory_.create();
    ASSERT_EQ(1, usersFactory_.users().size());

    usersFactory_.destroy(user1.hash());
    ASSERT_EQ(0, usersFactory_.users().size());
}

TEST_F(UsersFactoryTest, WhenInvalidHashWasGivenThenFindShouldThrowException)
{
    FakeUser &user1 = usersFactory_.create();
    FakeUser &user2 = usersFactory_.create();

    bool exceptionThrown = false;
    try
    {
        usersFactory_.find(12345);
    }
    catch(common::exceptions::UnknownUserException &exception)
    {
        exceptionThrown = true;
    }

    ASSERT_TRUE(exceptionThrown);

    usersFactory_.destroy(user1.hash());
    usersFactory_.destroy(user2.hash());
}

TEST_F(UsersFactoryTest, find)
{
    FakeUser &user1 = usersFactory_.create();
    FakeUser &user2 = usersFactory_.create();

    FakeUser &foundUser1 = usersFactory_.find(user1.hash());
    ASSERT_EQ(user1.hash(), foundUser1.hash());

    FakeUser &foundUser2 = usersFactory_.find(user2.hash());
    ASSERT_EQ(user2.hash(), foundUser2.hash());

    usersFactory_.destroy(user1.hash());
    usersFactory_.destroy(user2.hash());
}
