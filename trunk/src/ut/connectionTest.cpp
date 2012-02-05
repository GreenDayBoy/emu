#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../core/connection.hpp"

class fakeSocket_t: public eMUCore::network::socket_t {
public:
    fakeSocket_t(boost::asio::ip::tcp::socket *socket):
      socket_t(socket) {}

    void close() {}

    void queueRead(uint8 *payload, size_t size, ioHandler_t handler) {
        readHandler_ = handler;
    }

    void dequeueRead(const boost::system::error_code& ec, size_t bytesTransferred) {
        readHandler_(ec, bytesTransferred);
    }
        
    void queueWrite(const uint8 *payload, size_t size, ioHandler_t handler) {
        writeHandler_ = handler;
    }

    void dequeueWrite(const boost::system::error_code& ec, size_t bytesTransferred) {
        writeHandler_(ec, bytesTransferred);
    }

private:
    ioHandler_t readHandler_;
    ioHandler_t writeHandler_;
};

class mockSocket_t: public eMUCore::network::socket_t {
public:
    mockSocket_t(boost::asio::ip::tcp::socket *socket):
        socket_t(socket),
        fake_(socket) {}

    void delegateMethods() {
        ON_CALL(*this, queueRead(::testing::_,
                                 ::testing::_,
                                 ::testing::_)).WillByDefault(::testing::Invoke(&fake_,
                                                                                &fakeSocket_t::queueRead));
        ON_CALL(*this, queueWrite(::testing::_,
                                  ::testing::_,
                                  ::testing::_)).WillByDefault(::testing::Invoke(&fake_,
                                                                                 &fakeSocket_t::queueWrite));
    }

    MOCK_METHOD0(close, void());
    MOCK_METHOD3(queueRead, void(uint8 *payload, size_t size, eMUCore::network::socket_t::ioHandler_t handler));
    MOCK_METHOD3(queueWrite, void(const uint8 *payload, size_t size, eMUCore::network::socket_t::ioHandler_t handler));

    fakeSocket_t fake_;
};

class mockEventHandler_t: public eMUCore::network::connection_t::eventHandler_t {
public:
    MOCK_METHOD2(onReceive, void(uint8 *payload, size_t size));
    MOCK_METHOD0(onClose, void());
};

class connectionTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        initializePatternBuf();
    }

    void initializePatternBuf() {
        uint8 value = 0;
        for(size_t i = 0; i < eMUCore::network::maxPayloadSize_; ++i)
            patternBuf_.payload_[i] = value++;
    }

    eMUCore::network::readBuffer_t patternBuf_;
};

TEST_F(connectionTest, construct) {
    mockSocket_t socket(NULL);
    mockEventHandler_t eventHandler;

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);
}

TEST_F(connectionTest, send) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 2. Expect queue write operation after send() call.
    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);

    // Dequeue earlier send() call and unlock write buffer.
    socket.fake_.dequeueWrite(boost::system::error_code(), eMUCore::network::maxPayloadSize_);

    // 3. Expect queue write after send call() and dequeue previous send().
    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);
}

TEST_F(connectionTest, send_Pending) {
    size_t testPayloadSize = eMUCore::network::maxPayloadSize_ / 2;
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 2. Expect queue write operation after send() call.
    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);

    // This send payloads should be add to second of wbuf payload
    // because master payload is currently pending.
    connection.send(&patternBuf_.payload_[0], testPayloadSize);
    connection.send(&patternBuf_.payload_[testPayloadSize], testPayloadSize);

    // 3. Dequeue previous write operation. It should trigger queueWrite with second payload from wbuf.
    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    socket.fake_.dequeueWrite(boost::system::error_code(), eMUCore::network::maxPayloadSize_);
}

TEST_F(connectionTest, send_Error) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 2. Expect queue write operation after send() call.
    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);

    // 3. Expect close connection after dequeue error.
    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    socket.fake_.dequeueWrite(boost::asio::error::connection_reset, 0);   
}

TEST_F(connectionTest, send_Overflow) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 2. Expect queue write operation after send() call.
    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);

    // This send payload should be add to second of wbuf payload
    // because main payload is currently pending.
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);

    // 3. Expect close connection because main payload is currently pending and second payload is full.
    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_);
}

TEST_F(connectionTest, receive) {
    size_t readPayloadSize = 30;
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 3. Expect call onReceive event and next queue read, after dequeue previous read operation
    // with non-zero payload size.
    EXPECT_CALL(eventHandler, onReceive(::testing::NotNull(), readPayloadSize));
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    socket.fake_.dequeueRead(boost::system::error_code(), readPayloadSize);
}

TEST_F(connectionTest, receive_Close) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 2. Expect close connection after dequeue read operation with zero payload size.
    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    socket.fake_.dequeueRead(boost::system::error_code(), 0);
}

TEST_F(connectionTest, receive_Error) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    // 1. Expect queue read operation after connection construct.
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    // 3. Expect close connection after dequeue error.
    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    socket.fake_.dequeueRead(boost::asio::error::connection_reset, 0);
}