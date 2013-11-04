#include <glog/logging.h>
#include <gtest/gtest.h>
#include <mt/mt.hpp>

int main(int count, char *args[])
{
    google::InitGoogleLogging(args[0]);
    testing::InitGoogleTest(&count, args);

    return RUN_ALL_TESTS();
}
