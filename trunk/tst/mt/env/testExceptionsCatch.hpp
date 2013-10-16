#pragma once

#define TEST_EXCEPTIONS_CATCH \
    catch(asioStub::BaseSocket::NullBufferException&) \
    { \
        ASSERT_TRUE(false) << "async operation was not queued."; \
    } \
    catch(asioStub::BaseSocket::TooBigPayloadException&) \
    { \
        ASSERT_TRUE(false) << "trying to insert too big payload to tested object."; \
    } \
    catch(asioStub::io_service::UnknownSocketException&) \
    { \
        ASSERT_TRUE(false) << "socket does not exists."; \
    } \
    catch(asioStub::io_service::NotCreatedUdpSocketException&) \
    { \
        ASSERT_TRUE(false) << "udp socket not created."; \
    } \
