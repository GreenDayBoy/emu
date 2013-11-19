#include <protocol/readStreamsExtractor.hpp>

#include <string.h>

namespace eMU
{
namespace protocol
{

//StreamsExtractor::StreamsExtractor(const network::Payload &payload):
//    payload_(payload) {}

//void PacketsExtractor::extract()
//{
//    if(payload_.empty())
//    {
//        throw EmptyPayloadException();
//    }

//    size_t totalSize = 0;

//    while(totalSize < payload_.getSize())
//    {
//        try
//        {
//            size_t packetSize = this->getPacketSize(totalSize);
//            size_t dataOffset = this->calculateDataOffset(totalSize, packetSize);

//            // When I will be familiar with rvalue references then I will extract this code to separated method :D...
//            network::Payload packet;
//            memcpy(&packet[0], &payload_[dataOffset], packetSize);
//            packet.setSize(packetSize);

//            packets_.push_back(std::move(packet));

//            totalSize += packetSize + sizeof(uint32_t);
//        }
//        catch(const network::Payload::GetOverflowException&)
//        {
//            throw PacketSizeOutOfBoundException();
//        }
//    }
//}

//PacketsExtractor::PacketsContainer &PacketsExtractor::getPackets()
//{
//    return packets_;
//}

//size_t PacketsExtractor::getPacketSize(size_t offset) const
//{
//    size_t size = payload_.getValue<uint32_t>(offset);

//    if(size == 0)
//    {
//        throw NullPacketSizeException();
//    }

//    return size;
//}

//size_t PacketsExtractor::calculateDataOffset(size_t offset, size_t packetSize) const
//{
//    size_t dataOffset = (offset + sizeof(uint32_t));

//    if(dataOffset + packetSize > payload_.getSize())
//    {
//        throw PacketSizeOutOfBoundException();
//    }

//    return dataOffset;
//}

}
}
