//==================================================================================================
//  File:
//      container_tests.h
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef CONTAINER_TESTS_H_DEFINED
#define CONTAINER_TESTS_H_DEFINED

#include "pointer_tests.h"

template<class Alloc>
class simple_string
{
  public:
    using allocator_type     = Alloc;
    using size_type          = typename allocator_type::size_type;
    using difference_type    = typename allocator_type::difference_type;
    using void_pointer       = typename allocator_type::void_pointer;
    using const_void_pointer = typename allocator_type::const_void_pointer;
    using pointer            = typename allocator_type::pointer;
    using const_pointer      = typename allocator_type::const_pointer;
    using value_type         = char;

  public:
    ~simple_string();

    simple_string(allocator_type const& alloc=allocator_type()) noexcept;
    simple_string(simple_string&& other) noexcept;
    simple_string(simple_string const& other);
    simple_string(simple_string const& other, allocator_type const& alloc);
    simple_string(std::string const& str);
    simple_string(char const* pstr, allocator_type const& alloc=allocator_type());

    simple_string&  operator =(simple_string&& rhs) noexcept;
    simple_string&  operator =(simple_string const& rhs);
    simple_string&  operator =(std::string const& str);
    simple_string&  operator =(char const* pstr);

    char const*     c_str() const noexcept;
    bool            equal_to(simple_string const& rhs) const noexcept;
    bool            less_than(simple_string const& rhs) const noexcept;
    size_type       size() const noexcept;

    void            assign(char const* pstr);
    void            swap(simple_string& other) noexcept;

  private:
    pointer         mp_data;
    size_type       m_size;
    allocator_type  m_alloc;
};


template<class Alloc> inline
simple_string<Alloc>::~simple_string()
{
    if (mp_data != nullptr)
    {
        m_alloc.deallocate(mp_data, 1);
    }
}

template<class Alloc> inline
simple_string<Alloc>::simple_string(allocator_type const& alloc) noexcept
:   mp_data(nullptr)
,   m_size(0)
,   m_alloc(alloc)
{}

template<class Alloc> inline
simple_string<Alloc>::simple_string(simple_string&& other) noexcept
:   mp_data(other.mp_data)
,   m_size(other.m_size)
{
    other.mp_data = nullptr;
    other.m_size  = 0;
}

template<class Alloc>
simple_string<Alloc>::simple_string(simple_string const& other)
:   mp_data(nullptr)
,   m_size(0)
{
    if (other.m_size > 0)
    {
        mp_data = m_alloc.allocate(other.m_size + 1);
        m_size  = other.m_size;
        std::copy(other.mp_data, other.mp_data + m_size, mp_data);
        mp_data[m_size] = '\0';
    }
}

template<class Alloc>
simple_string<Alloc>::simple_string(simple_string const& other, allocator_type const& alloc)
:   mp_data(nullptr)
,   m_size(0)
,   m_alloc(alloc)
{
    if (other.m_size > 0)
    {
        mp_data = m_alloc.allocate(other.m_size + 1);
        m_size  = other.m_size;
        std::copy(other.mp_data, other.mp_data + m_size, mp_data);
        mp_data[m_size] = '\0';
    }
}

template<class Alloc>
simple_string<Alloc>::simple_string(std::string const& str)
:   mp_data(nullptr)
,   m_size(0)
{
    if (str.size() > 0)
    {
        mp_data = m_alloc.allocate(str.size() + 1);
        m_size  = str.size();
        std::copy(str.begin(), str.end() + m_size, mp_data);
        mp_data[m_size] = '\0';
    }
}

template<class Alloc>
simple_string<Alloc>::simple_string(char const* pstr, allocator_type const& alloc)
:   mp_data(nullptr)
,   m_size(0)
,   m_alloc(alloc)
{
    if (pstr != nullptr)
    {
        if (size_t len = std::strlen(pstr);  len > 0)
        {
            mp_data = m_alloc.allocate(len + 1);
            m_size  = len;
            std::copy(pstr, pstr + len, mp_data);
            mp_data[m_size] = '\0';
        }
    }
}

template<class Alloc>
simple_string<Alloc>&
simple_string<Alloc>::operator =(simple_string&& other) noexcept
{
    this->swap(other);
    return *this;
}

template<class Alloc>
simple_string<Alloc>&
simple_string<Alloc>::operator =(simple_string const& other)
{
    if (&other != this)
    {
        simple_string   tmp(other, m_alloc);
        this->swap(tmp);
    }
    return *this;
}

template<class Alloc>
simple_string<Alloc>&
simple_string<Alloc>::operator =(std::string const& str)
{
    simple_string   tmp(str, m_alloc);
    this->swap(tmp);
    return *this;
}

template<class Alloc>
simple_string<Alloc>&
simple_string<Alloc>::operator =(char const* pstr)
{
    simple_string   tmp(pstr, m_alloc);
    this->swap(tmp);
    return *this;
}

template<class Alloc> inline
char const*
simple_string<Alloc>::c_str() const noexcept
{
    return static_cast<char const*>(mp_data);
}

template<class Alloc> inline
bool
simple_string<Alloc>::equal_to(simple_string const& rhs) const noexcept
{
    return (m_size == rhs.m_size)  &&
           (mp_data == rhs.mp_data  ||  memcmp(mp_data, rhs.mp_data, m_size) == 0);
}

template<class Alloc> inline
bool
simple_string<Alloc>::less_than(simple_string const& rhs) const noexcept
{
    return (mp_data != nullptr)     &&
           (rhs.mp_data != nullptr) &&
           (std::char_traits<char>::compare(mp_data, rhs.mp_data, std::min(m_size, rhs.m_size)) < 0);
}

template<class Alloc> inline
typename simple_string<Alloc>::size_type
simple_string<Alloc>::size() const noexcept
{
    return m_size;
}

template<class Alloc> inline
void
simple_string<Alloc>::assign(char const* pstr)
{
    simple_string   tmp(pstr, m_alloc);
    this->swap(tmp);
}

template<class Alloc> inline
void
simple_string<Alloc>::swap(simple_string& other) noexcept
{
    std::swap(mp_data, other.mp_data);
    std::swap(m_size, other.m_size);
}


template<class Alloc> inline
bool
operator <(simple_string<Alloc> const& lhs, simple_string<Alloc> const& rhs)
{
    return lhs.less_than(rhs);
}

template<class Alloc> inline
bool
operator ==(simple_string<Alloc> const& lhs, simple_string<Alloc> const& rhs)
{
    return lhs.equal_to(rhs);
}

template<class Alloc> inline
bool
operator !=(simple_string<Alloc> const& lhs, simple_string<Alloc> const& rhs)
{
    return !lhs.equal_to(rhs);
}

template<class Alloc> inline
std::ostream&
operator <<(std::ostream& os, simple_string<Alloc> const& str)
{
    os << str.c_str();
    return os;
}

#endif  //- CONTAINER_TESTS_H_DEFINED
