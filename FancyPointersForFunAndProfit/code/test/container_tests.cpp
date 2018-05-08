//==================================================================================================
//  File:   container_tests.cpp
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include "container_tests.h"

template class simple_string<rhx_allocator<char, wrapper_strategy>>;
template class simple_string<rhx_allocator<char, based_2d_xl_strategy>>;
template class simple_string<rhx_allocator<char, based_2d_sm_strategy>>;
template class simple_string<rhx_allocator<char, based_2d_msk_strategy>>;
template class simple_string<rhx_allocator<char, offset_strategy>>;

template<class AS>
void
do_test_string()
{
    using   string_type = simple_string<rhx_allocator<char, AS>>;

    std::string     ss1("good-bye, world");

    string_type     s1;
    string_type     s2("hello, world");
    string_type     s3(ss1);
    string_type     s4(ss1);
    string_type     s5(std::move(s4));

    std::map<string_type, int>      mmap;

    mmap["test1"] = 1;
    mmap["test2"] = 2;

    std::cout << s2 << "! " << s5 << "!" << std::endl;
    for (auto const& p : mmap)
    {
        std::cout << "  " << p.first << " : " << p.second << std::endl;
    }
    std::cout << std::endl;
}


void
test_string_ops()
{
    do_test_string<wrapper_strategy>();
    do_test_string<based_2d_xl_strategy>();
    do_test_string<based_2d_sm_strategy>();
    do_test_string<based_2d_msk_strategy>();
    do_test_string<offset_strategy>();
}


static int  outer = 961;
static int  inner = 231;

template<class MT> void
print_map(MT const& map)
{
    std::cout << "orignal map address is: 0x" << std::hex << (uintptr_t) &(map) << std::endl;
    for (auto const& elem : map)
    {
        std::cout << elem.first << std::endl;
        for (auto const& val : elem.second)
        {
            std::cout << "    " <<  val << "   0x" << std::hex << (uintptr_t) &val << std::endl;            
        }
    }
    std::cout << std::endl;
}

template<typename AllocStrategy>
void
do_map_test(char const* strategy_name, bool do_reloc)
{
    //- Various type aliases to aid readability.
    //
    using strategy   = AllocStrategy;
    using syn_string = simple_string<rhx_allocator<char, strategy>>;
    using syn_list   = std::list<syn_string, rhx_allocator<syn_string, strategy>>;

    using syn_less   = std::less<syn_string>;
    using syn_pair   = std::pair<const syn_string, syn_list>;
    using syn_alloc  = rhx_allocator<syn_pair, strategy>;
    using syn_map    = std::map<syn_string, syn_list, syn_less, syn_alloc>;

    auto    spmap = allocate<syn_map, strategy>();
    auto    spkey = allocate<syn_string, strategy>();
    auto    spval = allocate<syn_string, strategy>();

    char    key_str[128], val_str[128];

    for (int i = outer;  i < (outer + 3);  ++i)
    {
        sprintf(key_str, "this is key string #%d", i);
        spkey->assign(key_str);

        for (int j = inner;  j < (inner + 5);  ++j)
        {
            sprintf(val_str, "this is string #%d created for syn_map<syn_string, syn_list>", j);
            spval->assign(val_str);
            (*spmap)[*spkey].push_back(*spval);
        }
        inner += 10;
    }
    outer += 10;

    std::cout << "***********************" << std::endl;
    std::cout << "*****  TEST MAP  ******" << std::endl;
    std::cout << "for strategy: " << strategy_name << std::endl;
    print_map(*spmap);

    if (do_reloc)
    {
        std::cout << "------  SWAPPING  ------" << std::endl;
        strategy::swap_segments();
        print_map(*spmap);
    }

    strategy::reset_segments();
}

void
test_map_ops()
{
    #define DO_MAP_TEST(AS, RELOC)  do_map_test<AS>(#AS, RELOC)
#if 0
    DO_MAP_TEST(based_2d_xl_strategy, true);
#else
    DO_MAP_TEST(wrapper_strategy, false);
    DO_MAP_TEST(offset_strategy, false);
    DO_MAP_TEST(based_2d_xl_strategy, true);
    DO_MAP_TEST(based_2d_sm_strategy, true);
    DO_MAP_TEST(based_2d_msk_strategy, true);
#endif
}

