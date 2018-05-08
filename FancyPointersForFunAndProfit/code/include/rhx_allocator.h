//==================================================================================================
//  File:
//      rhx_allocator.h
//
//  Summary:
//      Defines the rhx_allocator<T> class template (relocatable heap experimental allocator).
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef RHX_ALLOCATOR_H_DEFINED
#define RHX_ALLOCATOR_H_DEFINED

#include <type_traits>
#include <memory>

//--------------------------------------------------------------------------------------------------
//  Class Template:
//      rhx_allocator<T,HT>
//
//  Summary:
//      This class template implements a standard-conforming allocator that uses the pointer
//      interface and allocation strategy expressed by its second template parameter to allocate
//      memory for representing objects of type T.
//--------------------------------------------------------------------------------------------------
//
template<class T, class HT>
class rhx_allocator
{
  public:
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    using difference_type       = typename HT::difference_type;
    using size_type             = typename HT::size_type;
    using void_pointer          = typename HT::void_pointer;
    using const_void_pointer    = typename HT::const_void_pointer;
    using pointer               = typename HT::template rebind_pointer<T>;
    using const_pointer         = typename HT::template rebind_pointer<T const>;
    using reference             = T&;
    using const_reference       = T const&;
    using value_type            = T;
    using element_type          = T;

    template<class U>
    struct rebind
    {
        using other = rhx_allocator<U, HT>;
    };

  public:
    rhx_allocator();
    rhx_allocator(const rhx_allocator& src) noexcept;
    template<class U>
    rhx_allocator(const rhx_allocator<U, HT>& src) noexcept;
    ~rhx_allocator();

    rhx_allocator&  operator =(const rhx_allocator& vRhs) noexcept;

    T*          address(reference t) const noexcept;
    T const*    address(const_reference t) const noexcept;
    size_type   max_size() const noexcept;

    pointer     allocate(size_type n);
    pointer     allocate(size_type n, const_void_pointer p);
    void        deallocate(pointer p, size_type n);

    template<class U, class... Args>
    void        construct(U* p, Args&&... args);

    template<class U>
    void        destroy(U* p);

  private:
    template<class OT, class OHT> friend class rhx_allocator;

    HT          m_heap;
};


//--------------------------------------------------------------------------------------------------
//  Class Template:
//      rhx_allocator<void>
//
//  Summary:
//      This class is a partial specialization of class template rhx_allocator<T>.
//--------------------------------------------------------------------------------------------------
//
template<class HT>
class rhx_allocator<void, HT>
{
  public:
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    using difference_type       = typename HT::difference_type;
    using size_type             = typename HT::size_type;
    using void_pointer          = typename HT::void_pointer;
    using const_void_pointer    = typename HT::const_void_pointer;
    using value_type            = void;
    using element_type          = void;

    template<class U>
    struct rebind
    {
        using other = rhx_allocator<U, HT>;
    };
};


//--------------------------------------------------------------------------------------------------
//  Facility:   rhx_allocator<T>
//--------------------------------------------------------------------------------------------------
//
template<class T, class HT> inline
rhx_allocator<T, HT>::rhx_allocator()
:   m_heap()
{}

template<class T, class HT> inline
rhx_allocator<T, HT>::rhx_allocator(const rhx_allocator& src) noexcept
:   m_heap(src.m_heap)
{}

template<class T, class HT>
template<class U> inline
rhx_allocator<T, HT>::rhx_allocator(const rhx_allocator<U, HT>& src) noexcept
:   m_heap(src.m_heap)
{}

template<class T, class HT> inline
rhx_allocator<T, HT>::~rhx_allocator()
{}

template<class T, class HT> inline
rhx_allocator<T, HT>&
rhx_allocator<T, HT>::operator =(const rhx_allocator& vRhs) noexcept
{
    m_heap = vRhs.m_heap;
    return *this;
}

template<class T, class HT> inline
T*
rhx_allocator<T, HT>::address(reference t) const noexcept
{
    return &t;
}

template<class T, class HT> inline
T const*
rhx_allocator<T, HT>::address(const_reference t) const noexcept
{
    return &t;
}

template<class T, class HT> inline
typename rhx_allocator<T, HT>::size_type
rhx_allocator<T, HT>::max_size() const noexcept
{
    return m_heap.max_size() / sizeof(T);
}

template<class T, class HT> inline
typename rhx_allocator<T, HT>::pointer
rhx_allocator<T, HT>::allocate(size_type n)
{
    return static_cast<pointer>(m_heap.allocate(n * sizeof(T)));
}

template<class T, class HT> inline
typename rhx_allocator<T, HT>::pointer
rhx_allocator<T, HT>::allocate(size_type n, const_void_pointer)
{
    return static_cast<pointer>(m_heap.allocate(n * sizeof(T)));
}

template<class T, class HT> inline
void
rhx_allocator<T, HT>::deallocate(pointer p, size_type)
{
    m_heap.deallocate(p);
}

template<class T, class HT>
template<class U, class... Args> inline
void
rhx_allocator<T, HT>::construct(U* p, Args&&... args)
{
    ::new ((void*) p) U(std::forward<Args>(args)...);
}

template<class T, class HT>
template<class U> inline
void
rhx_allocator<T, HT>::destroy(U* p)
{
    p->~U();
}


//--------------------------------------------------------------------------------------------------
//  Facility:   rhx_allocator<T> Comparison Operators
//--------------------------------------------------------------------------------------------------
//
template<class T, class HT> inline bool
operator ==(const rhx_allocator<T, HT>&, const rhx_allocator<T, HT>&)
{
    return true;
}

template<class T, class HT> inline bool
operator !=(const rhx_allocator<T, HT>&, const rhx_allocator<T, HT>&)
{
    return false;
}


//--------------------------------------------------------------------------------------------------
//  Facility:   rhx_allocator<T> Object Allocation
//--------------------------------------------------------------------------------------------------
//
template<class T, class HT, class... Args>
typename rhx_allocator<T, HT>::pointer
allocate(Args&&... args)
{
    auto    pobj = rhx_allocator<T, HT>().allocate(1);

    try
    {
        rhx_allocator<T, HT>().construct(static_cast<T*>(pobj), std::forward<Args>(args)...);
    }
    catch (...)
    {
        rhx_allocator<T, HT>().deallocate(pobj, 1);
        throw;
    }

    return pobj;
}

template<class T, class Alloc, class... Args>
typename Alloc::pointer
allocate_object(Args&&... args)
{
    auto    pobj = Alloc().allocate(1);

    try
    {
        Alloc().construct(static_cast<T*>(pobj), std::forward<Args>(args)...);
    }
    catch (...)
    {
        Alloc().deallocate(pobj);
        throw;
    }

    return pobj;
}
#endif  //-  RHX_ALLOCATOR_H_DEFINED
