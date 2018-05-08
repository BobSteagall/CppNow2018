//==================================================================================================
//  File:
//      based_2d_sm_storage.h
//
//  Summary:
//      Defines a simple two-dimensional based addressing model.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef BASED_2D_SM_STORAGE_H_DEFINED
#define BASED_2D_SM_STORAGE_H_DEFINED

#include "storage_base.h"
#include "based_2d_sm_addressing.h"

//--------------------------------------------------------------------------------------------------
//  Class:
//      based_2d_sm_storage_model
//
//  Summary:
//      This base class implements a based 2D storage model using the facilities provided by
//      the "storage_model_base" base class.
//--------------------------------------------------------------------------------------------------
//
class based_2d_sm_storage_model : public storage_model_base
{
  public:
    using addressing_model = based_2d_sm_addressing_model<based_2d_sm_storage_model>;

    static  addressing_model    segment_pointer(size_type segment, size_type offset=0);
};

//------
//
inline based_2d_sm_storage_model::addressing_model
based_2d_sm_storage_model::segment_pointer(size_type segment, size_type offset)
{
    return addressing_model{(addressing_model::size_type) segment, (addressing_model::size_type) offset};
}

#endif  //- BASED_2D_SM_STORAGE_H_DEFINED
