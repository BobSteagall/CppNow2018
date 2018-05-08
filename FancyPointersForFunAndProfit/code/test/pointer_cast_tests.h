//==================================================================================================
//  File:   pointer_cast_tests.h
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef POINTER_CAST_TESTS_H_DEFINED
#define POINTER_CAST_TESTS_H_DEFINED

#include "pointer_tests.h"

//- A handful of simple types, functions, and macros to assist with type pointer cast tests
//  below.  These types help in the upcasting and downcasting tests.
//
struct base
{
    int m_base = 0;
};

struct user : public base
{
    int m_type = 1;
};

struct derived : public user
{
    int m_drvd = 2;
};

struct unrelated
{
    int m_unr = 3;
};

//- This unimplemented function template is used in the macros below to aid in the process
//  of determining convertibility.
//
template<class T>   T   make_val();

#define exp_conv(SRC, DST)                                                                  \
    static_assert((!std::is_convertible<SRC, DST>::value  &&                                \
                    std::is_same<DST, decltype(static_cast<DST>(make_val<SRC>()))>::value), \
                    "error: missing explicit conversion of " #SRC " to " #DST)

#define imp_conv(SRC, DST)                                                                  \
    static_assert(std::is_convertible<SRC, DST>::value,                                     \
                  "error: missing implicit conversion of " #SRC " to " #DST)

#define not_conv(SRC, DST)                                                                  \
    static_assert(!std::is_convertible<SRC, DST>::value,                                    \
                  "error: found unexpected implicit conversion of " #SRC " to " #DST)

template<typename AS, typename T>
using test_ptr = typename AS::template rebind_pointer<T>;

//--------------------------------------------------------------------------------------------------
//  Function:
//      run_pointer_cast_tests<AS>
//
//  Summary:
//      This function template verifies conversion operations between natural and synthetic
//      pointers.
//--------------------------------------------------------------------------------------------------
//
template<typename AS>
void
run_pointer_cast_tests()
{
    static volatile int x = 0;

    using test_ptr_void             = test_ptr<AS, void>;
    using test_ptr_void_const       = test_ptr<AS, void const>;
    using test_ptr_base             = test_ptr<AS, base>;
    using test_ptr_base_const       = test_ptr<AS, base const>;
    using test_ptr_type             = test_ptr<AS, user>;
    using test_ptr_type_const       = test_ptr<AS, user const>;
    using test_ptr_derived          = test_ptr<AS, derived>;
    using test_ptr_derived_const    = test_ptr<AS, derived const>;
    using test_ptr_unrelated        = test_ptr<AS, unrelated>;
    using test_ptr_unrelated_const  = test_ptr<AS, unrelated const>;

    imp_conv(user*, void*);
    exp_conv(void*, user*);

    imp_conv(void*, test_ptr_void);
    imp_conv(void*, test_ptr_void_const);
    not_conv(void*, test_ptr_base);
    not_conv(void*, test_ptr_base_const);
    not_conv(void*, test_ptr_type);
    not_conv(void*, test_ptr_type_const);
    not_conv(void*, test_ptr_derived);
    not_conv(void*, test_ptr_derived_const);

    not_conv(void const*, test_ptr_void);
    imp_conv(void const*, test_ptr_void_const);
    not_conv(void const*, test_ptr_base);
    not_conv(void const*, test_ptr_base_const);
    not_conv(void const*, test_ptr_type);
    not_conv(void const*, test_ptr_type_const);
    not_conv(void const*, test_ptr_derived);
    not_conv(void const*, test_ptr_derived_const);

    imp_conv(base*, test_ptr_void);
    imp_conv(base*, test_ptr_void_const);
    imp_conv(base*, test_ptr_base);
    imp_conv(base*, test_ptr_base_const);
    not_conv(base*, test_ptr_type);
    not_conv(base*, test_ptr_type_const);
    not_conv(base*, test_ptr_derived);
    not_conv(base*, test_ptr_derived_const);

    not_conv(base const*, test_ptr_void);
    imp_conv(base const*, test_ptr_void_const);
    not_conv(base const*, test_ptr_base);
    imp_conv(base const*, test_ptr_base_const);
    not_conv(base const*, test_ptr_type);
    not_conv(base const*, test_ptr_type_const);
    not_conv(base const*, test_ptr_derived);
    not_conv(base const*, test_ptr_derived_const);

    imp_conv(user*, test_ptr_void);
    imp_conv(user*, test_ptr_void_const);
    imp_conv(user*, test_ptr_base);
    imp_conv(user*, test_ptr_base_const);
    imp_conv(user*, test_ptr_type);
    imp_conv(user*, test_ptr_type_const);
    not_conv(user*, test_ptr_derived);
    not_conv(user*, test_ptr_derived_const);
    not_conv(user*, test_ptr_unrelated);
    not_conv(user*, test_ptr_unrelated_const);

    not_conv(user const*, test_ptr_void);
    imp_conv(user const*, test_ptr_void_const);
    not_conv(user const*, test_ptr_base);
    imp_conv(user const*, test_ptr_base_const);
    not_conv(user const*, test_ptr_type);
    imp_conv(user const*, test_ptr_type_const);
    not_conv(user const*, test_ptr_derived);
    not_conv(user const*, test_ptr_derived_const);
    not_conv(user const*, test_ptr_unrelated);
    not_conv(user const*, test_ptr_unrelated_const);

    imp_conv(derived*, test_ptr_void);
    imp_conv(derived*, test_ptr_void_const);
    imp_conv(derived*, test_ptr_base);
    imp_conv(derived*, test_ptr_base_const);
    imp_conv(derived*, test_ptr_type);
    imp_conv(derived*, test_ptr_type_const);
    imp_conv(derived*, test_ptr_derived);
    imp_conv(derived*, test_ptr_derived_const);

    not_conv(derived const*, test_ptr_void);
    imp_conv(derived const*, test_ptr_void_const);
    not_conv(derived const*, test_ptr_base);
    imp_conv(derived const*, test_ptr_base_const);
    not_conv(derived const*, test_ptr_type);
    imp_conv(derived const*, test_ptr_type_const);
    not_conv(derived const*, test_ptr_derived);
    imp_conv(derived const*, test_ptr_derived_const);

    imp_conv(test_ptr_void, void*);
    imp_conv(test_ptr_void, void const*);
    exp_conv(test_ptr_void, base*);
    exp_conv(test_ptr_void, base const*);
    exp_conv(test_ptr_void, user*);
    exp_conv(test_ptr_void, user const*);
    exp_conv(test_ptr_void, derived*);
    exp_conv(test_ptr_void, derived const*);
    imp_conv(test_ptr_void, test_ptr_void);
    imp_conv(test_ptr_void, test_ptr_void_const);
    not_conv(test_ptr_void, test_ptr_base);
    not_conv(test_ptr_void, test_ptr_base_const);
    not_conv(test_ptr_void, test_ptr_type);
    not_conv(test_ptr_void, test_ptr_type_const);
    not_conv(test_ptr_void, test_ptr_derived);
    not_conv(test_ptr_void, test_ptr_derived_const);

    not_conv(test_ptr_void_const, void*);
    imp_conv(test_ptr_void_const, void const*);
    not_conv(test_ptr_void_const, base*);
    exp_conv(test_ptr_void_const, base const*);
    not_conv(test_ptr_void_const, user*);
    exp_conv(test_ptr_void_const, user const*);
    not_conv(test_ptr_void_const, derived*);
    exp_conv(test_ptr_void_const, derived const*);
    not_conv(test_ptr_void_const, test_ptr_void);
    imp_conv(test_ptr_void_const, test_ptr_void_const);
    not_conv(test_ptr_void_const, test_ptr_base);
    not_conv(test_ptr_void_const, test_ptr_base_const);
    not_conv(test_ptr_void_const, test_ptr_type);
    not_conv(test_ptr_void_const, test_ptr_type_const);
    not_conv(test_ptr_void_const, test_ptr_derived);
    not_conv(test_ptr_void_const, test_ptr_derived_const);

    imp_conv(test_ptr_base, void*);
    imp_conv(test_ptr_base, void const*);
    imp_conv(test_ptr_base, base*);
    imp_conv(test_ptr_base, base const*);
    exp_conv(test_ptr_base, user*);
    exp_conv(test_ptr_base, user const*);
    exp_conv(test_ptr_base, derived*);
    exp_conv(test_ptr_base, derived const*);
    imp_conv(test_ptr_base, test_ptr_void);
    imp_conv(test_ptr_base, test_ptr_void_const);
    imp_conv(test_ptr_base, test_ptr_base);
    imp_conv(test_ptr_base, test_ptr_base_const);
    not_conv(test_ptr_base, test_ptr_type);
    not_conv(test_ptr_base, test_ptr_type_const);
    not_conv(test_ptr_base, test_ptr_derived);
    not_conv(test_ptr_base, test_ptr_derived_const);

    not_conv(test_ptr_base_const, void*);
    imp_conv(test_ptr_base_const, void const*);
    not_conv(test_ptr_base_const, base*);
    imp_conv(test_ptr_base_const, base const*);
    not_conv(test_ptr_base_const, user*);
    exp_conv(test_ptr_base_const, user const*);
    not_conv(test_ptr_base_const, derived*);
    exp_conv(test_ptr_base_const, derived const*);
    not_conv(test_ptr_base_const, test_ptr_void);
    imp_conv(test_ptr_base_const, test_ptr_void_const);
    not_conv(test_ptr_base_const, test_ptr_base);
    imp_conv(test_ptr_base_const, test_ptr_base_const);
    not_conv(test_ptr_base_const, test_ptr_type);
    not_conv(test_ptr_base_const, test_ptr_type_const);
    not_conv(test_ptr_base_const, test_ptr_derived);
    not_conv(test_ptr_base_const, test_ptr_derived_const);

    imp_conv(test_ptr_type, void*);
    imp_conv(test_ptr_type, void const*);
    imp_conv(test_ptr_type, base*);
    imp_conv(test_ptr_type, base const*);
    imp_conv(test_ptr_type, user*);
    imp_conv(test_ptr_type, user const*);
    exp_conv(test_ptr_type, derived*);
    exp_conv(test_ptr_type, derived const*);
    imp_conv(test_ptr_type, test_ptr_void);
    imp_conv(test_ptr_type, test_ptr_void_const);
    imp_conv(test_ptr_type, test_ptr_base);
    imp_conv(test_ptr_type, test_ptr_base_const);
    imp_conv(test_ptr_type, test_ptr_type);
    imp_conv(test_ptr_type, test_ptr_type_const);
    not_conv(test_ptr_type, test_ptr_derived);
    not_conv(test_ptr_type, test_ptr_derived_const);

    not_conv(test_ptr_type_const, void*);
    imp_conv(test_ptr_type_const, void const*);
    not_conv(test_ptr_type_const, base*);
    imp_conv(test_ptr_type_const, base const*);
    not_conv(test_ptr_type_const, user*);
    imp_conv(test_ptr_type_const, user const*);
    not_conv(test_ptr_type_const, derived*);
    exp_conv(test_ptr_type_const, derived const*);
    not_conv(test_ptr_type_const, test_ptr_void);
    imp_conv(test_ptr_type_const, test_ptr_void_const);
    not_conv(test_ptr_type_const, test_ptr_base);
    imp_conv(test_ptr_type_const, test_ptr_base_const);
    not_conv(test_ptr_type_const, test_ptr_type);
    imp_conv(test_ptr_type_const, test_ptr_type_const);
    not_conv(test_ptr_type_const, test_ptr_derived);
    not_conv(test_ptr_type_const, test_ptr_derived_const);

    imp_conv(test_ptr_derived, void*);
    imp_conv(test_ptr_derived, void const*);
    imp_conv(test_ptr_derived, base*);
    imp_conv(test_ptr_derived, base const*);
    imp_conv(test_ptr_derived, user*);
    imp_conv(test_ptr_derived, user const*);
    imp_conv(test_ptr_derived, derived*);
    imp_conv(test_ptr_derived, derived const*);
    imp_conv(test_ptr_derived, test_ptr_void);
    imp_conv(test_ptr_derived, test_ptr_void_const);
    imp_conv(test_ptr_derived, test_ptr_base);
    imp_conv(test_ptr_derived, test_ptr_base_const);
    imp_conv(test_ptr_derived, test_ptr_type);
    imp_conv(test_ptr_derived, test_ptr_type_const);
    imp_conv(test_ptr_derived, test_ptr_derived);
    imp_conv(test_ptr_derived, test_ptr_derived_const);

    not_conv(test_ptr_derived_const, void*);
    imp_conv(test_ptr_derived_const, void const*);
    not_conv(test_ptr_derived_const, base*);
    imp_conv(test_ptr_derived_const, base const*);
    not_conv(test_ptr_derived_const, user*);
    imp_conv(test_ptr_derived_const, user const*);
    not_conv(test_ptr_derived_const, derived*);
    imp_conv(test_ptr_derived_const, derived const*);
    not_conv(test_ptr_derived_const, test_ptr_void);
    imp_conv(test_ptr_derived_const, test_ptr_void_const);
    not_conv(test_ptr_derived_const, test_ptr_base);
    imp_conv(test_ptr_derived_const, test_ptr_base_const);
    not_conv(test_ptr_derived_const, test_ptr_type);
    imp_conv(test_ptr_derived_const, test_ptr_type_const);
    not_conv(test_ptr_derived_const, test_ptr_derived);
    imp_conv(test_ptr_derived_const, test_ptr_derived_const);

    ++x;
}

#endif  //- POINTER_CAST_TESTS_H_DEFINED
