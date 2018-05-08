//==================================================================================================
//  File:
//      storage_base.h
//
//  Summary:
//      Defines a generalized storage model base class that can be reused by derived types to 
//      implement specific storage models for testing.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include <cstring>
#include <utility>
#include "storage_base.h"

using size_type = storage_model_base::size_type;

char*       storage_model_base::sm_segment_ptrs[max_segments + 2];
size_type   storage_model_base::sm_segment_size[max_segments + 2];
char*       storage_model_base::sm_shadow_ptrs[max_segments + 2];
bool        storage_model_base::sm_ready = false;

void
storage_model_base::allocate_segment(size_type segment, size_type size)
{
    if (segment >= first_segment_index()  &&  segment <= last_segment_index()  &&  
        size <= max_size  &&  sm_segment_ptrs[segment] == nullptr)
    {
        sm_shadow_ptrs[segment] = new char[size];
        memset(sm_shadow_ptrs[segment], 0, size);

        sm_segment_ptrs[segment] = new char[size];
        memset(sm_segment_ptrs[segment], 0, size);

        sm_segment_size[segment] = size;
    }
}

void
storage_model_base::clear_segments()
{
    sm_ready = false;

    for (size_type i = first_segment_index();  i <= last_segment_index();  ++i)
    {
        deallocate_segment(i);
    }
}

void
storage_model_base::deallocate_segment(size_type segment)
{
    if (sm_segment_ptrs[segment] != nullptr)
    {
        delete [] sm_segment_ptrs[segment];
        delete [] sm_shadow_ptrs[segment];
        sm_segment_ptrs[segment] = nullptr;
        sm_segment_size[segment] = 0;
        sm_shadow_ptrs[segment]  = nullptr;
    }
}

void
storage_model_base::init_segments()
{
    if (!sm_ready)
    {
        for (size_type i = first_segment_index();  i <= last_segment_index();  ++i)
        {
            allocate_segment(i);
        }

        sm_ready = true;
    }
}

void
storage_model_base::reset_segments()
{
    for (size_type i = first_segment_index();  i <= last_segment_index();  ++i)
    {
        if (sm_segment_ptrs[i] != nullptr)
        {
            memset(sm_segment_ptrs[i], 0, sm_segment_size[i]);
            memset(sm_shadow_ptrs[i], 0, sm_segment_size[i]);
        }
    }
}

void
storage_model_base::swap_segments()
{
    for (size_type i = first_segment_index();  i <= last_segment_index();  ++i)
    {
        memcpy(sm_shadow_ptrs[i], sm_segment_ptrs[i], sm_segment_size[i]);
        std::swap(sm_shadow_ptrs[i], sm_segment_ptrs[i]);
    }
}
