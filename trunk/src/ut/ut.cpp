#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ut.hpp>

void main(int count, char *args[]) {
    testing::InitGoogleMock(&count, args);
    testing::InitGoogleTest(&count, args);

    RUN_ALL_TESTS();
}