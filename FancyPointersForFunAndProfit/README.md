## Overview

This directory contains a PDF and supporting materials for my C++Now 2018 talk entitled **Fancy Pointers for Fun and Profit**.  It contains the source code used to create the results for the talk, as well as the initial results files and a spreadsheet with the graphs used to create the presentation.

## Contents

The source code is contained in the ``code/include``, ``code/src``, and ``code/test`` subdirectories.  The ``code`` subdirectory contains a solution and project file for Visual Studio 2017.

The files in ``code/include`` and ``code/src`` observe the following naming conventions:
* File names ending in ``_addressing.h/.cpp`` define _addressing models_;
* File names ending in ``_storage.h/.cpp`` define _storage models_;
* File names ending in ``_pointer.h`` define _pointer interfaces_ (there is only one pointer interface used in this project, and it is defined in ``pointer_interface.h``);
* File names ending in ``_allocation_strategy.h/.cpp`` define _allocation strategies_; and,
* File names ending in ``_allocator.h/.cpp`` define C++14-compatible allocator types.

The terms _addressing model_, _storage model_, _pointer interface_, and _allocation strategy_ are all defined in the presentation.

## Building the Tests

On Windows, just open the Visual Studio solution file - you should be able to immediately build and run.

On Linux, you will need CMake 3.5 or higher installed. Change directory to ``code`` and run the ``setup_cmake_builds.sh`` shell script.  At that point you can ``cd`` to the appropriate build subdirectory and make the tests.

```
$ cd <some-work-dir>/code
$ ./setup_cmake_buiilds.sh
$ cd build-debug-clang 
$ make
$ ./fancy [args]
```

## Other Work

For my other C++ work, see my site [The State Machine](https://bobsteagall.com).
