//==================================================================================================
//  File:
//      pointer_sort_tests.h
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef POINTER_SORT_TESTS_H_DEFINED
#define POINTER_SORT_TESTS_H_DEFINED

#include "pointer_tests.h"

//--------------------------------------------------------------------------------------------------
//  Function:
//      do_pointer_sort_test<AS,DT>
//
//  Summary:
//      This function template measures the time it takes to sort elements in an array.  It
//      measures elapsed time twice: once for the case when the destination is accessed with
//      native pointers, and once for the case when the destination is accessed with synthetic
//      pointers.
//--------------------------------------------------------------------------------------------------
//
template<typename AllocStrategy, typename DataType>
timing_pair
do_pointer_sort_test(size_t nelem)
{
    using syn_ptr_void = typename AllocStrategy::void_pointer;
    using syn_ptr_data = typename AllocStrategy::template rebind_pointer<DataType>;

    //- Instantiate a std::vector and fill it with some randomly-generated test data.  Instantiate
    //  a second vector that will contain the sorted version of the first.
    //
    std::vector<DataType>   random_data(generate_test_data<DataType>(nelem));   //- Random source data
    std::vector<DataType>   sorted_data(random_data);                           //- Sorted reference data

    std::sort(std::begin(sorted_data), std::end(sorted_data));

    //- Instantiate the allocation strategy object and allocate a buffer of appropriate size using
    //  the pointer types expressed in the strategy's public interface.
    //
    AllocStrategy   heap;
    syn_ptr_void    psyn_void  = heap.allocate(nelem*sizeof(DataType));
    syn_ptr_data    psyn_begin = static_cast<syn_ptr_data >(psyn_void);
    syn_ptr_data    psyn_end   = psyn_begin + nelem;

    //- Initialize the raw storage with dummy values so we can just use copy() below.
    //
    std::uninitialized_fill(psyn_begin, psyn_end, DataType());

    //- Get a couple of corresponding natural pointers for comparison purposes.
    //
    DataType*   pnat_begin = psyn_begin;
    DataType*   pnat_end   = psyn_end;

    //- Get ready to time the sorts.
    //
    static bool native_first = true;
    stopwatch   sw;
    int64_t     el_nat = 0;
    int64_t     el_syn = 0;

    //- We're going to alternate between native operations first and synthetic operations first
    //  in order to avoid any bias in the average due to the order.
    //
    if (native_first)
    {
        //- Copy the random data into the buffer using native pointers and verify its integrity.
        //
        std::copy(random_data.cbegin(), random_data.cend(), pnat_begin);
        auto    mm1_copy_nat = std::mismatch(pnat_begin, pnat_end, random_data.cbegin(), random_data.cend());
        CHECK(mm1_copy_nat.first == pnat_end);

        //- Sort the buffer using native pointers as iterators.
        //
        sw.start();
        std::sort(pnat_begin, pnat_end);
        sw.stop();
        el_nat = sw.elapsed_nsec();

        //- Verify that the newly-sorted buffer matches the sorted reference vector.
        //
        auto    mm1_sort_nat = std::mismatch(pnat_begin, pnat_end, sorted_data.cbegin(), sorted_data.cend());
        CHECK(mm1_sort_nat.first == pnat_end);

        //- Copy the random data into the buffer using synthetic pointers and verify its integrity.
        //
        std::copy(random_data.cbegin(), random_data.cend(), psyn_begin);

        auto    mm2_copy_syn = std::mismatch(psyn_begin, psyn_end, random_data.cbegin(), random_data.cend());
        CHECK(mm2_copy_syn.first == psyn_end);
        auto    mm2_copy_nat = std::mismatch(pnat_begin, pnat_end, random_data.cbegin(), random_data.cend());
        CHECK(mm2_copy_nat.first == pnat_end);

        //- Sort the buffer using synthetic pointers as iterators.
        //
        sw.start();
        std::sort(psyn_begin, psyn_end);
        sw.stop();
        el_syn = sw.elapsed_nsec();

        //- Verify that the newly-sorted buffer matches the sorted reference vector.
        //
        auto    mm2_sort_syn = std::mismatch(psyn_begin, psyn_end, sorted_data.cbegin(), sorted_data.cend());
        CHECK(mm2_sort_syn.first == psyn_end);
        auto    mm2_sort_nat = std::mismatch(pnat_begin, pnat_end, sorted_data.cbegin(), sorted_data.cend());
        CHECK(mm2_sort_nat.first == pnat_end);
    }
    else    //- synthetic first.
    {
        //- Copy the random data into the buffer using synthetic pointers and verify its integrity.
        //
        std::copy(cbegin(random_data), random_data.cend(), psyn_begin);

        auto    mm2_copy_syn = std::mismatch(psyn_begin, psyn_end, random_data.cbegin(), random_data.cend());
        CHECK(mm2_copy_syn.first == psyn_end);
        auto    mm2_copy_nat = std::mismatch(pnat_begin, pnat_end, random_data.cbegin(), random_data.cend());
        CHECK(mm2_copy_nat.first == pnat_end);

        //- Sort the buffer using synthetic pointers as iterators.
        //
        sw.start();
        std::sort(psyn_begin, psyn_end);
        sw.stop();
        el_syn = sw.elapsed_nsec();

        //- Verify that the newly-sorted buffer matches the sorted reference vector.
        //
        auto    mm2_sort_syn = std::mismatch(psyn_begin, psyn_end, sorted_data.cbegin(), sorted_data.cend());
        CHECK(mm2_sort_syn.first == psyn_end);
        auto    mm2_sort_nat = std::mismatch(pnat_begin, pnat_end, sorted_data.cbegin(), sorted_data.cend());
        CHECK(mm2_sort_nat.first == pnat_end);

        //- Copy the random data into the buffer using native pointers and verify its integrity.
        //
        std::copy(cbegin(random_data), random_data.cend(), pnat_begin);
        auto    mm1_copy_nat = std::mismatch(pnat_begin, pnat_end, random_data.cbegin(), random_data.cend());
        CHECK(mm1_copy_nat.first == pnat_end);

        //- Sort the buffer using native pointers as iterators.
        //
        sw.start();
        std::sort(pnat_begin, pnat_end);
        sw.stop();
        el_nat = sw.elapsed_nsec();

        //- Verify that the newly-sorted buffer matches the sorted reference vector.
        //
        auto    mm1_sort_nat = std::mismatch(pnat_begin, pnat_end, sorted_data.cbegin(), sorted_data.cend());
        CHECK(mm1_sort_nat.first == pnat_end);
    }

    destroy_range(pnat_begin, pnat_end);
    heap.reset_segments();
    native_first = !native_first;

    return timing_pair{el_nat, el_syn};
}

//--------------------------------------------------------------------------------------------------
//  Function:
//      run_pointer_sort_test<AS,DT>
//
//  Summary:
//      This function template manages the process of calling do_pointer_sort_test() multiple
//      times, accumulating the timings, and reporting the results.
//--------------------------------------------------------------------------------------------------
//
template<typename AllocStrategy, typename DataType>
std::tuple<std::string, std::vector<size_t>, std::vector<double>>
run_pointer_sort_tests(char const* stype, char const* dtype)
{
    size_t const    stat_rejects = 4;       //- Measurements to drop; highest & lowest.
    size_t const    sort_reps[]  = { 104u, 104u, 104u,
                                     54u,  54u,  54u,
                                     24u,  24u,  24u,
                                     14u,  14u,  14u,
                                     14u };

    std::string         tname(stype);
    std::string         name;
    std::vector<size_t> counts;
    std::vector<double> ratios;

    name.assign("sort/").append(stype).append("/").append(dtype);

    //- Time the sort operation for variously-sized arrays.
    //
    for (size_t i = 0;  i < max_element_index();  ++i)
    {
        size_t          nelem = elem_counts[i];
        size_t          sreps = sort_reps[i];
        timing_pair     timing;
        timing_vector   timings;

        for (size_t j = 0;  j < sreps;  ++j)
        {
            timing = do_pointer_sort_test<AllocStrategy, DataType>(nelem);
            timings.push_back(timing);
        }

        //- Sort the timings vector so we can reject highest/lowest timings.
        //
        std::sort(std::begin(timings), std::end(timings));

        //- Compute the synthetic-to-natural ratio, dropping outliers.
        //
        double      ratio;
        int64_t     el_nat_total = 0;
        int64_t     el_syn_total = 0;

        for (size_t j = 0;  j < timings.size();  ++j)
        {
            el_nat_total += timings[j].m_el_nat;
            el_syn_total += timings[j].m_el_syn;
        }

        el_nat_total = 0;
        el_syn_total = 0;

        for (size_t j = (stat_rejects/2);  j < (timings.size() - stat_rejects);  ++j)
        {
            el_nat_total += timings[j].m_el_nat;
            el_syn_total += timings[j].m_el_syn;
        }

        ratio = (double) el_syn_total / (double) el_nat_total;

        counts.push_back(nelem);
        ratios.push_back(ratio);

        std::printf("%s, %7.5f, %zu\n", name.c_str(), ratio, nelem);
        fflush(stdout);
    }
    std::printf("\n");

    if (size_t pos = tname.find("_strategy");  pos < tname.size())
    {
        tname.erase(pos);
    }
    return {tname, counts, ratios};
}

#endif  //- POINTER_SORT_TESTS_H_DEFINED
