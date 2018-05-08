cmake_minimum_required (VERSION 3.5)
project (FancyPointer)

include_directories(include)

set(Sources
        include/based_2d_msk_addressing.h
        include/based_2d_msk_storage.h
        include/based_2d_sm_addressing.h
        include/based_2d_sm_storage.h
        include/based_2d_xl_addressing.h
        include/based_2d_xl_storage.h
        include/monotonic_allocation_strategy.h
        include/offset_addressing.h
        include/offset_storage.h
        include/rhx_allocator.h
        include/stopwatch.h
        include/storage_base.h
        include/synthetic_pointer.h
        include/wrapper_addressing.h
        include/wrapper_storage.h

        src/based_2d_msk_storage.cpp
        src/based_2d_sm_storage.cpp
        src/based_2d_xl_storage.cpp
        src/monotonic_allocation_strategy.cpp
        src/offset_storage.cpp
        src/storage_base.cpp
        src/wrapper_storage.cpp

        test/common.cpp
        test/common.h
        test/container_tests.cpp
        test/container_tests.h
        test/main.cpp
        test/pointer_cast_tests.h
        test/pointer_copy_tests.h
        test/pointer_sort_tests.h
        test/pointer_tests.cpp
        test/pointer_tests.h
        test/scd_tests.cpp
)

add_executable(fancy ${Sources})

set(CMAKE_VERBOSE_MAKEFILE 1)

if(CXX_COMPILER STREQUAL clang++)
    set(CMAKE_C_COMPILER   clang)
    set(CMAKE_CXX_COMPILER clang++)
else()
    set(CMAKE_C_COMPILER   gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

if(CMAKE_CXX_COMPILER STREQUAL clang++)

    set(CMAKE_C_FLAGS_DEBUG   "-g -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++17 -stdlib=libc++ -Wall -pedantic -Wextra -Wno-unused-parameter")

    set(CMAKE_C_FLAGS_RELEASE   "-O2 -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++17 -Wall -pedantic -Wextra -Wno-unused-parameter")

elseif(CMAKE_CXX_COMPILER STREQUAL g++)

    set(CMAKE_C_FLAGS_DEBUG   "-g -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++17 -Wall -pedantic -Wextra")

    set(CMAKE_C_FLAGS_RELEASE   "-O2 -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++17 -Wall -pedantic -Wextra")

endif()
