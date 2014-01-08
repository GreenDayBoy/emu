#pragma once

#ifdef eMU_UT

#include <ut/asioStub/ioService.hpp>

#include <ut/asioStub/tcp/socket.hpp>
#include <ut/asioStub/tcp/acceptor.hpp>

#include <ut/asioStub/udp/socket.hpp>

namespace asio = eMU::ut::env::asioStub;

#elif eMU_MT

#include <mt/asioStub/ioService.hpp>

#include <mt/asioStub/tcp/socket.hpp>
#include <mt/asioStub/tcp/acceptor.hpp>

#include <mt/asioStub/udp/socket.hpp>

namespace asio = eMU::mt::env::asioStub;

#else

#include <boost/asio.hpp>

namespace asio = boost::asio;

#endif
