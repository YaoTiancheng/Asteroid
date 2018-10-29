#pragma once

#include <cstdlib>
#include "Debug.h"

namespace ASTEROID_NAMESPACE
{
    enum class EAllocPolicy
    {
        eNormal, ePool
    };


    template<EAllocPolicy Policy>
    class Alloc
    {
        template<typename T, typename Alloc>
        friend class STLAllocator;

    private:
        static void* Allocate(size_t size)
        {
            ASTEROID_LOG_INFO_TRACE_F("Allocated %d bytes", size);
            return malloc(size);
        }

        static void Deallocate(void* ptr)
        {
            ASTEROID_LOG_INFO_TRACE_F("Deallocated");
            return free(ptr);
        }
    };


    template<EAllocPolicy Policy, size_t Alignment>
    class AllocAligned
    {
        template<typename T, typename Alloc>
        friend class STLAllocator;

    private:
        static void* Allocate(size_t size)
        {
            ASTEROID_LOG_INFO_TRACE_F("Allocated aligned %d bytes", size);
            return _aligned_malloc(size, Alignment);
        }

        static void Deallocate(void* ptr)
        {
            ASTEROID_LOG_INFO_TRACE_F("Deallocated aligned");
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

        typedef T * pointer;
        typedef const T * const_pointer;

        typedef T& reference;
        typedef const T& const_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        template<class _Other>
        struct rebind
        {
            using other = STLAllocator<_Other, Alloc>;
        };

        T* address(T& _Val) const noexcept
        {	// return address of mutable _Val
            return (std::addressof(_Val));
        }

        const T* address(const T& _Val) const noexcept
        {	// return address of nonmutable _Val
            return (std::addressof(_Val));
        }

        constexpr STLAllocator() noexcept
        {	// construct default allocator (do nothing)
        }

        constexpr STLAllocator(const STLAllocator&) noexcept = default;
        template<typename _Other, typename Alloc>
        constexpr STLAllocator(const STLAllocator<_Other, Alloc>&) noexcept
        {	// construct from a related allocator (do nothing)
        }

        void deallocate(T* const ptr, const size_t count)
        {	
            // deallocate object at ptr
            Alloc::Deallocate(ptr);
        }

        T* allocate(const size_t count)
        {	
            // allocate array of count elements
            return static_cast<T*>(Alloc::Allocate(sizeof(T) * count));
        }

        T* allocate(const size_t _Count, const void *)
        {	
            // allocate array of _Count elements, ignore hint
            return (allocate(_Count));
        }

        template<typename _Objty, typename... _Types>
        void construct(_Objty * const _Ptr, _Types&&... _Args)
        {	
            // construct _Objty(_Types...) at _Ptr
            ::new (const_cast<void*>(static_cast<const volatile void*>(_Ptr)))
                _Objty(std::forward<_Types>(_Args)...);
        }

        template<class _Uty>
        void destroy(_Uty* const _Ptr)
        {	
            // destroy object at _Ptr
            _Ptr->~_Uty();
        }

        size_t max_size() const noexcept
        {	// estimate maximum array size
            return (static_cast<size_t>(-1) / sizeof(T));
        }
    };


    
}