//==================================================================================================
//  File:
//      monotonic_allocation_strategy.cpp
//
//  Summary:
//      Instantiates a monotonic allocation strategy object for various storage models.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include "based_2d_msk_storage.h"
#include "based_2d_sm_storage.h"
#include "based_2d_xl_storage.h"
#include "offset_storage.h"
#include "wrapper_storage.h"
#include "monotonic_allocation_strategy.h"

#if 1
template class monotonic_allocation_strategy<based_2d_msk_storage_model>;
template class monotonic_allocation_strategy<based_2d_sm_storage_model>;
template class monotonic_allocation_strategy<based_2d_xl_storage_model>;
template class monotonic_allocation_strategy<offset_storage_model>;
template class monotonic_allocation_strategy<wrapper_storage_model>;
#endif
