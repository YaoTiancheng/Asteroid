#pragma once

#include "resource.h"

#define ASTEROID_NAMESPACE Asteroid


#define ASTEROID_NON_COPYABLE(T)    T(const T&) = delete; \
                                    T& operator=(const T&) = delete;
#define ASTEROID_NO_DEFAULT_CTOR(T) T() = delete;


#ifdef _DEBUG
    #define ASTEROID_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#else
    #define ASTEROID_NEW new 
#endif

#define ASTEROID_DELETE delete


namespace ASTEROID_NAMESPACE
{
    typedef Microsoft::WRL::ComPtr<ID3D11Buffer> ID3D11BufferPtr;
}

