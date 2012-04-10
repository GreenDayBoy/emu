#include "user.hpp"

eMU::core::network::connection_t<>* eMU::connectServer::user_t::connection() {
    return connection_;
}