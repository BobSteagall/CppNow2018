//==================================================================================================
//  File:
//      common.cpp
//
//  Summary:
//      Defines services that generate data for testing.
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include "common.h"
#include <random>

using namespace std;

test_struct::test_struct()
:   m1(999)
,   m2(998)
{
    memset(m3, 0, sizeof(m3));
}

#ifdef USE_OOL_TEST_STRUCT_MEMBERS

test_struct::test_struct(test_struct const& other)
:   m1(other.m1)
,   m2(other.m2)
{
    memcpy(&m3[0], &other.m3[0], sizeof(m3));
}

test_struct&
test_struct::operator =(test_struct const& rhs)
{
    if (&rhs != this)
    {
        m1 = rhs.m1;
        m2 = rhs.m2;
        memcpy(&m3[0], &rhs.m3[0], sizeof(m3));
    }
    return *this;
}

#endif

template<typename INT_T>
using dist = uniform_int_distribution<INT_T>;

template<>
vector<uint32_t>
generate_test_data(size_t num_elements)
{
    random_device       rnd;
    mt19937             gen(rnd());
    dist<uint32_t>      dist;
    vector<uint32_t>    data;

    data.reserve(num_elements);
    
    while (num_elements-- > 0)    
    {
        data.push_back(dist(gen));
    }

    return data;
}

string
generate_test_string(size_t max_size)
{
    random_device   rnd;
    mt19937         gen(rnd());
    dist<uint64_t>  len_dist(2, max_size);
    dist<uint32_t>  chr_dist(32, 126);
    string          str;

    str.resize(len_dist(gen));

    for (size_t i = 0;  i < str.size();  ++i)
    {
        str[i] = static_cast<char>(chr_dist(gen));
    }

    return str;
}

template<>
string
generate_data()
{
    return generate_test_string(64);
}

test_struct
generate_test_struct()
{
    random_device   rnd;
    mt19937_64      gen(rnd());
    dist<uint64_t>  num_dist(1, 1000);
    dist<uint32_t>  chr_dist(32, 126);
    test_struct     ts;

    memset(&ts, 0, sizeof(test_struct));
    ts.m1 = num_dist(gen);
    ts.m2 = num_dist(gen);

    for (size_t i = 0;  i < sizeof(ts.m3);  ++i)
    {
        do
        {
            ts.m3[i] = static_cast<char>(chr_dist(gen));
        }
        while (ts.m3[i] == '\"'  ||  ts.m3[i] == '\'');
    }
    ts.m3[sizeof(ts.m3)-1] = '\0';

    return ts;
}

template<>
test_struct
generate_data()
{
    return generate_test_struct();
}

template<>
vector<uint64_t>
generate_test_data(size_t num_elements)
{
    random_device       rnd;
    mt19937_64          gen(rnd());
    dist<uint64_t>      dist;
    vector<uint64_t>    data;

    data.reserve(num_elements);
    
    while (num_elements-- > 0)    
    {
        data.push_back(dist(gen));
    }

    return data;
}

template<>
vector<string>
generate_test_data(size_t num_elements)
{
    random_device       rnd;
    mt19937             gen(rnd());
    dist<uint32_t>      len_dist(2, sizeof(string)/2);
    dist<uint32_t>      chr_dist(32, 126);
    vector<string>      data;

    data.reserve(num_elements);

    while (num_elements-- > 0)
    {
        string str;
        uint32_t len = len_dist(gen);

        while (len-- > 0)
        {
            str.push_back(static_cast<char>(chr_dist(gen)));
        }
        data.push_back(str);
    }

    return data;
}

template<>
vector<test_struct>
generate_test_data(size_t num_elements)
{
    random_device       rnd;
    mt19937_64          gen(rnd());
    dist<uint64_t>      num_dist(1, 1000);
    dist<uint32_t>      chr_dist(32, 126);
    vector<test_struct> data;

    data.reserve(num_elements);

    while (num_elements-- > 0)
    {
        test_struct ts;

        ts.m1 = num_dist(gen);
        ts.m2 = num_dist(gen);

        for (size_t i = 0;  i < sizeof(ts.m3);  ++i)
        {
            do
            {
                ts.m3[i] = static_cast<char>(chr_dist(gen));
            }
            while (ts.m3[i] == '\"'  ||  ts.m3[i] == '\'');
        }
        ts.m3[sizeof(ts.m3)-1] = '\0';

        data.push_back(ts);
    }

    return data;
}


