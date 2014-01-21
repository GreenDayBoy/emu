#pragma once

#include <string>

namespace eMU
{
namespace streaming
{
namespace common
{

struct CharacterViewInfo
{
    CharacterViewInfo():
        skin_(0), race_(0), face_(0),
        faceScars_(0), hairType_(0),
        hairColor_(0),  tatoo_(0),
        skinColor_(0) {}

    CharacterViewInfo(const std::string &name,
                      uint8_t skin, uint8_t race,
                      uint8_t face, uint8_t faceScars,
                      uint8_t hairType, uint8_t hairColor,
                      uint8_t tatoo, uint8_t skinColor):
        name_(name), skin_(skin), race_(race), face_(face),
        faceScars_(faceScars), hairType_(hairType),
        hairColor_(hairColor), tatoo_(tatoo),
        skinColor_(skinColor) {}

    std::string name_;
    uint8_t skin_;
    uint8_t race_;
    uint8_t face_;
    uint8_t faceScars_;
    uint8_t hairType_;
    uint8_t hairColor_;
    uint8_t tatoo_;
    uint8_t skinColor_;
};

}
}
}
