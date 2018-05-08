//==================================================================================================
//  File:
//      synthetic_pointer.h
//
//  Summary:
//      Defines synthetic pointers and related services.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef SYNTHETIC_POINTER_H_DEFINED
#define SYNTHETIC_POINTER_H_DEFINED

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <iterator>

template<class From, class To>
using enable_if_convertible_t = 
    typename std::enable_if<std::is_convertible<From*, To*>::value, bool>::type;

template<class From, class To>
using enable_if_not_convertible_t = 
    typename std::enable_if<!std::is_convertible<From*, To*>::value, bool>::type;

template<class T1, class T2>
using enable_if_comparable_t =
    typename std::enable_if<std::is_convertible<T1*, T2 const*>::value ||
                            std::is_convertible<T2*, T1 const*>::value, 
                            bool>::type;

template<class T, class U>
using enable_if_non_void_t = 
    typename std::enable_if<!std::is_void<U>::value && std::is_same<T, U>::value, bool>::type;

template<class T>
using get_type_or_void_t =
    typename std::conditional<std::is_void<T>::value, void, typename std::add_lvalue_reference<T>::type>::type;


template<class T, class AM>
class syn_ptr
{
  public:
    //- Re-binder alias required for C++11 support.
    //
    template<class U>
    using rebind = syn_ptr<U, AM>;

    //- Other aliases required by allocator_traits<T>, pointer_traits<T>, and the containers.
    //
    using difference_type   = typename AM::difference_type;
    using size_type         = typename AM::size_type;
    using element_type      = T;
    using value_type        = T;
    using reference         = get_type_or_void_t<T>; //typename std::conditional<std::is_void<T>::value, void, typename std::add_lvalue_reference<T>::type>::type;
    using pointer           = syn_ptr;
    using iterator_category = std::random_access_iterator_tag;

  public:
    //- Special member functions - make intentions explicit.
    //
    ~syn_ptr() noexcept = default;

    syn_ptr() noexcept = default;
    syn_ptr(syn_ptr&&) noexcept = default;
    syn_ptr(syn_ptr const&) noexcept = default;

    syn_ptr&  operator =(syn_ptr&&) noexcept = default;
    syn_ptr&  operator =(syn_ptr const&) noexcept = default;

    //- User-defined construction.  Allow only implicit conversion at compile time.
    //
    syn_ptr(AM am);
    syn_ptr(std::nullptr_t);
    template<class U, enable_if_convertible_t<U, T> = true>
    syn_ptr(U* p);
    template<class U, enable_if_convertible_t<U, T> = true>
    syn_ptr(syn_ptr<U, AM> const& p);

    //- User-defined assignment.
    //
    syn_ptr&   operator =(std::nullptr_t);
    template<class U, enable_if_convertible_t<U, T> = true>
    syn_ptr&   operator =(U* p);
    template<class U, enable_if_convertible_t<U, T> = true>
    syn_ptr&   operator =(syn_ptr<U, AM> const& p);

    //- User-defined conversion.
    //
    explicit    operator bool() const;
    template<class U, enable_if_convertible_t<T, U> = true>
                operator U* () const;
    template<class U, enable_if_not_convertible_t<T, U> = true>
    explicit    operator U* () const;
    template<class U, enable_if_not_convertible_t<T, U> = true>
    explicit    operator syn_ptr<U, AM>() const;

    //- De-referencing and indexing.
    //
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    U*  operator ->() const;
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    U&  operator  *() const;
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    U&  operator [](size_type n) const;

    //- Pointer arithmetic operators.
    //
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    difference_type operator -(const syn_ptr& p) const;
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr         operator -(difference_type n) const;
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr         operator +(difference_type n) const;

    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr&        operator ++();
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr const   operator ++(int);
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr&        operator --();
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr const   operator --(int);
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr&        operator +=(difference_type n);
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    syn_ptr&        operator -=(difference_type n);

    //- Helper function required by pointer_traits<T>.
    //
    template<class U = T, enable_if_non_void_t<T, U> = true> 
    static  syn_ptr pointer_to(U& e);

    //- Additional helper functions used to implement the comparison operators.
    //
    bool    equals(std::nullptr_t) const;
    template<class U, enable_if_comparable_t<T, U> = true>
    bool    equals(U const* p) const;
    template<class U, enable_if_comparable_t<T, U> = true>
    bool    equals(syn_ptr<U, AM> const& p) const;

    bool    greater_than(std::nullptr_t) const;
    template<class U, enable_if_comparable_t<T, U> = true>
    bool    greater_than(U const* p) const;
    template<class U, enable_if_comparable_t<T, U> = true>
    bool    greater_than(syn_ptr<U, AM> const& p) const;

    bool    less_than(std::nullptr_t) const;
    template<class U, enable_if_comparable_t<T, U> = true>
    bool    less_than(U const* p) const;
    template<class U, enable_if_comparable_t<T, U> = true>
    bool    less_than(syn_ptr<U, AM> const& p) const;

  private:
    template<class OT, class OAM> friend class syn_ptr;   //- For conversion ctor

    AM      m_addrmodel;
};

//--------------------------------------------------------------------------------------------------
//  Facility:   syn_ptr<T,AM> implementation
//--------------------------------------------------------------------------------------------------
//
template<class T, class AM> inline
syn_ptr<T, AM>::syn_ptr(AM am)
:   m_addrmodel{am}
{}

template<class T, class AM> inline
syn_ptr<T, AM>::syn_ptr(std::nullptr_t)
:   m_addrmodel(nullptr)
{}

template<class T, class AM>
template<class U, enable_if_convertible_t<U, T>> inline
syn_ptr<T, AM>::syn_ptr(U* p)
{
    m_addrmodel.assign_from(p);
}

template<class T, class AM>
template<class U, enable_if_convertible_t<U, T>> inline
syn_ptr<T, AM>::syn_ptr(syn_ptr<U, AM> const& src)
:   m_addrmodel(src.m_addrmodel)
{}

//------
//
template<class T, class AM> inline
syn_ptr<T, AM>&
syn_ptr<T, AM>::operator =(std::nullptr_t)
{
    m_addrmodel = nullptr;
    return *this;
}

template<class T, class AM>
template<class U, enable_if_convertible_t<U, T>> inline
syn_ptr<T, AM>&
syn_ptr<T, AM>::operator =(U* p)
{
    m_addrmodel.assign_from(p);
    return *this;
}

template<class T, class AM>
template<class U, enable_if_convertible_t<U, T>> inline
syn_ptr<T, AM>&
syn_ptr<T, AM>::operator =(syn_ptr<U, AM> const& rhs)
{
    m_addrmodel = rhs.m_addrmodel;
    return *this;
}

//------
//
template<class T, class AM> inline
syn_ptr<T, AM>::operator bool() const
{
    return !m_addrmodel.equals(nullptr);
}

template<class T, class AM>
template<class U, enable_if_convertible_t<T, U>> inline
syn_ptr<T, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class T, class AM>
template<class U, enable_if_not_convertible_t<T, U>> inline
syn_ptr<T, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class T, class AM>
template<class U, enable_if_not_convertible_t<T, U>> inline
syn_ptr<T, AM>::operator syn_ptr<U, AM>() const
{
    return syn_ptr<U, AM>(m_addrmodel);
}

//------
//
template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline U*
syn_ptr<T, AM>::operator ->() const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class T, class AM> 
template<class U, enable_if_non_void_t<T, U>> 
inline U&
syn_ptr<T, AM>::operator *() const
{
    return *static_cast<U*>(m_addrmodel.address());
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline U&
syn_ptr<T, AM>::operator [](size_type n) const
{
    return static_cast<U*>(m_addrmodel.address())[n];
}

//------
//
template<class T, class AM> 
template<class U, enable_if_non_void_t<T, U>> 
inline typename syn_ptr<T, AM>::difference_type
syn_ptr<T, AM>::operator -(syn_ptr const& rhs) const
{
    uint8_t const*  p1 = static_cast<uint8_t const*>(m_addrmodel.address());
    uint8_t const*  p2 = static_cast<uint8_t const*>(rhs.m_addrmodel.address());

    return (p1 - p2) / sizeof(T);
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>
syn_ptr<T, AM>::operator -(difference_type n) const
{
    syn_ptr   tmp{*this};
    tmp.m_addrmodel.decrement(n*sizeof(T));
    return tmp;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>
syn_ptr<T, AM>::operator +(difference_type n) const
{
    syn_ptr   tmp{*this};
    tmp.m_addrmodel.increment(n*sizeof(T));
    return tmp;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>&
syn_ptr<T, AM>::operator ++()
{
    m_addrmodel.increment(sizeof(T));
    return *this;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM> const
syn_ptr<T, AM>::operator ++(int)
{
    syn_ptr   tmp{*this};
    m_addrmodel.increment(sizeof(T));
    return tmp;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>&
syn_ptr<T, AM>::operator --()
{
    m_addrmodel.decrement(sizeof(T));
    return *this;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM> const
syn_ptr<T, AM>::operator --(int)
{
    syn_ptr   tmp{*this};
    m_addrmodel.decrement(sizeof(T));
    return tmp;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>&
syn_ptr<T, AM>::operator +=(difference_type n)
{
    m_addrmodel.increment(n * sizeof(T));
    return *this;
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>&
syn_ptr<T, AM>::operator -=(difference_type n)
{
    m_addrmodel.decrement(n * sizeof(T));
    return *this;
}

//------
//
template<class T, class AM> inline
bool
syn_ptr<T, AM>::equals(std::nullptr_t) const
{
    return m_addrmodel.equals(nullptr);
}

template<class T, class AM>
template<class U, enable_if_comparable_t<T, U>> inline
bool
syn_ptr<T, AM>::equals(U const* p) const
{
    return m_addrmodel.equals(p);
}

template<class T, class AM>
template<class U, enable_if_comparable_t<T, U>> inline
bool
syn_ptr<T, AM>::equals(syn_ptr<U, AM> const& p) const
{
    return m_addrmodel.equals(p.m_addrmodel);
}

template<class T, class AM> inline
bool
syn_ptr<T, AM>::greater_than(std::nullptr_t) const
{
    return m_addrmodel.greater_than(nullptr);
}

template<class T, class AM>
template<class U, enable_if_comparable_t<T, U>> inline
bool
syn_ptr<T, AM>::greater_than(U const* p) const
{
    return m_addrmodel.greater_than(p);
}

template<class T, class AM>
template<class U, enable_if_comparable_t<T, U>> inline
bool
syn_ptr<T, AM>::greater_than(syn_ptr<U, AM> const& p) const
{
    return m_addrmodel.greater_than(p);
}

template<class T, class AM> inline
bool
syn_ptr<T, AM>::less_than(std::nullptr_t) const
{
    return m_addrmodel.less_than(nullptr);
}

template<class T, class AM>
template<class U, enable_if_comparable_t<T, U>> inline
bool
syn_ptr<T, AM>::less_than(U const* p) const
{
    return m_addrmodel.less_than(p);
}

template<class T, class AM>
template<class U, enable_if_comparable_t<T, U>> inline
bool
syn_ptr<T, AM>::less_than(syn_ptr<U, AM> const& p) const
{
    return m_addrmodel.less_than(p);
}

template<class T, class AM>
template<class U, enable_if_non_void_t<T, U>> 
inline syn_ptr<T, AM>
syn_ptr<T, AM>::pointer_to(U& e)
{
    return syn_ptr(&e);
}

//--------------------------------------------------------------------------------------------------
//  Facility:   syn_ptr<T,AM> comparison operators
//--------------------------------------------------------------------------------------------------
//
template<class T, class U, class AM> inline bool
operator ==(syn_ptr<T, AM> p1, syn_ptr<U, AM> p2)
{
    return p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator ==(syn_ptr<T, AM> p1, U p2)
{
    return p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator ==(T p1, syn_ptr<U, AM> p2)
{
    return p2.equals(p1);
}
#if defined(__clang__)
template<class T, class AM> inline bool
operator ==(syn_ptr<T, AM> p1, int p2)
{
    return p1.equals(nullptr)  &&  p2 == 0;
}

template<class U, class AM> inline bool
operator ==(int p1, syn_ptr<U, AM> p2)
{
    return p2.equals(nullptr)  &&  p1 == 0;
}
#endif
//------
//
template<class T, class U, class AM> inline bool
operator !=(syn_ptr<T, AM> p1, syn_ptr<U, AM> p2)
{
    return !p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator !=(syn_ptr<T, AM> p1, U p2)
{
    return !p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator !=(T p1, syn_ptr<U, AM> p2)
{
    return !p2.equals(p1);
}

template<class T, class AM> inline bool
operator !=(syn_ptr<T, AM> p1, int p2)
{
    return !p1.equals(nullptr)  &&  p2 == 0;
}

template<class U, class AM> inline bool
operator !=(int p1, syn_ptr<U, AM> p2)
{
    return !p2.equals(nullptr)  &&  p1 == 0;
}

//------
//
template<class T, class U, class AM> inline bool
operator <(syn_ptr<T, AM> p1, syn_ptr<U, AM> p2)
{
    return p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator <(syn_ptr<T, AM> p1, U p2)
{
    return p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator <(T p1, syn_ptr<U, AM> p2)
{
    return p2.greater_than(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator <=(syn_ptr<T, AM> p1, syn_ptr<U, AM> p2)
{
    return !p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator <=(syn_ptr<T, AM> p1, U p2)
{
    return !p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator <=(T p1, syn_ptr<U, AM> p2)
{
    return !p2.less_than(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator >(syn_ptr<T, AM> p1, syn_ptr<U, AM> p2)
{
    return p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator >(syn_ptr<T, AM> p1, U p2)
{
    return p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator >(T p1, syn_ptr<U, AM> p2)
{
    return p2.less_than(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator >=(syn_ptr<T, AM> p1, syn_ptr<U, AM> p2)
{
    return !p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator >=(syn_ptr<T, AM> p1, U p2)
{
    return !p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator >=(T p1, syn_ptr<U, AM> p2)
{
    return !p2.greater_than(p1);
}

#endif  //- SYNTHETIC_POINTER_H_DEFINED
