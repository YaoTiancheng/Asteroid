#pragma once

#include "resource.h"

#ifndef ASTEROID_NAMESPACE
#define ASTEROID_NAMESPACE Asteroid
#endif

#ifndef ASTEROID_NON_COPYABLE 
#define ASTEROID_NON_COPYABLE(T) T(const T&) = delete; \
    T& operator=(const T&) = delete;
#endif

#ifndef ASTEROID_NO_DEFAULT_CTOR
#define ASTEROID_NO_DEFAULT_CTOR(T) T() = delete;
#endif

namespace ASTEROID_NAMESPACE
{

    typedef Microsoft::WRL::ComPtr<ID3D11Buffer> ID3D11BufferPtr;

}