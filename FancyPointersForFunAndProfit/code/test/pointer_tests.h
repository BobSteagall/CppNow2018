//==================================================================================================
//  File:
//      pointer_tests.h
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef POINTER_TESTS_H_DEFINED
#define POINTER_TESTS_H_DEFINED

#include "common.h"

namespace
{
    size_t const    elem_counts[] = { 100u,    200u,    500u,
                                      1000u,   2000u,   5000u,
                                      10000u,  20000u,  50000u,
                                      100000u, 200000u, 500000u,
                                      1000000u };
}

inline size_t
max_element_index()
{
    return std::min(array_size(elem_counts), max_ptr_op_count_index());
}

#endif  //- POINTER_TESTS_H_DEFINED
