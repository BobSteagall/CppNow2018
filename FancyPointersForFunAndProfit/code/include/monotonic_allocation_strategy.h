//==================================================================================================
//  File:
//      monotonic_allocation_strategy.h
//
//  Summary:
//      Defines a very leaky allocation strategy class for testing rhx_allocator.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef SEGMENTED_LEAKY_ALLOCATION_STRATEGY_H_DEFINED
#define SEGMENTED_LEAKY_ALLOCATION_STRATEGY_H_DEFINED

#include <cstddef>
#include <cstdint>

#include "synthetic_pointer.h"

//--------------------------------------------------------------------------------------------------
//  Class:
//      monotonic_allocation_strategy<SM>
//
//  Summary:
//      This class implements a simple leaky allocation strategy for testing purposes.  It relies
//      on the interface provided by storage_model_base and its derived types.
//--------------------------------------------------------------------------------------------------
//
template<class SM>
class monotonic_allocation_strategy
{
  public:
    using storage_model         = SM;
    using addressing_model      = typename SM::addressing_model;
    using difference_type       = typename SM::difference_type;
    using size_type             = typename SM::size_type;
    using void_pointer          = syn_ptr<void, addressing_model>;
    using const_void_pointer    = syn_ptr<void const, addressing_model>;

    template<class T>
    using rebind_pointer        = syn_ptr<T, addressing_model>;

  public:
    size_type       max_size() const;

    void_pointer    allocate(size_type n);
    void            deallocate(void_pointer p);

    static  void    reset_segments();
    static  void    swap_segments();

  private:
    static  difference_type     round_up(difference_type x, difference_type r);

    static  size_type   sm_curr_segment;
    static  size_type   sm_curr_offset;
    static  bool        sm_initialized;
};

//------
//
template<class SM>  typename monotonic_allocation_strategy<SM>::size_type
    monotonic_allocation_strategy<SM>::sm_curr_segment = 0;

template<class SM>  typename monotonic_allocation_strategy<SM>::size_type
monotonic_allocation_strategy<SM>::sm_curr_offset = 0;

template<class SM>  bool
monotonic_allocation_strategy<SM>::sm_initialized = false;

//------
//
template<class SM> inline
typename monotonic_allocation_strategy<SM>::size_type
monotonic_allocation_strategy<SM>::max_size() const
{
    return storage_model::max_segment_size();
}

//------
//
template<class SM>
typename monotonic_allocation_strategy<SM>::void_pointer
monotonic_allocation_strategy<SM>::allocate(size_type n)
{
    if (!sm_initialized)
    {
        storage_model::init_segments();
        sm_curr_segment = storage_model::first_segment_index();
        sm_curr_offset  = 64;
        sm_initialized  = true;
    }

    size_type   chunk_size   = round_up(n, 16u);
    size_type   chunk_offset = sm_curr_offset;

    if ((chunk_offset + chunk_size) > storage_model::max_segment_size())
    {
        ++sm_curr_segment;
        chunk_offset   = 64;
        sm_curr_offset = chunk_offset + chunk_size;
    }
    else
    {
        sm_curr_offset += chunk_size;
    }

    return storage_model::segment_pointer(sm_curr_segment, chunk_offset);
}

template<class SM> inline
void
monotonic_allocation_strategy<SM>::deallocate(void_pointer)
{}

//------
//
template<class SM> inline
void
monotonic_allocation_strategy<SM>::reset_segments()
{
    storage_model::reset_segments();
    sm_curr_segment = storage_model::first_segment_index();
    sm_curr_offset  = 64;
}

template<class SM> inline
void
monotonic_allocation_strategy<SM>::swap_segments()
{
    storage_model::swap_segments();
}

//------
//
template<class SM> inline
typename monotonic_allocation_strategy<SM>::difference_type
monotonic_allocation_strategy<SM>::round_up(difference_type x, difference_type r)
{
    return (x % r) ? (x + r - (x % r)) : x;
}

#endif  //- SEGMENTED_LEAKY_ALLOCATION_STRATEGY_H_DEFINED
