===================
Screening Interview
===================

Welcome to the screening interview of Naive Systems!

This is a program that parses URLs and extracts query parameters.  We prepared
some code for you to get started.  You can work on any of the following
operating systems:

- Linux
- macOS
- Windows

You may write code conforming to any well-known C standard: C89, C99, or C11.
Please do not introduce new third-party code.

To complete the screening interview, you should create 3 to 4 git commits
directly in this repository:

1. Build the program.

   This is a program with multiple source files and third-party dependencies.
   You are required to build the program with a well-known build system (e.g.
   Makefile, CMake, Bazel, ...).  If you choose to build the program with an
   IDE (e.g. Visual Studio, Xcode, CLion, ...), please submit your project
   files (e.g. .vcxproj, .xcodeproj).

2. Finish the function parse_query_string() in parse.c.

   You may #include any standard C headers such as <string.h>, <stdlib.h> and
   many others.  If you need to write helper functions, put them in parse.c as
   well.

   After the function is completed, running the program should print an OK for
   each passed testcase.

3. Add more testcases.

   After completing the function above, how would you test it?

   Edit testcases.json to add more testcases.  Pay attention to corner cases.
   Make your program and testcases robust.  Think about real-world scenarios.

4. Fix memory errors in main.c.

   We intentionally left a few memory errors in main.c.  Try your best to fix
   them!

In the end, please create a git bundle with all of your git commits and send it
back to the interviewer::

  git bundle create submission.bundle HEAD master

Thanks for taking your time to do this screening interview!
