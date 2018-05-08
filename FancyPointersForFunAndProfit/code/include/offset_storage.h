//==================================================================================================
//  File:
//      offset_storage.h
//
//  Summary:
//      Defines a simple storage model that uses the wrapper addressing model.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef OFFSET_STORAGE_H_DEFINED
#define OFFSET_STORAGE_H_DEFINED

#include "storage_base.h"
#include "offset_addressing.h"

//--------------------------------------------------------------------------------------------------
//  Class:
//      offset_storage_model
//
//  Summary:
//      This base class implements a based 1D storage model using the facilities provided by
//      the "storage_model_base" base class.  For this model, all allocations occur from the
//      first segment, making it appear as if there is only one segment.
//--------------------------------------------------------------------------------------------------
//
class offset_storage_model : public storage_model_base
{
  public:
    using addressing_model = offset_addressing_model;

    static  addressing_model    segment_pointer(size_type, size_type offset);
};

//------
//
inline offset_storage_model::addressing_model
offset_storage_model::segment_pointer(size_type segment, size_type offset)
{
    return addressing_model{segment_address(segment) + offset};
}

#endif  //- OFFSET_STORAGE_H_DEFINED
