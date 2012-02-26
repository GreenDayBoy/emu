#ifndef eMUCORE_PEERSLIST_HPP
#define eMUCORE_PEERSLIST_HPP

#include <boost/noncopyable.hpp>
#include "../shared/types.hpp"
#include "peer.hpp"

namespace eMUCore {
namespace network {

template<typename PeerImpl = peer_t<> >
class peersList_t: private boost::noncopyable {
public:
    virtual ~peersList_t() {}

    void initialize(size_t maxNumOfPeers) {
        for(size_t i = 0; i < maxNumOfPeers; ++i) {
            PeerImpl::ptr_t peer(new PeerImpl(static_cast<uint32>(i)));
            list_.push_back(peer);
        }
    }

    void cleanup() {
        list_.clear();
    }

    typename PeerImpl::ptr_t findFree() {
        for(size_t i = 0; i < list_.size(); ++i)
            if(!list_[i]->connected())
                return list_[i];

        return PeerImpl::ptr_t();
    }

    template<typename Key>
    typename PeerImpl::ptr_t find(const Key &key) {
        for(size_t i = 0; i < list_.size(); ++i)
            if(*list_[i] == key)
                return list_[i];

        return PeerImpl::ptr_t();
    }

    size_t size() {
        return list_.size();
    }

private:
    std::vector<typename PeerImpl::ptr_t> list_;
};

}
}

#endif