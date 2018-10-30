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


/** Create a SharedPtr of type T */
#define ASTEROID_ALLOCATE_SHARED(T, ...)                std::allocate_shared<T, NormalSTLAllocator<T>>(NormalSTLAllocator<T>(), __VA_ARGS__)
/** Create a SharedPtr of type T using aligned allocation */
#define ASTEROID_ALLOCATE_SHARED_A(T, Alignment, ...)   std::allocate_shared<T, AlignedSTLAllocator<T, Alignment>>(AlignedSTLAllocator<T, Alignment>(), __VA_ARGS__)


namespace ASTEROID_NAMESPACE
{
    typedef Microsoft::WRL::ComPtr<ID3D11Buffer> ID3D11BufferPtr;
}


/**
 *  Container class templates
 */
#include "Util/STLAllocator.h"

namespace ASTEROID_NAMESPACE
{
    template<typename T>
    using Vector = std::vector<T, NormalSTLAllocator<T>>;

    template<typename T, size_t Alignment>
    using VectorA = std::vector<T, AlignedSTLAllocator<T, Alignment>>;

    template<typename T>
    using List = std::list<T, NormalSTLAllocator<T>>;

    template<typename T, size_t Alignment>
    using ListA = std::list<T, AlignedSTLAllocator<T, Alignment>>;

    template<typename TKey, 
        typename TValue, 
        typename Hasher = std::hash<TKey>, 
        typename Keyeq = std::equal_to<TKey>>
    using UnorderedMap = std::unordered_map<TKey,
        TValue, 
        Hasher, 
        Keyeq, 
        NormalSTLAllocator<std::pair<const TKey, TValue>>>;

    template<typename TKey,
        typename TValue,
        size_t Alignment,
        typename Hasher = std::hash<TKey>,
        typename Keyeq = std::equal_to<TKey>>
    using UnorderedMapA = std::unordered_map<TKey,
        TValue,
        Hasher,
        Keyeq,
        AlignedSTLAllocator<std::pair<const TKey, TValue>, Alignment>>;
}


namespace ASTEROID_NAMESPACE
{
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
}