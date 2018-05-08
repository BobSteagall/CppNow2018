//==================================================================================================
//  File:
//      pointer_copy_tests.h
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#ifndef POINTER_COPY_TESTS_H_DEFINED
#define POINTER_COPY_TESTS_H_DEFINED

#include "pointer_tests.h"

//--------------------------------------------------------------------------------------------------
//  Function:
//      test_copy<II,OI>
//
//  Summary:
//      This function template performs a naive copy of elements from a source array to a
//      destination array.  Both sequences must have the same size.  This function is used in
//      testing below to avoid the optimizations using memcpy() implemented by std::copy().
//--------------------------------------------------------------------------------------------------
//
template<typename II, typename OI, typename TAG>
void
test_copy_imp(II src_begin, II src_end, OI dst_begin, OI dst_end, TAG)
{
    //- Make sure the compiler doesn't try to optimize the loop into something like memcpy().
    //
    static  volatile uint64_t   dummy = 0;

    //- Make sure the sizes contents_match; this is just some extra paranoia when using
    //  synthetic pointers.
    //
    CHECK(std::distance(src_end, src_begin) == std::distance(dst_end, dst_begin));

    for (;  src_begin != src_end;  ++src_begin, ++dst_begin)
    {
        *dst_begin = *src_begin;
        ++dummy;
    }
}
//------
//
template<typename II, typename OI>
void
test_copy_imp(II src_begin, II src_end, OI dst_begin, OI dst_end, std::random_access_iterator_tag)
{
    //- Make sure the compiler doesn't try to optimize the loop into something like memcpy().
    //
    static  volatile uint64_t   dummy = 0;

    //- Make sure the sizes contents_match; this is just some extra paranoia when using
    //  synthetic pointers.
    //
    CHECK((src_end - src_begin) == (dst_end - dst_begin));

    //- Assume the memory is contiguous and get actual pointers as the begin/end source iters.
    //
    auto const*     tmp_begin = std::addressof(*src_begin);
    auto const*     tmp_end   = tmp_begin + (src_end - src_begin);

    for (;  tmp_begin != tmp_end;  ++tmp_begin, ++dst_begin)
    {
        *dst_begin = *tmp_begin;
        ++dummy;
    }
}


//------
//
template<typename II, typename OI>
inline void
test_copy(II src_begin, II src_end, OI dst_begin, OI dst_end)
{
    using category = typename std::iterator_traits<II>::iterator_category;
    test_copy_imp(src_begin, src_end, dst_begin, dst_end, category());
}

//--------------------------------------------------------------------------------------------------
//  Function:
//      do_pointer_copy_test<AS,DT>
//
//  Summary:
//      This function template measures the time it takes to copy elements from a source vector
//      to a destination array.  It measures elapsed time twice: once for the case when the
//      destination is accessed by native pointers, and once for the case when the destination
//      is accessed with synthetic pointers.
//--------------------------------------------------------------------------------------------------
//
template<typename AllocStrategy, typename DataType>
timing_pair
do_pointer_copy_test(size_t nelem, size_t nreps)
{
    using syn_ptr_void = typename AllocStrategy::void_pointer;
    using syn_ptr_data = typename AllocStrategy::template rebind_pointer<DataType>;

    //- Instantiate a std::vector and fill it with some randomly-generated test data.
    //
    std::vector<DataType>   random_data(generate_test_data<DataType>(nelem));

    //- Instantiate the allocation strategy object and allocate a buffer of appropriate size using
    //  the pointer types expressed by the strategy's public interface.
    //
    AllocStrategy   heap;
    syn_ptr_void    psyn_void  = heap.allocate(nelem*sizeof(DataType));
    syn_ptr_data    psyn_begin = static_cast<syn_ptr_data>(psyn_void);
    syn_ptr_data    psyn_end   = psyn_begin + nelem;

    //- Initialize the raw storage with dummy values so we can just use copy() below.  Yes, it
    //  wastes time, but it makes the code simpler.
    //
    std::uninitialized_fill(psyn_begin, psyn_end, DataType());

    //- Get a couple of corresponding natural pointers for comparison purposes.
    //
    DataType*   pnat_begin = psyn_begin;
    DataType*   pnat_end   = psyn_end;

    //- Get ready to time the copies.
    //
    static bool native_first = true;
    stopwatch   sw;
    int64_t     el_nat = 0;
    int64_t     el_syn = 0;

    //- Do a dummy copy and touch all the pages to minimize the effects of cache misses later.
    //
    test_copy(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);

    //- We're going to alternate between native operations first and synthetic operations first
    //  in order to avoid any bias in the average due to the order.
    //
    if (native_first)
    {
        sw.start();
        for (size_t i = 0;  i < nreps;  ++i)
        {
            test_copy(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);
        }
        sw.stop();
        el_nat = sw.elapsed_nsec();

        auto    mm1 = std::mismatch(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);
        CHECK(mm1.first == std::cend(random_data));

        sw.start();
        for (size_t i = 0;  i < nreps;  ++i)
        {
            test_copy(std::cbegin(random_data), std::cend(random_data), psyn_begin, psyn_end);
        }
        sw.stop();
        el_syn = sw.elapsed_nsec();

        auto    mm2 = std::mismatch(std::cbegin(random_data), std::cend(random_data), psyn_begin, psyn_end);
        CHECK(mm2.first == std::cend(random_data));
        auto    mm3 = std::mismatch(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);
        CHECK(mm3.first == std::cend(random_data));
    }
    else    //- synthetic first
    {
        sw.start();
        for (size_t i = 0;  i < nreps;  ++i)
        {
            test_copy(std::cbegin(random_data), std::cend(random_data), psyn_begin, psyn_end);
        }
        sw.stop();
        el_syn = sw.elapsed_nsec();

        auto    mm2 = std::mismatch(std::cbegin(random_data), std::cend(random_data), psyn_begin, psyn_end);
        CHECK(mm2.first == std::cend(random_data));
        auto    mm3 = std::mismatch(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);
        CHECK(mm3.first == std::cend(random_data));

        sw.start();
        for (size_t i = 0;  i < nreps;  ++i)
        {
            test_copy(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);
        }
        sw.stop();
        el_nat = sw.elapsed_nsec();

        auto    mm1 = std::mismatch(std::cbegin(random_data), std::cend(random_data), pnat_begin, pnat_end);
        CHECK(mm1.first == std::cend(random_data));
    }

    destroy_range(pnat_begin, pnat_end);
    heap.reset_segments();
    native_first = !native_first;

    return timing_pair{el_nat, el_syn};
}

//--------------------------------------------------------------------------------------------------
//  Function:
//      run_pointer_copy_test<AS,DT>
//
//  Summary:
//      This function template manages the process of calling do_pointer_copy_test() multiple
//      times, accumulating the timings, and reporting the results.
//--------------------------------------------------------------------------------------------------
//
template<typename AllocStrategy, typename DataType>
std::tuple<std::string, std::vector<size_t>, std::vector<double>>
run_pointer_copy_tests(char const* stype, char const* dtype)
{
    size_t const    stat_repeats = 16;      //- Times to repeat the test.
    size_t const    stat_rejects = 6;       //- Measurements to drop; highest & lowest.

    std::string         tname(stype);
    std::string         name;
    std::vector<size_t> counts;
    std::vector<double> ratios;

    name.assign("copy/").append(stype).append("/").append(dtype);

    //- Time the copy operation for variously-sized arrays.
    //
    for (size_t i = 0;  i < max_element_index();  ++i)
    {
        size_t          nelem    = elem_counts[i];
        size_t          run_reps = std::max((size_t)1, (size_t)(10'000'000/nelem));
        timing_pair     timing;
        timing_vector   timings;

        for (size_t j = 0;  j < stat_repeats;  ++j)
        {
            timing = do_pointer_copy_test<AllocStrategy, DataType>(nelem, run_reps);
            timings.push_back(timing);
        }

        //- Sort the timings vector so we can reject highest/lowest timings.
        //
        sort(begin(timings), end(timings));

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

#endif  //-  POINTER_COPY_TESTS_H_DEFINED
