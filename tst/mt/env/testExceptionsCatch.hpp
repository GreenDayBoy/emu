#pragma once

#include <mt/env/dataserver/database/sqlInterfaceStub.hpp>

#define IO_SERVICE_EXCEPTIONS_CATCH \
catch(const eMU::mt::env::asioStub::io_service::AcceptNotStartedException&) \
{ \
    ASSERT_TRUE(false) << "accept not queued."; \
} \
catch(const eMU::mt::env::asioStub::io_service::NonExistentTcpSocketException&) \
{ \
    ASSERT_TRUE(false) << "tcp socket does not exists."; \
} \
catch(const eMU::mt::env::asioStub::io_service::NonExistentUdpSocketException&) \
{ \
    ASSERT_TRUE(false) << "udp socket does not exists."; \
} \
catch(const eMU::mt::env::asioStub::io_service::ClientSocketNotConnectedException&) \
{ \
    ASSERT_TRUE(false) << "payload size out of bound."; \
}

#define SOCKET_EXCEPTIONS_CATCH \
catch(const eMU::mt::env::asioStub::BaseSocket::ReceiveNotStartedException&) \
{ \
    ASSERT_TRUE(false) << "receive operation was not queued."; \
} \
catch(const eMU::mt::env::asioStub::BaseSocket::SendNotStartedException&) \
{ \
    ASSERT_TRUE(false) << "send operation was not queued."; \
} \
catch(const eMU::mt::env::asioStub::BaseSocket::PayloadSizeOutOfBoundException&) \
{ \
    ASSERT_TRUE(false) << "payload size out of bound."; \
}

#define SQL_EXCEPTIONS_CATCH \
catch(const eMU::mt::env::dataserver::database::SqlInterfaceStub::UnexpectedQueryExecutionException&) \
{ \
    ASSERT_TRUE(false) << "unexpected sql query execution."; \
} \
catch(const eMU::mt::env::dataserver::database::SqlInterfaceStub::UnexpectedFetchResultExecutionException&) \
{ \
    ASSERT_TRUE(false) << "unexpected fetch of sql result."; \
}

#define IO_CHECK(statement) \
    try { \
        statement; \
    } \
    IO_SERVICE_EXCEPTIONS_CATCH \
    SOCKET_EXCEPTIONS_CATCH \
    SQL_EXCEPTIONS_CATCH
