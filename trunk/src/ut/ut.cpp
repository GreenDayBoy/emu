#include "ut.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

void main(int count, char *args[]) {
    testing::InitGoogleMock(&count, args);
    testing::InitGoogleTest(&count, args);
    RUN_ALL_TESTS();

    system("PAUSE");
}