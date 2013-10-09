#pragma once

#define TEST_EXCEPTIONS_CATCH \
    catch(asioStub::exceptions::NullBufferException&) \
    { \
        ASSERT_TRUE(false) << "async operation was not queued."; \
    } \
    catch(asioStub::exceptions::TooBigPayloadException&) \
    { \
        ASSERT_TRUE(false) << "trying to insert too big payload to tested object."; \
    } \
    catch(asioStub::exceptions::UnknownSocketException&) \
    { \
        ASSERT_TRUE(false) << "socket does not exists."; \
    } \
    catch(asioStub::exceptions::NotCreatedUdpSocketException&) \
    { \
        ASSERT_TRUE(false) << "udp socket not created."; \
    } \
    catch(eMU::core::protocol::exceptions::EmptyPayloadException&) \
    { \
        ASSERT_TRUE(false) << "empty payload to verify!"; \
    }
