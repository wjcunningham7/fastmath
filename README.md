FastMath Library
================

##Summary
This repository contains the FastMath library. It provides numerical approximations for math functions (especially for the Gauss Hypergeometric function) numerical integration wrappers, compact data structures (such as the FastBitset), and other useful utility functions. This package is intended to be copmpiled and installed on the platform on which it will ultimately be used.  It is **not** portable once installed.  **NOTE**: This package is officially maintained for Linux only, though some users have had success using it on MacOS as well.

##Environment Variables
There are several environment variables one should export in the $HOME/.bashrc file before installation:

1\. **FASTMATH\_HOME** - This will be the directory where the package is installed, and where other programs may find it later. Some options are

        $ export FASTMATH_HOME=/usr/local
        $ export FASTMATH_HOME=/opt
        $ export FASTMATH_HOME=$HOME/release

2\. **FBALIGN** - This is the alignment of the FastBitset. If your CPU supports AVX2 (AVX-512) it will be set automatically to 256 (512). If not set, the installation script will determine this for you. Options are

        $ export FBALIGN=64
        $ export FBALIGN=256
        $ export FBALIGN=512

3\. **PLATFORM** - This is the name of the system on which you are installing the package. If you are using a workstation, try

        $ export PLATFORM=$HOSTNAME

   whereas if you are on a cluster, you should set it to the name of the partition/queue you are using, e.g.

        $ export PLATFORM=general

   supposing that "general" is the name of the default partition.

4\. **BOOST\_ROOT** - This is the installation directory of Boost. See below for more information on dependencies. If you installed Boost using a package manager, or you installed it from source without specifying the installation directory, you do not need to set this variable.

5\. **CPATH** - This tells the compiler where to search for headers. If other packages cannot find the FastMath headers, try the following:

        $ export CPATH=$FASTMATH_HOME/include:$CPATH

6\. **LD\_LIBRARY\_PATH** - Likewise, if FastMath is installed into a non-standard directory, you may need to add the following for other packages to find the FastMath library:

        $ export LD_LIBRARY_PATH=$FASTMATH_HOME/lib:$LD_LIBRARY_PATH

##Prerequisites
The following packages should be installed prior to installation of this package:

1\. **Boost** v1.55.0 or newer. There is not guaranteed support for the newest releases. It can be installed via the Linux package managers in the following way:

   Ubuntu/Debian:

        $ sudo apt-get install libboost-all-dev

   Fedora/RedHat/CentOS:

        $ sudo yum install boost boost-devel

   Arch:

        $ sudo pacman -S boost boost-libs

   Gentoo:

        $ sudo emerge boost

   If you wish to install a particular version from source, visit the [Boost](www.boost.org) webpage and follow their instructions.

2\. **GNU Scientific Library** v1.13 or newer. It can be installed via the Linux package managers in the following way:

   Ubuntu/Debian:

        $ sudo apt-get install libgsl-dev

   Fedora/RedHat/CentOS:

        $ sudo yum install gsl gsl-devel

   Arch:

        $ sudo pacman -S gsl

   Gentoo:

        $ sudo emerge gsl

3\. **GCC/GCC++** v.5.4.0 or newer. Note that the version of Boost you choose will determine which compiler version you need, e.g., it is unwise to use a very old or very new compiler with the newest release of Boost.  To determine your compiler version, run

        $ gcc --version

4\. **Binutils** v2.24 or newer. This is needed so AVX2 and AVX-512 instructions are recognized by your assembler, supposing they are supported by the hardware.  To determine your assembler version, run

        $ as -v

5\. **Autotools** v2.69 or newer. This is the Linux utility used to generate the configure and Makefile scripts. It may be installed via the package managers in the following way:

   Ubuntu/Debian:

        $ sudo apt-get install autoconf automake

   Fedora/RedHat/CentOS:

        $ sudo yum install autoconf automake

   Arch:

        $ sudo pacman -S autoconf automake

   Gentoo:

        $ sudo emerge autoconf automake

   To determine the version currently installed, run

        $ autoconf --version

## Compilation and Installation
The script **install** will compile the code. Run it with

        $ ./install

Once it has been executed, if there are no errors you may run

        $ sudo make install

to install it system-wide.  Make sure all environment variables are set and all prerequisites are satisfied before attempting this.  To clean up the directory, you can reverse the above with

        $ make distclean

If you wish to uninstall it system-wide, you should first run

        $ make uninstall

before cleaning up.

## Testing
There are several test scripts which are used to verify the assembly code used in inc/FastBitset.h is working. You can try this with

        $ (cd test && sh ./test.sh)

## Usage
If you intend to use this with another package, and that package is compiled using autotools, you can use the M4 file **ax_fastmath.m4**.  Alternatively, you can link directly against the library with

        $ -L $FASTMATH_HOME/lib -lfastmath$_PLATFORM

and include the headers with

        $ -I $FASTMATH_HOME/include

##Package Contents
* FastMath:  contains approximations of many mathematical functions. In particular, the \_2F1() function permits one to specify the number of terms _or_ the desired error when using a series solution. The header also defines many constants.

* FastBitset: a header-only class used for efficient bitset algorithms. The Bitvector object is a vector of FastBitsets, often used to represent an adjacency matrix.

* FastNumInt: A wrapper for numerical integration in 1D and 2D.

* Stopwatch: A data structure and some functions for a stopwatch accurate to the millisecond.

* ProgressBar: A data structure and some functions used to produce an on-screen progress bar, often useful for Monte Carlo experiments.

* MersenneRNG: A data structure based on Boost's Mersenne Twister (19937) random number generator, useful for efficiently generating reliably random numbers in large simulations.

##Maintainers
Please direct all questions and report all bugs to Will Cunningham at <wcunningham AT perimeterinstitute DOT ca>

(C) Will Cunningham 2014-2019
