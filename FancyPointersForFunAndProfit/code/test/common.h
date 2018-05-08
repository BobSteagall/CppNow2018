//==================================================================================================
//  File:
//      common.h
//
//  Summary:
//      Defines services that generate data for testing.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef COMMON_H_DEFINED
#define COMMON_H_DEFINED

//- Disable compilation noise from Windows before including any headers.
//
#ifdef _WIN32
#pragma warning (disable: 4100 4101 4189 4324 4390 4494 4503 4661 4996)
#endif

#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <scoped_allocator>
#include <string>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "stopwatch.h"
#include "based_2d_msk_storage.h"
#include "based_2d_sm_storage.h"
#include "based_2d_xl_storage.h"
#include "offset_storage.h"
#include "wrapper_storage.h"
#include "monotonic_allocation_strategy.h"
#include "rhx_allocator.h"

#undef max
#undef min

//- Compiler ID.
//
#if defined(__GNUG__) && !defined(__clang__)
    #define KEWB_COMPILER_GCC
#elif defined(__clang__)
    #define KEWB_COMPILER_CLANG
#elif defined(_WIN32)
    #define KEWB_COMPILER_MS
#endif

//- Function signature string.
//
#ifdef _WIN32
    #define KEWB_FN_SIG __FUNCSIG__
#else
    #define KEWB_FN_SIG __PRETTY_FUNCTION__
#endif

//- Very simple assertions.
//
#undef  CHECK
#define CHECK(EXPR)                                                                 \
     ((void)((EXPR) ? std::cout                                                     \
                    : std::cout << "FAILURE: in file: " << __FILE__                 \
                                << " at line " << __LINE__                          \
                                << "\n in function: " << KEWB_FN_SIG << std::endl))

//- Printing.
//
#define PRINT(C1, C2)   if (verbose_output()) print_sequences(C1, C2)


//using namespace std;        //- Yes, I know it's bad, but...

using wrapper_strategy      = monotonic_allocation_strategy<wrapper_storage_model>;
using based_2d_xl_strategy  = monotonic_allocation_strategy<based_2d_xl_storage_model>;
using based_2d_sm_strategy  = monotonic_allocation_strategy<based_2d_sm_storage_model>;
using based_2d_msk_strategy = monotonic_allocation_strategy<based_2d_msk_storage_model>;
using offset_strategy       = monotonic_allocation_strategy<offset_storage_model>;

bool    verbose_output();
size_t  max_ptr_op_count_index();


template<typename T, size_t N>
inline constexpr size_t array_size(T (&)[N]) { return N; }


//- Large struct that is intended to simulate an object with slow copy/move performance
//
struct test_struct
{
    uint64_t    m1;
    uint64_t    m2;
    char        m3[112];

    test_struct();

#define USE_OOL_TEST_STRUCT_MEMBERS
#ifdef USE_OOL_TEST_STRUCT_MEMBERS
    test_struct(test_struct const& other);
    test_struct&    operator =(test_struct const& rhs);
#else
    test_struct(test_struct const& other) = default;
    test_struct&    operator =(test_struct const& rhs) = default;
#endif
};

inline bool
operator ==(test_struct const& lhs, test_struct const& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(test_struct)) == 0;
}

inline bool
operator !=(test_struct const& lhs, test_struct const& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(test_struct)) != 0;
}

inline bool
operator <(test_struct const& lhs, test_struct const& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(test_struct)) < 0;
}

inline std::ostream&
operator <<(std::ostream& os, test_struct const& ts)
{
    os << "[" << ts.m1 << ",  " << ts.m2 << ",  '" << ts.m3 << "']";
    return os;
}

template<typename T, typename U>
inline std::ostream&
operator <<(std::ostream& os, std::pair<T,U> const& p)
{
    os << "{ " << p.first << ", " << p.second << " }";
    return os;
}


//- Simple struct to hold timing data for paired natural/synthetic comparison tests.
//
struct timing_pair
{
    int64_t     m_el_nat;
    int64_t     m_el_syn;

    int64_t     diff() const;
};

inline int64_t
timing_pair::diff() const
{
    return m_el_syn - m_el_nat;
}

inline bool
operator <(timing_pair const& lhs, timing_pair const& rhs)
{
    return lhs.diff() < rhs.diff();
}

using timing_vector = std::vector<timing_pair>;


//- Some facilities for generating random test data.
//
std::string     generate_test_string(size_t max_size);
test_struct     generate_test_struct();

template<typename T>
T   generate_data();

template<typename T>
std::vector<T>  generate_test_data(size_t num_elements);

template<typename IT>
void
destroy_range(IT iter, IT last)
{
    using value_type = typename std::iterator_traits<IT>::value_type;

    for (;  iter != last;  ++iter)
    {
        iter->~value_type();
    }
}

#endif  //- COMMON_H_DEFINED
