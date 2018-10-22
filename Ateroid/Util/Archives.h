#pragma once

#include "cereal/archives/json.hpp"

namespace ASTEROID_NAMESPACE
{
    typedef cereal::JSONOutputArchive JSONOutputArchive;
    typedef cereal::JSONInputArchive JSONInputArchive;
}

#ifndef ASTEROID_ARCHIVE_MAKE_NVP
#define ASTEROID_ARCHIVE_MAKE_NVP(NAME, VALUE) cereal::make_nvp(NAME, VALUE)
#endif