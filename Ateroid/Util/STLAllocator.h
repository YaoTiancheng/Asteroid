#pragma once

#include <cstdlib>
#include "Debug.h"

namespace ASTEROID_NAMESPACE
{
    class AllocNormal
    {
        template<typename T, typename Alloc>
        friend class STLAllocator;

    private:
        static void* Allocate(size_t size)
        {
            return malloc(size);
        }

        static void Deallocate(void* ptr)
        {
            return free(ptr);
        }
    };


    template<size_t Alignment>
    class AllocAligned
    {
        template<typename T, typename Alloc>
        friend class STLAllocator;

    private:
        static void* Allocate(size_t size)
        {
            return _aligned_malloc(size, Alignment);
        }

        static void Deallocate(void* ptr)
        {
            return _aligned_free(ptr);
        }
    };


    template<typename T, typename Alloc>
    class STLAllocator
    {	
    public:
        static_assert(!std::is_const_v<T>,
            "The C++ Standard forbids containers of const elements "
            "because allocator<const T> is ill-formed.");

        using _Not_user_specialized = void;

        using value_type = T;

        typedef T*          pointer;
        typedef const T*    const_pointer;

        typedef T&          reference;
        typedef const T&    const_reference;

        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        template<class Other>
        struct rebind
        {
            using other = STLAllocator<Other, Alloc>;
        };

        T* address(T& val) const noexcept
        {	
            return (std::addressof(val));
        }

        const T* address(const T& val) const noexcept
        {	
            return (std::addressof(val));
        }

        constexpr STLAllocator() noexcept
        {	
        }

        constexpr STLAllocator(const STLAllocator&) noexcept = default;
        template<typename Other, typename Alloc>
        constexpr STLAllocator(const STLAllocator<Other, Alloc>&) noexcept
        {	
        }

        void deallocate(T* const ptr, const size_t count)
        {	
            Alloc::Deallocate(ptr);
        }

        T* allocate(const size_t count)
        {	
            return static_cast<T*>(Alloc::Allocate(sizeof(T) * count));
        }

        T* allocate(const size_t count, const void*)
        {	
            return (allocate(count));
        }

        template<typename Objty, typename... Types>
        void construct(Objty * const ptr, Types&&... args)
        {	
            ::new (const_cast<void*>(static_cast<const volatile void*>(ptr)))
                Objty(std::forward<Types>(args)...);
        }

        template<class Uty>
        void destroy(Uty* const _Ptr)
        {	
            _Ptr->~Uty();
        }

        size_t max_size() const noexcept
        {	
            return (static_cast<size_t>(-1) / sizeof(T));
        }

        template<typename Other>
        bool operator==(const STLAllocator<Other, Alloc>&) noexcept
        {	
            return (true);
        }

        template<typename Other>
        bool operator!=(const STLAllocator<Other, Alloc>&) noexcept
        {	
            return (false);
        }
    };


    template<typename T>
    using NormalSTLAllocator = STLAllocator<T, AllocNormal>;

    template<typename T, size_t Alignment>
    using AlignedSTLAllocator = STLAllocator<T, AllocAligned<Alignment>>;
}