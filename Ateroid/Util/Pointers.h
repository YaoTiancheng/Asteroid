#pragma once

/** Create a SharedPtr of type T */
#define ASTEROID_ALLOCATE_SHARED(T, ...)                std::allocate_shared<T, NormalSTLAllocator<T>>(NormalSTLAllocator<T>(), __VA_ARGS__)
/** Create a SharedPtr of type T using aligned allocation */
#define ASTEROID_ALLOCATE_SHARED_A(T, Alignment, ...)   std::allocate_shared<T, AlignedSTLAllocator<T, Alignment>>(AlignedSTLAllocator<T, Alignment>(), __VA_ARGS__)

namespace ASTEROID_NAMESPACE
{
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
}