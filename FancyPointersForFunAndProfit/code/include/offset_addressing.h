//==================================================================================================
//  File:
//      offset_addressing.h
//
//  Summary:
//      Defines a simple offset addressing model as a class template.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef OFFSET_ADDRESSING_H_DEFINED
#define OFFSET_ADDRESSING_H_DEFINED

#include <cstddef>
#include <cstdint>

//--------------------------------------------------------------------------------------------------
//  Class:
//      offset_addressing_model
//
//  Summary:
//      This class implements a based (base + offset) addressing model.  The offset is stored
//      as a 64-but integer, and the base address is provided by a segment address from an 
//      instance of the template argument SM.
//
//      Note that the comparison helper functions include several that define the "greater_than"
//      relationship.  They are included because they are trivial, and make the code for any
//      synthetic pointer wrapper class comparison operators easier to implement and read.
//--------------------------------------------------------------------------------------------------
//
class alignas(sizeof(std::ptrdiff_t))  offset_addressing_model
{
  public:
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

  public:
    ~offset_addressing_model() = default;

    offset_addressing_model() noexcept;
    offset_addressing_model(offset_addressing_model&& other) noexcept;
    offset_addressing_model(offset_addressing_model const& other) noexcept;
    offset_addressing_model(std::nullptr_t) noexcept;
    offset_addressing_model(void const* p) noexcept;

    offset_addressing_model&  operator =(offset_addressing_model&& rhs) noexcept;
    offset_addressing_model&  operator =(offset_addressing_model const& rhs) noexcept;
    offset_addressing_model&  operator =(std::nullptr_t) noexcept;
    offset_addressing_model&  operator =(void const* p) noexcept;

    void*       address() const noexcept;

    bool        equals(std::nullptr_t) const noexcept;
    bool        equals(void const* p) const noexcept;
    bool        equals(offset_addressing_model const& other) const noexcept;

    bool        greater_than(std::nullptr_t) const noexcept;
    bool        greater_than(void const* p) const noexcept;
    bool        greater_than(offset_addressing_model const& other) const noexcept;

    bool        less_than(std::nullptr_t) const noexcept;
    bool        less_than(void const* p) const noexcept;
    bool        less_than(offset_addressing_model const& other) const noexcept;

    void        assign_from(void const* p);

    void        decrement(difference_type dec) noexcept;
    void        increment(difference_type inc) noexcept;

  private:
    using diff_type = difference_type ;

    enum : diff_type { null_offset = 1 };

    difference_type m_offset;

    static  diff_type   offset_between(void const *from, void const *to) noexcept;

    diff_type   offset_to(offset_addressing_model const &other) noexcept;
    diff_type   offset_to(void const *other) noexcept;
};

//------
//
inline offset_addressing_model::difference_type
offset_addressing_model::offset_between(void const *from, void const *to) noexcept
{
    return reinterpret_cast<intptr_t>(to) - reinterpret_cast<intptr_t>(from);
}

inline offset_addressing_model::difference_type
offset_addressing_model::offset_to(offset_addressing_model const &other) noexcept
{
    return (other.m_offset == null_offset) ? null_offset : (offset_between(this, &other) + other.m_offset);
}

inline offset_addressing_model::difference_type
offset_addressing_model::offset_to(void const *other) noexcept
{
    return (other == nullptr) ? null_offset : offset_between(this, other);
}

//------
//
inline
offset_addressing_model::offset_addressing_model() noexcept
:   m_offset{null_offset}
{}

inline
offset_addressing_model::offset_addressing_model(offset_addressing_model&& rhs) noexcept
:   m_offset{offset_to(rhs)}
{}

inline
offset_addressing_model::offset_addressing_model(offset_addressing_model const& rhs) noexcept
:   m_offset{offset_to(rhs)}
{}

inline
offset_addressing_model::offset_addressing_model(std::nullptr_t) noexcept
:   m_offset{null_offset}
{}

inline
offset_addressing_model::offset_addressing_model(void const* p) noexcept
:   m_offset{offset_to(p)}
{}

//------
//
inline offset_addressing_model&
offset_addressing_model::operator =(offset_addressing_model&& rhs) noexcept
{
    m_offset = offset_to(rhs);
    return *this;
}

inline offset_addressing_model&
offset_addressing_model::operator =(offset_addressing_model const& rhs) noexcept
{
    m_offset = offset_to(rhs);
    return *this;
}

inline offset_addressing_model&
offset_addressing_model::operator =(std::nullptr_t) noexcept
{
    m_offset = null_offset;
    return *this;
}

inline offset_addressing_model&
offset_addressing_model::operator =(void const* rhs) noexcept
{
    m_offset = offset_to(rhs);
    return *this;
}

//------
//
inline void*
offset_addressing_model::address() const noexcept
{
    return (m_offset == null_offset) ? nullptr : reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + m_offset);
}

//------
//
inline bool
offset_addressing_model::equals(std::nullptr_t) const noexcept
{
    return m_offset == null_offset;
}

inline bool
offset_addressing_model::equals(void const* p) const noexcept
{
    return address() == p;
}

inline bool
offset_addressing_model::equals(offset_addressing_model const& other) const noexcept
{
    return address() == other.address();
}

//------
//
inline bool
offset_addressing_model::greater_than(std::nullptr_t) const noexcept
{
    return address() != nullptr;
}

inline bool
offset_addressing_model::greater_than(void const* p) const noexcept
{
    return address() > p;
}

inline bool
offset_addressing_model::greater_than(offset_addressing_model const& other) const noexcept
{
    return address() > other.address();
}

//------
//
inline bool
offset_addressing_model::less_than(std::nullptr_t) const noexcept
{
    return false;
}

inline bool
offset_addressing_model::less_than(void const* p) const noexcept
{
    return address() < p;
}

inline bool
offset_addressing_model::less_than(offset_addressing_model const& other) const noexcept
{
    return address() < other.address();
}

//------
//
inline void
offset_addressing_model::assign_from(void const* p)
{
    m_offset = offset_to(p);
}

//------
//
inline void
offset_addressing_model::decrement(difference_type dec) noexcept
{
    m_offset -= dec;
}

inline void
offset_addressing_model::increment(difference_type inc) noexcept
{
    m_offset += inc;
}

#endif  //- 1D_ADDRESSING_H_DEFINED
