//==================================================================================================
//  File:
//      based_2dxl_addressing.h
//
//  Summary:
//      Defines a simple, two-dimensional based addressing model as a class template.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef BASED_2D_XL_ADDRESSING_H_DEFINED
#define BASED_2D_XL_ADDRESSING_H_DEFINED

#include <cstddef>
#include <cstdint>

//--------------------------------------------------------------------------------------------------
//  Class:
//      based_2d_xl_addressing_model
//
//  Summary:
//      This class template implements a based (segment:offset) addressing model in a 128-bit
//      package composed of two 64-bit unsigned integers.
//
//      Note that the comparison helper functions include several that define the "greater_than"
//      relationship.  They are included because they are trivial, and make the code for any
//      synthetic pointer wrapper class comparison operators easier to implement and read.
//--------------------------------------------------------------------------------------------------
//
template<typename SM>
class alignas(2*sizeof(std::uint64_t)) based_2d_xl_addressing_model
{
  public:
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

  public:
    ~based_2d_xl_addressing_model() = default;

    based_2d_xl_addressing_model() noexcept = default;
    based_2d_xl_addressing_model(based_2d_xl_addressing_model&&) noexcept = default;
    based_2d_xl_addressing_model(based_2d_xl_addressing_model const&) noexcept = default;
    based_2d_xl_addressing_model(std::nullptr_t) noexcept;
    based_2d_xl_addressing_model(size_type segment, size_type offset) noexcept;

    based_2d_xl_addressing_model&  operator =(based_2d_xl_addressing_model&&) noexcept = default;
    based_2d_xl_addressing_model&  operator =(based_2d_xl_addressing_model const&) noexcept = default;
    based_2d_xl_addressing_model&  operator =(std::nullptr_t) noexcept;

    void*       address() const noexcept;
    size_type   offset() const noexcept;
    size_type   segment() const noexcept;

    bool        equals(std::nullptr_t) const noexcept;
    bool        equals(void const* p) const noexcept;
    bool        equals(based_2d_xl_addressing_model const& other) const noexcept;

    bool        greater_than(std::nullptr_t) const noexcept;
    bool        greater_than(void const* p) const noexcept;
    bool        greater_than(based_2d_xl_addressing_model const& other) const noexcept;

    bool        less_than(std::nullptr_t) const noexcept;
    bool        less_than(void const* p) const noexcept;
    bool        less_than(based_2d_xl_addressing_model const& other) const noexcept;

    void        assign_from(void const* p);

    void        decrement(difference_type dec) noexcept;
    void        increment(difference_type inc) noexcept;

  private:
    uint64_t    m_offset;
    uint64_t    m_segment;
};

//--------------------------------------------------------------------------------------------------
//  Facility:   based_2d_xl_addressing_model<SM> implementation
//--------------------------------------------------------------------------------------------------
//
template<typename SM> inline
based_2d_xl_addressing_model<SM>::based_2d_xl_addressing_model(std::nullptr_t) noexcept
:   m_offset{0}
,   m_segment{0}
{}

template<typename SM> inline
based_2d_xl_addressing_model<SM>::based_2d_xl_addressing_model(size_type seg, size_type off) noexcept
:   m_offset{off}
,   m_segment{seg}
{}

template<typename SM> inline
based_2d_xl_addressing_model<SM>&
based_2d_xl_addressing_model<SM>::operator =(std::nullptr_t) noexcept
{
    m_offset = m_segment = 0u;
    return *this;
}

//------
//
template<typename SM> inline
void*
based_2d_xl_addressing_model<SM>::address() const noexcept
{
    return SM::segment_address(m_segment) + m_offset;
}

//------
//
template<typename SM> inline
typename based_2d_xl_addressing_model<SM>::size_type
based_2d_xl_addressing_model<SM>::offset() const noexcept
{
    return m_offset;
}

template<typename SM> inline
typename based_2d_xl_addressing_model<SM>::size_type
based_2d_xl_addressing_model<SM>::segment() const noexcept
{
    return m_segment;
}

//------
//
template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::equals(std::nullptr_t) const noexcept
{
    return address() == nullptr;
}

template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::equals(void const* p) const noexcept
{
    return address() == p;
}

template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::equals(based_2d_xl_addressing_model const& other) const noexcept
{
    return address() == other.address();
}

//------
//
template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::greater_than(std::nullptr_t) const noexcept
{
    return address() != nullptr;
}

template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::greater_than(void const* p) const noexcept
{
    return address() > p;
}

template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::greater_than(based_2d_xl_addressing_model const& other) const noexcept
{
    return address() > other.address();
}

//------
//
template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::less_than(std::nullptr_t) const noexcept
{
    return false;
}

template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::less_than(void const* p) const noexcept
{
    return address() < p;
}

template<typename SM> inline
bool
based_2d_xl_addressing_model<SM>::less_than(based_2d_xl_addressing_model const& other) const noexcept
{
    return address() < other.address();
}

//------
//
template<typename SM>
void
based_2d_xl_addressing_model<SM>::assign_from(void const* p)
{
    char const*     pdata = static_cast<char const*>(p);

    for (size_type segment_index = SM::first_segment_index();  segment_index <= SM::last_segment_index();  ++segment_index)
    {
        char const*     pbottom = SM::segment_address(segment_index);

        if (pbottom != nullptr)
        {
            char const*     ptop = pbottom + SM::segment_size(segment_index);

            if (pbottom <= pdata  &&  pdata < ptop)
            {
                m_offset  = pdata - pbottom;
                m_segment = segment_index;
                return;
            }
        }
    }
    m_segment = 0;
    m_offset  = pdata - static_cast<char const*>(nullptr);
}

//------
//
template<typename SM> inline
void
based_2d_xl_addressing_model<SM>::decrement(difference_type dec) noexcept
{
    m_offset -= dec;
}

template<typename SM> inline
void
based_2d_xl_addressing_model<SM>::increment(difference_type inc) noexcept
{
    m_offset += inc;
}

#endif  //- BASED_2D_XL_ADDRESSING_H_DEFINED
