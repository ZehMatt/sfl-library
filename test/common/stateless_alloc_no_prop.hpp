#ifndef SFL_STATELESS_ALLOC_NO_PROP_HPP
#define SFL_STATELESS_ALLOC_NO_PROP_HPP

#include <cstddef>
#include <type_traits>

namespace sfl
{
namespace test
{

template<typename T>
class stateless_alloc_no_prop
{
public:

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap            = std::false_type;

    template <typename U>
    struct rebind
    {
        using other = stateless_alloc_no_prop<U>;
    };

    //
    // ---- CONSTRUCTION AND DESTRUCTION --------------------------------------
    //

    stateless_alloc_no_prop() noexcept
    {}

    stateless_alloc_no_prop(const stateless_alloc_no_prop& /*other*/) noexcept
    {}

    template <typename U>
    stateless_alloc_no_prop(const stateless_alloc_no_prop<U>& /*other*/) noexcept
    {}

    stateless_alloc_no_prop(stateless_alloc_no_prop&& /*other*/) noexcept
    {}

    template <typename U>
    stateless_alloc_no_prop(stateless_alloc_no_prop<U>&& /*other*/) noexcept
    {}

    ~stateless_alloc_no_prop() noexcept
    {}

    //
    // ---- ASSIGNMENT --------------------------------------------------------
    //

    stateless_alloc_no_prop& operator=(const stateless_alloc_no_prop& /*other*/) noexcept
    {
        return *this;
    }

    stateless_alloc_no_prop& operator=(stateless_alloc_no_prop&& /*other*/) noexcept
    {
        return *this;
    }

    //
    // ---- ALLOCATE AND DEALLOACTE -------------------------------------------
    //

    T* allocate(size_type n, const void* = nullptr)
    {
        if (n > max_size())
        {
            #ifdef SFL_NO_EXCEPTIONS
            assert(!"n > max_size()");
            std::abort();
            #else
            throw std::bad_alloc();
            #endif
        }

        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_type)
    {
        ::operator delete(p);
    }

    //
    // ---- CONSTRUCT AND DESTROY ---------------------------------------------
    //

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) noexcept(
        std::is_nothrow_constructible<U, Args...>::value
    )
    {
        ::new ((void *)p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p) noexcept(std::is_nothrow_destructible<U>::value)
    {
        p->~U();
    }

    //
    // ---- ADDRESS -----------------------------------------------------------
    //

    pointer address(reference x) const noexcept
    {
        return std::addressof(x);
    }

    const_pointer address(const_reference x) const noexcept
    {
        return std::addressof(x);
    }

    //
    // ---- MAX SIZE ----------------------------------------------------------
    //

    size_type max_size() const noexcept
    {
        return std::size_t(PTRDIFF_MAX) / sizeof(T);
    }

    //
    // ---- COMPARISONS -------------------------------------------------------
    //

    template <typename T1, typename T2>
    friend bool operator==
    (
        const stateless_alloc_no_prop<T1>& /*x*/,
        const stateless_alloc_no_prop<T2>& /*y*/
    ) noexcept;

    template <typename T1, typename T2>
    friend bool operator!=
    (
        const stateless_alloc_no_prop<T1>& /*x*/,
        const stateless_alloc_no_prop<T2>& /*y*/
    ) noexcept;
};

template <typename T1, typename T2>
bool operator==
(
    const stateless_alloc_no_prop<T1>& /*x*/,
    const stateless_alloc_no_prop<T2>& /*y*/
) noexcept
{
    return true;
}

template <typename T1, typename T2>
bool operator!=
(
    const stateless_alloc_no_prop<T1>& /*x*/,
    const stateless_alloc_no_prop<T2>& /*y*/
) noexcept
{
    return false;
}

} // namespace test
} // namespace sfl

#endif // SFL_STATELESS_ALLOC_NO_PROP_HPP
