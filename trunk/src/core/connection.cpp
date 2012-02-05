#include <boost/bind.hpp>
#include "connection.hpp"
#include "log.hpp"

eMUCore::network::connection_t::connection_t(socket_t *socket,
                                             eventHandler_t *eventHandler):
  socket_(socket),
  eventHandler_(eventHandler) {
    socket_->queueRead(&rbuf_.payload_[0],
                       maxPayloadSize_,
                       boost::bind(&connection_t::receiveHandler,
                                   this,
                                   boost::asio::placeholders::error,
                                   boost::asio::placeholders::bytes_transferred));
}

void eMUCore::network::connection_t::send(const uint8 *payload, size_t size) {
    bool result = wbuf_.insert(payload, size);

    if(!result) {
        LOG_ERROR << "Cannot insert payload into write buffer." << std::endl;
        this->disconnect();
        return;
    }

    if(!wbuf_.pending_) {
        wbuf_.pending_ = true;

        socket_->queueWrite(&wbuf_.payload_[0],
                            wbuf_.payloadSize_,
                            boost::bind(&connection_t::sendHandler,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    }
}

void eMUCore::network::connection_t::disconnect() {
    eventHandler_->onClose();
    socket_->close();
}

void eMUCore::network::connection_t::receiveHandler(const boost::system::error_code& ec,
                                                    size_t bytesTransferred) {
    if(ec) {
        LOG_ERROR << "Error during handling receive operation, error: " << ec.message() << std::endl;
        this->disconnect();
        return;
    }

    if(bytesTransferred > 0) {
        eventHandler_->onReceive(&rbuf_.payload_[0], bytesTransferred);
        socket_->queueRead(&rbuf_.payload_[0],
                           maxPayloadSize_,
                           boost::bind(&connection_t::receiveHandler,
                                       this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred));
    } else {
        this->disconnect();
    }
}

void eMUCore::network::connection_t::sendHandler(const boost::system::error_code& ec,
                                                 size_t bytesTransferred) {
    if(ec) {
        LOG_ERROR << "Error during handling sent operation, error: " << ec.message() << std::endl;
        this->disconnect();
        return;
    }

    if(wbuf_.secPayloadSize_ > 0) {
        wbuf_.swap();
        socket_->queueWrite(&wbuf_.payload_[0],
                            wbuf_.payloadSize_,
                            boost::bind(&connection_t::sendHandler,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    }
    else
        wbuf_.clear();
}