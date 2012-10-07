#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ut.hpp>

int main(int count, char *args[]) {
    testing::InitGoogleMock(&count, args);
    testing::InitGoogleTest(&count, args);

    return RUN_ALL_TESTS();
}