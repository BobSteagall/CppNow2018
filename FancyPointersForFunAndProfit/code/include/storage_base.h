//==================================================================================================
//  File:
//      storage_base.h
//
//  Summary:
//      Defines a generalized storage model base class that can be reused by derived types to 
//      implement specific storage models (wrapper, based 2D, offset) for testing.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef STORAGE_MODEL_BASE_H_DEFINED
#define STORAGE_MODEL_BASE_H_DEFINED

#include <cstddef>
#include <cstdint>

//--------------------------------------------------------------------------------------------------
//  Class:
//      storage_model_base
//
//  Summary:
//      This base class implements a simple, segmented storage model where segments are allocated
//      from the process's own heap (i.e., its private address space).  It is intended to be used
//      by derived classes to implement a small number of alternative storage models - wrapper,
//      based 2D, and offset.
//--------------------------------------------------------------------------------------------------
//
class storage_model_base
{
  public:
    using difference_type = std::ptrdiff_t;
    using size_type       = std::size_t; 

    enum : size_type
    {
        max_segments = 3,           //- Don't need many for testing
        max_size     = 1u << 27     //- 128 MB segments
    };

  public:
    static  void        allocate_segment(size_type segment, size_type size = max_size);
    static  void        clear_segments();
    static  void        deallocate_segment(size_type segment);
    static  void        init_segments();
    static  void        reset_segments();
    static  void        swap_segments();

    static  char*       segment_address(size_type segment) noexcept;
    static  size_type   segment_size(size_type segment) noexcept;

    static  constexpr   size_type   first_segment_index();
    static  constexpr   size_type   last_segment_index();
    static  constexpr   size_type   max_segment_count();
    static  constexpr   size_type   max_segment_size();

  protected:
    static  char*       sm_segment_ptrs[max_segments + 2];
    static  size_type   sm_segment_size[max_segments + 2];
    static  char*       sm_shadow_ptrs[max_segments + 2];
    static  bool        sm_ready;
};

//------
//
inline char*
storage_model_base::segment_address(size_type segment) noexcept
{
    return sm_segment_ptrs[segment];
}

inline storage_model_base::size_type
storage_model_base::segment_size(size_type segment) noexcept
{
    return sm_segment_size[segment];
}

//------
//
constexpr inline storage_model_base::size_type
storage_model_base::first_segment_index()
{
    return 2;
}

constexpr inline storage_model_base::size_type
storage_model_base::last_segment_index()
{
    return max_segments + 1;
}

constexpr inline storage_model_base::size_type
storage_model_base::max_segment_count()
{
    return max_segments;
}

constexpr inline storage_model_base::size_type
storage_model_base::max_segment_size()
{
    return max_size;
}

#endif  //- STORAGE_MODEL_BASE_H_DEFINED
