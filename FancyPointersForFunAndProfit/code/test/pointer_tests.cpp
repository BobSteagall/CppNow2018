//==================================================================================================
//  File:   pointer_tests.cpp
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include "pointer_tests.h"
#include "pointer_cast_tests.h"
#include "pointer_copy_tests.h"
#include "pointer_sort_tests.h"

#define RUN_COPY_TESTS(ST, DT)          run_pointer_copy_tests<ST,DT>(#ST, #DT)
#define RUN_SORT_TESTS(ST, DT)          run_pointer_sort_tests<ST,DT>(#ST, #DT)

using name_list   = std::vector<std::string>;
using counts_list = std::vector<size_t>;
using ratio_list  = std::vector<double>;
using ratios_list = std::vector<std::vector<double>>;

void    
print_tabular_summary
(name_list const& test_names, counts_list const& test_sizes, ratios_list const& test_ratios)
{
    printf("strategy\\elements");
    for (size_t elem_count : test_sizes)
    {
        printf(", %u", (uint32_t) elem_count);
    }
    printf("\n");

    for (size_t i = 0;  i < test_names.size();  ++i)
    {
        ratio_list const&   ratios = test_ratios[i];

        printf("%s", test_names[i].c_str());

        for (double ratio : ratios)
        {
            printf(", %7.5f", ratio);
        }
        printf("\n");
    }
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    fflush(stdout);
}


void
run_pointer_tests(bool do_copy_tests, bool do_sort_tests)
{
    std::string     name;
    counts_list     counts;
    ratio_list      ratios;

    name_list       test_names;
    ratios_list     test_ratios;

    printf("\n");
    run_pointer_cast_tests<wrapper_strategy>();
    run_pointer_cast_tests<based_2d_xl_strategy>();
    run_pointer_cast_tests<based_2d_msk_strategy>();
    run_pointer_cast_tests<offset_strategy>();

    if (do_copy_tests)
    {
        //- for uint64_t
        //
        test_names.clear();
        test_ratios.clear();

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(wrapper_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(based_2d_xl_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(based_2d_sm_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(based_2d_msk_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(offset_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        printf("tabular summary for copy() with uint64_t:\n");
        print_tabular_summary(test_names, counts, test_ratios);

        //- for test_struct
        //
        test_names.clear();
        test_ratios.clear();

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(wrapper_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(based_2d_xl_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(based_2d_sm_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(based_2d_msk_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_COPY_TESTS(offset_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        printf("tabular summary for copy() with test_struct:\n");
        print_tabular_summary(test_names, counts, test_ratios);
    }

    if (do_sort_tests)
    {
        //- for uint64_t
        //
        test_names.clear();
        test_ratios.clear();

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(wrapper_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(based_2d_xl_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(based_2d_sm_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(based_2d_msk_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(offset_strategy, uint64_t);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        printf("tabular summary for sort() with uint64_t:\n");
        print_tabular_summary(test_names, counts, test_ratios);

        //- for test_struct
        //
        test_names.clear();
        test_ratios.clear();

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(wrapper_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(based_2d_xl_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(based_2d_sm_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(based_2d_msk_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        std::tie(name, counts, ratios) = RUN_SORT_TESTS(offset_strategy, test_struct);
        test_names.push_back(std::move(name));
        test_ratios.push_back(std::move(ratios));

        printf("tabular summary for sort() with test_struct:\n");
        print_tabular_summary(test_names, counts, test_ratios);
    }
}
