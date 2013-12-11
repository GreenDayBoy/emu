#include <core/common/factory.hpp>

#include <gtest/gtest.h>

using eMU::core::common::Factory;

class FactoryTest: public ::testing::Test
{
protected:
    class SimpleObject
    {
    public:
        SimpleObject():
            value_(0) {}

        SimpleObject(int32_t value):
            value_(value) {}

        int32_t getValue() { return value_; }

        bool operator==(const SimpleObject &object) const
        {
            return this == static_cast<const SimpleObject*>(&object);
        }

        bool operator==(int32_t value) const
        {
            return value_ == value;
        }

    private:
        int32_t value_;
    };

    Factory<SimpleObject> factory_;
};

TEST_F(FactoryTest, AfterCreateObjectShouldExistsInFactory)
{
    SimpleObject &object = factory_.create();

    ASSERT_EQ(object, factory_.find(object));

    factory_.destroy(object);
}

TEST_F(FactoryTest, AfterDestroyObjectShouldNotExistInFactory)
{
    int32_t value = 1;

    SimpleObject &object = factory_.create(value);

    ASSERT_EQ(object, factory_.find(value));
    factory_.destroy(object);

    SimpleObject object2(value);

    ASSERT_THROW(factory_.find(value), Factory<SimpleObject>::ObjectNotFoundException);
}

TEST_F(FactoryTest, DestroyObjectNotBelongsToFactoryShouldThrowException)
{
    SimpleObject object;

    ASSERT_THROW(factory_.destroy(object), Factory<SimpleObject>::ObjectNotFoundException);
}
