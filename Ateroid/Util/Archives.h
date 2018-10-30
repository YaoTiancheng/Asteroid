#pragma once

#include "cereal/archives/json.hpp"

namespace ASTEROID_NAMESPACE
{
    using JSONOutputArchive = cereal::JSONOutputArchive;
    using JSONInputArchive  = cereal::JSONInputArchive;
}

#ifndef ASTEROID_ARCHIVE_MAKE_NVP
#define ASTEROID_ARCHIVE_MAKE_NVP(NAME, VALUE) cereal::make_nvp(NAME, VALUE)
#endif