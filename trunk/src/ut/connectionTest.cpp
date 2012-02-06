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

class connectionTestFixture: public ::testing::Test {
protected:
    virtual void SetUp() {
        initializePatternBuf();
    }

    void initializePatternBuf() {
        uint8 value = 0;
        for(size_t i = 0; i < eMUCore::network::maxPayloadSize_c; ++i)
            patternBuf_.payload_[i] = value++;
    }

    eMUCore::network::readBuffer_t patternBuf_;
};

TEST_F(connectionTestFixture, construct) {
    mockSocket_t socket(NULL);
    mockEventHandler_t eventHandler;

    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);
}

TEST_F(connectionTestFixture, send) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    socket.fake_.dequeueWrite(boost::system::error_code(), eMUCore::network::maxPayloadSize_c);

    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);
}

TEST_F(connectionTestFixture, send_Pending) {
    size_t testPayloadSize = eMUCore::network::maxPayloadSize_c / 2;
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();
    
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    connection.send(&patternBuf_.payload_[0], testPayloadSize);
    connection.send(&patternBuf_.payload_[testPayloadSize], testPayloadSize);

    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    socket.fake_.dequeueWrite(boost::system::error_code(), eMUCore::network::maxPayloadSize_c);
}

TEST_F(connectionTestFixture, send_Error) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();
    
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    socket.fake_.dequeueWrite(boost::asio::error::connection_reset, 0);   
}

TEST_F(connectionTestFixture, send_Overflow) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(socket, queueWrite(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);

    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    connection.send(&patternBuf_.payload_[0], eMUCore::network::maxPayloadSize_c);
}

TEST_F(connectionTestFixture, receive) {
    size_t readPayloadSize = 30;
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();
    
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(eventHandler, onReceive(::testing::NotNull(), readPayloadSize));
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    socket.fake_.dequeueRead(boost::system::error_code(), readPayloadSize);
}

TEST_F(connectionTestFixture, receive_Close) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();
    
    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    socket.fake_.dequeueRead(boost::system::error_code(), 0);
}

TEST_F(connectionTestFixture, receive_Error) {
    mockEventHandler_t eventHandler;
    ::testing::StrictMock<mockSocket_t> socket(reinterpret_cast<boost::asio::ip::tcp::socket*>(NULL));
    socket.delegateMethods();

    EXPECT_CALL(socket, queueRead(::testing::NotNull(), eMUCore::network::maxPayloadSize_c, ::testing::_));
    eMUCore::network::connection_t connection(&socket, &eventHandler);

    EXPECT_CALL(eventHandler, onClose());
    EXPECT_CALL(socket, close());
    socket.fake_.dequeueRead(boost::asio::error::connection_reset, 0);
}