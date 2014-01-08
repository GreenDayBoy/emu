#include <core/common/logging.hpp>
#include <gtest/gtest.h>
#include <mt/mt.hpp>

int main(int count, char *args[])
{
    testing::InitGoogleTest(&count, args);

    return RUN_ALL_TESTS();
}
