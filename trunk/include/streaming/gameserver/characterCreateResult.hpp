#pragma once

#include <stdint.h>

namespace eMU
{
namespace streaming
{
namespace gameserver
{

enum class CharacterCreateResult
{
    Succeed,
    AlreadyExists,
    CharactersCountExceeded,
    UnknownError,
};

}
}
}
