//==================================================================================================
//  File:
//      main.cpp
//
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

void    run_pointer_tests(bool do_copy, bool do_sort);
void    test_string_ops();
void    test_map_ops();
void    test_scd();

bool    copy_flag    = true;
bool    sort_flag    = true;
bool    strop_flag   = true;
bool    map_flag     = false;
bool    heap_flag    = false;
bool    verbose_flag = false;
size_t  max_elem_idx = 13;

bool
verbose_output()
{
    return verbose_flag;
}

size_t
max_ptr_op_count_index()
{
    return max_elem_idx;
}


int main(int argc, char* argv[])
{
    for (int i = 1;  i < argc;  ++i)
    {
        std::string     arg(argv[i]);

        if (arg == "-v")
        {
            verbose_flag = true;
        }
        else if (arg == "-p")
        {
            if (++i < argc)
            {
                max_elem_idx = (size_t) atoi(argv[i]);

            }
        }
        else if (arg == "-c")
        {
            copy_flag  = true;
            sort_flag  = false;
            strop_flag = false;
            map_flag   = false;
            heap_flag  = false;
        }
        else if (arg == "-s")
        {
            copy_flag  = false;
            sort_flag  = true;
            strop_flag = false;
            map_flag   = false;
            heap_flag  = false;
        }
        else if (arg == "-ss")
        {
            copy_flag  = false;
            sort_flag  = false;
            strop_flag = true;
            map_flag   = false;
            heap_flag  = false;
        }
        else if (arg == "-m")
        {
            copy_flag  = false;
            sort_flag  = false;
            strop_flag = false;
            map_flag   = true;
            heap_flag  = false;
        }
        else if (arg == "-h")
        {
            copy_flag  = false;
            sort_flag  = false;
            strop_flag = false;
            map_flag   = false;
            heap_flag  = true;
        }
    }

    if (copy_flag || sort_flag)
        run_pointer_tests(copy_flag, sort_flag);

    if (strop_flag)
        test_string_ops();

    if (map_flag)
        test_map_ops();

    if (heap_flag)
        test_scd();

    return 0;
}
