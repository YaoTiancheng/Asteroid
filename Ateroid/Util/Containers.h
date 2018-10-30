#pragma once

#include <functional>
#include "Asteroid.h"
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