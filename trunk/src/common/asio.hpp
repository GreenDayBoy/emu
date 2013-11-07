#pragma once

#ifdef eMU_UT

#include <ut/env/asioStub/ioService.hpp>

#include <ut/env/asioStub/tcp/socket.hpp>
#include <ut/env/asioStub/tcp/acceptor.hpp>

#include <ut/env/asioStub/udp/socket.hpp>

#elif eMU_MT

#include <mt/env/asioStub/ioService.hpp>

#include <mt/env/asioStub/tcp/socket.hpp>
#include <mt/env/asioStub/tcp/acceptor.hpp>

#include <mt/env/asioStub/udp/socket.hpp>

#else

#include <boost/asio.hpp>

#endif

#ifdef eMU_UT

namespace asio = eMU::ut::env::asioStub;

#elif eMU_MT

namespace asio = eMU::mt::env::asioStub;

#else

namespace asio = boost::asio;

#endif
