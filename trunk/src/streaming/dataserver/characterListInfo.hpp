#pragma once

#include <string>
#include <vector>

namespace eMU
{
namespace streaming
{
namespace dataserver
{

struct CharacterListInfo
{
    CharacterListInfo():
        level_(0),
        race_(0),
        hairType_(0),
        hairColor_(0) {}

    std::string name_;
    uint8_t level_;
    uint8_t race_;
    uint8_t hairType_;
    uint8_t hairColor_;
};

typedef std::vector<CharacterListInfo> CharacterListInfoContainer;

}
}
}
