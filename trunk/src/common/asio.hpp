#pragma once

#ifdef eMU_UT

#include <ut/env/asioStub/ioService.hpp>

#include <ut/env/asioStub/tcp/socket.hpp>
#include <ut/env/asioStub/tcp/acceptor.hpp>

#include <ut/env/asioStub/udp/socket.hpp>

namespace asio = eMU::ut::env::asioStub;

#elif eMU_MT

#include <mt/env/asioStub/ioService.hpp>

#include <mt/env/asioStub/tcp/socket.hpp>
#include <mt/env/asioStub/tcp/acceptor.hpp>

#include <mt/env/asioStub/udp/socket.hpp>

namespace asio = eMU::mt::env::asioStub;

#else

#include <boost/asio.hpp>

namespace asio = boost::asio;

#endif
