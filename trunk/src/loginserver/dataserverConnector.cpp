#include <loginserver/dataserverConnector.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace loginserver
{

DataserverConnector::DataserverConnector(core::network::tcp::Connection &connection, const ConnectInfoContainer &connectInfos):
    connection_(connection),
    connectInfos_(connectInfos) {}

bool DataserverConnector::connect()
{
    for(const auto &connectInfo : connectInfos_)
    {
        if(connection_.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(connectInfo.address_),
                                                              connectInfo.port_)))
        {
            LOG(ERROR) << "Connected to dataserver address: " << connectInfo.address_ << ", port: " << connectInfo.port_;
            return true;
        }
    }

    return false;
}

}
}
