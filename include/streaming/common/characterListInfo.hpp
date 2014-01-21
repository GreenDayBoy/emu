#pragma once

#include <string>
#include <vector>

namespace eMU
{
namespace streaming
{
namespace common
{

struct CharacterListInfo
{
    CharacterListInfo():
        level_(0),
        race_(0),
        hairType_(0),
        hairColor_(0),
        tutorialState_(0) {}

    CharacterListInfo(const std::string &name,
                      uint8_t level,
                      uint8_t race,
                      uint8_t hairType,
                      uint8_t hairColor,
                      uint16_t tutorialState):
        name_(name),
        level_(level),
        race_(race),
        hairType_(hairType),
        hairColor_(hairColor),
        tutorialState_(tutorialState) {}

    std::string name_;
    uint8_t level_;
    uint8_t race_;
    uint8_t hairType_;
    uint8_t hairColor_;
    uint16_t tutorialState_;
};

typedef std::vector<CharacterListInfo> CharacterInfoContainer;

}
}
}
