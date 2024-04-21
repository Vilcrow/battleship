For unit testing used the CppUTest framework (https://cpputest.github.io).

Linux

There is a Debian and an Ubuntu package available for CppUTest.
This is by far the easiest way to install it, via:

    $ apt-get install cpputest

For Arch:

    $ pacman -S cpputest

MacOSX

For Mac, a Homebrew package is available too. You can install via:

    $ brew install cpputest

--------------------------------------------------------------------------------

If your CppUTest include directories are not located in the system directories
you’ll need to specify it in the Makefile. A special variable is provided for
this purposes:

    CPPUTEST_HOME

--------------------------------------------------------------------------------

Run the 'run_tests.sh' script to compile and run tests.
