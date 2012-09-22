#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <core/network/udp/udpWriteBufferManager.hpp>

TEST(udpWriteBufferManagerTest, get) {
    eMU::core::network::udp::writeBufferManager_t wbufManager;

    // Create buffer for given endpoint.
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::writeBuffer_t *wbuf = wbufManager.get(endpoint);

    // Check if buffer was created successful.
    ASSERT_THAT(wbuf, ::testing::NotNull());

    // Get buffer for previously created endpoint.
    eMU::core::network::writeBuffer_t *wbuf2 = wbufManager.get(endpoint);

    // wbufManager should return the same pointer because we didn't freed buffer for given endpoint.
    EXPECT_EQ(wbuf, wbuf2);

    // Create buffer for another endpoint.
    boost::asio::ip::udp::endpoint endpoint3(boost::asio::ip::address::from_string("5.4.3.2"), 1234);
    eMU::core::network::writeBuffer_t *wbuf3 = wbufManager.get(endpoint3);

    // Check if buffer created successfully.
    ASSERT_THAT(wbuf3, ::testing::NotNull());

    // Check if buffer is different than previously created one. Just in case :-).
    ASSERT_NE(wbuf, wbuf3);
}

TEST(udpWriteBufferManagerTest, free) {
    eMU::core::network::udp::writeBufferManager_t wbufManager;

    // Create buffer for given endpoint.
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("1.2.3.4"), 1234);
    eMU::core::network::writeBuffer_t *wbuf = wbufManager.get(endpoint);

    // Check if buffer created successful.
    ASSERT_THAT(wbuf, ::testing::NotNull());

    // Prepare payloads that we insert to created buffer.
    eMU::core::network::payload_t priPayload(100, 0x10);
    eMU::core::network::payload_t secPayload(100, 0x12);

    // Insert previous prepared payloads to buffer.
    wbuf->payload_ = priPayload;
    wbuf->secPayload_ = secPayload;
    wbuf->pending_ = true;

    // Free buffer for given endpoint.
    wbufManager.free(endpoint);

    // Create new buffer for the same endpoint.
    wbuf = wbufManager.get(endpoint);

    // Check if buffer created successful.
    ASSERT_THAT(wbuf, ::testing::NotNull());

    // We need to check if buffer after freed was cleared (doesn't contain the same data).
    // We cannot simply compare pointers of old and new buffer
    // because they can be allocated at the same place in memory.
    EXPECT_NE(wbuf->payload_, priPayload);
    EXPECT_NE(wbuf->secPayload_, secPayload);
    EXPECT_FALSE(wbuf->pending_);
}