//==================================================================================================
//  File:
//      wrapper_addressing.h
//
//  Summary:
//      Defines the simplest synthetic addressing model, one that wraps a void pointer.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef WRAPPER_ADDRESSING_H_DEFINED
#define WRAPPER_ADDRESSING_H_DEFINED

#include <cstddef>
#include <cstdint>

//--------------------------------------------------------------------------------------------------
//  Class:
//      wrapper_addressing_model
//
//  Summary:
//      This class implements what is probably the simplest possible synthetic addressing model,
//      one that wraps a native void pointer.  
//
//      Note that the comparison helper functions include several that define the "greater_than"
//      relationship.  They are included because they are trivial, and make the code for any
//      synthetic pointer wrapper class comparison operators easier to implement and read.
//--------------------------------------------------------------------------------------------------
//
class alignas(sizeof(void*)) wrapper_addressing_model
{
  public:
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

  public:
    ~wrapper_addressing_model() = default;

    wrapper_addressing_model() noexcept = default;
    wrapper_addressing_model(wrapper_addressing_model&&) noexcept = default;
    wrapper_addressing_model(wrapper_addressing_model const&) noexcept = default;
    wrapper_addressing_model(std::nullptr_t) noexcept;
    wrapper_addressing_model(void*) noexcept;

    wrapper_addressing_model& operator =(wrapper_addressing_model&&) noexcept = default;
    wrapper_addressing_model& operator =(wrapper_addressing_model const&) noexcept = default;
    wrapper_addressing_model& operator =(std::nullptr_t) noexcept;

    void*   address() const noexcept;

    bool    equals(std::nullptr_t) const noexcept;
    bool    equals(void const* p) const noexcept;
    bool    equals(wrapper_addressing_model const& other) const noexcept;

    bool    greater_than(std::nullptr_t) const noexcept;
    bool    greater_than(void const* p) const noexcept;
    bool    greater_than(wrapper_addressing_model const& other) const noexcept;

    bool    less_than(std::nullptr_t) const noexcept;
    bool    less_than(void const* p) const noexcept;
    bool    less_than(wrapper_addressing_model const& other) const noexcept;

    void    assign_from(void* p) noexcept;
    void    assign_from(void const* p) noexcept;

    void    decrement(difference_type dec) noexcept;
    void    increment(difference_type inc) noexcept;

  private:
    union
    {
        void*       m_addr;
        void const* m_caddr;
    };
};

//------
//
inline
wrapper_addressing_model::wrapper_addressing_model(std::nullptr_t) noexcept
:   m_addr{nullptr}
{}

inline
wrapper_addressing_model::wrapper_addressing_model(void* p) noexcept
:   m_addr{p}
{}

inline wrapper_addressing_model&
wrapper_addressing_model::operator =(std::nullptr_t) noexcept
{
    m_addr = nullptr;
    return *this;
}

//------
//
inline void*
wrapper_addressing_model::address() const noexcept
{
    return m_addr;
}

//------
//
inline bool
wrapper_addressing_model::equals(std::nullptr_t) const noexcept
{
    return m_addr == nullptr;
}

inline bool
wrapper_addressing_model::equals(void const* p) const noexcept
{
    return m_addr == p;
}

inline bool
wrapper_addressing_model::equals(wrapper_addressing_model const& other) const noexcept
{
    return m_addr == other.m_addr;
}

//------
//
inline bool
wrapper_addressing_model::greater_than(std::nullptr_t) const noexcept
{
    return m_addr != nullptr;
}

inline bool
wrapper_addressing_model::greater_than(void const* p) const noexcept
{
    return m_addr > p;
}

inline bool
wrapper_addressing_model::greater_than(wrapper_addressing_model const& other) const noexcept
{
    return m_addr > other.m_addr;
}

//------
//
inline bool
wrapper_addressing_model::less_than(std::nullptr_t) const noexcept
{
    return false;
}

inline bool
wrapper_addressing_model::less_than(void const* p) const noexcept
{
    return m_addr < p;
}

inline bool
wrapper_addressing_model::less_than(wrapper_addressing_model const& other) const noexcept
{
    return m_addr < other.m_addr;
}

//------
//
inline void
wrapper_addressing_model::assign_from(void* p) noexcept
{
    m_addr = p;
}

inline void
wrapper_addressing_model::assign_from(void const* p) noexcept
{
    m_caddr = p;
}

//------
//
inline void
wrapper_addressing_model::decrement(difference_type dec) noexcept
{
    m_addr = static_cast<char*>(m_addr) - dec;
}

inline void
wrapper_addressing_model::increment(difference_type inc) noexcept
{
    m_addr = static_cast<char*>(m_addr) + inc;
}

#endif  //- WRAPPER_ADDRESSING_H_DEFINED
