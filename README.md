# SGAL project

1. Introduction

The Scene Graph Algorithms Library (SGAL) is a toolkit that supports the
construction and maintenance of directed acyclic graphs that represent
scenes and models in three-dimensional space. The toolkit consists of
two libraries, namely SGAL and SCGAL, and an application, called player,
which enables users to visualize the scene in an interactive manner. The
player parses input files that describe the scene in a degenerate yet
extended VRML format. The format is degenerate, as not all VRML features
are supported (yet). However, it has been extended to support several
new features. For example, it has been extended with several geometry
node types, such as the Assembly node type, which represents assemblies
or subassemblies; the AssemblyPart node type, which represents parts of
assemblies; the SphericalGaussianMap node type, which represents
(spherical) Gaussian maps of convex polytopes in 3D, and the
ArrangementOnSphere, which represents arrangements of geodesic arcs
embedded on the sphere. The extended format also supports, for example, a
node type called ExactCoordinate, which represents exact coordinates, and
enables the provision of exact rational coordinates as input. Each node
object of the three types AssemblyPart, SphericalGaussianMap, and
ArrangementOnSphere internally maintains the CGAL data structure that
represents an arrangement of geodesic arcs embedded on the sphere.

2. Installing

The code is cross-platform. Building and executing is supported on Windows
using MSVC and Unix-like systems using GNU tools. It is not supported on
Mac OS (yet).

Ideally, building SGAL amounts to:

  cd  SGAL-1.0                                # go to SGAL directory
  cmake .                                     # configure SGAL
  make                                        # build the SGAL libraries

However, cmake is not supported yet...

2.1. Prerequisites
Installing SGAL requires a few components to be installed apriori: a
supported compiler (see Section 2.3.), bison (see Section 2.5.2), flex,
Boost, gl2ps, Magick++, CGAL, GMP, and MPFR; see Section 2.5. for more
details on essential third party software.

2.2. Downloading
You can obtain the most up-to-date sources from the git repository
at https://bitbucket.org/.

You need to register and obtain a user account. You also need to add an
ssh key for every machine you would like to access the repository from
via the clone command below. Once you have an account, an admin of the
repository, e.g., Efi (efifogel@gmail.com), can grant you the appropriate
access permissions. Currently, the repository is closed to the public.

  git clone git@bitbucket.org:efogel/ego.git

If you are on Windows you may not download and run SGAL-1.0-Setup.exe, as
such an installer does not exist (yet).

Directory Content
--------- -------
bison     a submodule for the bison sources; see Section 2.5.2.
data      data files
docs      documents
figs      figures
make      makefile files
src       source files

2.3. Supported Compilers
In order to build the SGAL libraries and the executables, you need a
C++ compiler. SGAL 1.0 is supported for the following combinations of
compilers and operating systems:

Compiler                      | Operating System
------------------------------|-------------------------------------
GNU g++ 4.2 or later          | Linux
MS Visual C++ 9.0, 10.0, 12.0 | MS Windows 95/98/2000/XP/NT4/Vista/7

2.4. SGAL Libraries
SGAL is split into two libraries, namely SGAL and SCGAL. The former does
not depend on CGAL (Computational Geometry Algorithm Library). This is
going to change in the near future. The plan is still to split SGAL into
two libraries, namely SLGAL and SUGAL, but using a different distinction
criterion. Both libraries will depend on CGAL. However, SIGAL will use
limited arithmetic precision, while SEGAL will use unlimited precision.

2.5. Essential Third Party Libraries
The focus of SGAL is on scene graph handling. It relies on other highly
specialized libraries and software for other tasks. When an external
library is used and the external library is not installed in a standard
place (e.g.,, /usr/[local/]include and /usr/[local/]lib in Linux), the
directories where the header source files and the library object file
reside must be specified. urrently (until we switch to cmake) this is
done using environment variables. On Windows, for example, there is no
canonical directory for where to find external libraries. Thus, for each
external library used one or more environment variables must be set as
follows. For a given external libarary, say EXTLIB, set the environment
variable EXTLIB_DIR to point at the directory where the external library
is installed. If the header source and the library object files are not
installed under $EXTLIB_DIR/include and $EXTLIB_DIR/lib, respectively,
set the environment variables EXTLIB_INC_DIR and EXTLIB_LIB_DIR to point
at the directories where the header sources and the library object file
are installed, respectively.

In addition to the header source and library object files, the shared
library object files (.so on Linux and .dll on Windows) must be made
accessible to the loader. On Windows, this implies that the shared
libraries must be placed in a directory that is in the PATH. On linux,
this can be done by placing the shared libraries is a directory that
is in the LD_LIBRARY_PATH.

2.5.1. List of required packages.

2.5.1.1. Required packages of Ubuntu 12.04:
* libboost-dev
* libmagick++-dev
* libgl2ps-dev
* libxxf86vm-dev
* libgmp-dev

2.5.1.2. Required items on Windows
* Boost
* ImageMagick
* gl2ps
* GMP
* MPFR
* flex (cygwin)
* bison (cygwin)

In fact, I needed more packages from cygwin to build bison (see
Section 2.5.2.1.) , but I hope that this is temporary.

2.5.1. Boost
Boost is a set of portable C++ source libraries. Most Boost libraries
are library-less (headers only), but a few of them need to be compiled
or installed as binaries.

SGAL requires Boost version 1.39 or higher. In particular, the header
files and the Boost.Thread, Boost.System, and Boost.Program_options
binaries.

On Windows, as auto-linking is used, you also need the binaries of
Boost.Serialization and Boost.DateTime, but the dependency is artificial
and used only at link-time: the SGAL libraries do not depend on the
DLL's of those two libraries.

If Boost is not installed on your system already, you can obtain it from
http://www.boost.org/. For Windows you can download an installer from
http://www.boostpro.com/download/. Since Boost.Thread is required, make
sure to either install the precompiled libraries for your compiler or
build libboost-thread and libboost-system.

If Boost is not installed in a standard place, you must set the
environment variable BOOST_DIR to point at the directory where Boost is
installed. The header source files of Boost are installed by default
immediately under the main directory (and not under the 'include'
subdirectory. Therefore, you also need to set BOOST_INC_DIR to point at
that directory where the header source files are installed. For example:

  BOOST_DIR=D:/boost/boost_1_50
  BOOST_INC_DIR=D:/boost/boost_1_50

2.5.2. bison and flex

2.5.2.1. bison
Bison is a general-purpose parser generator that converts an annotated
context-free grammar into a deterministic LR or generalized LR (GLR)
parser employing LALR(1) parser tables. For more information see
http://www.gnu.org/software/bison/

The code of SGAL is using an advanced feature of bison available in
bison version 2.7 and above. The binaries of this release has not made
it yet to the standard distrubution channels (e.g., Ubuntu, Red Hat,
cygwin, and bison for Windows). Thus, it must be built from sources.
A bison submodule exists in the SGAL git repository for convenience.
To build bison you need to install apriori the following components:

  gcc,
  autopoint
  make
  automake
  autoconf
  texinfo
  pkg-config
  rsync

This holds for Linux as well as for Windows, assuming you are going to
build bison via cygwin. First you need to update the bison sources.
Assuming you are in the main SGAL directory, issue the following command:

  git submodule update

If, for some reason, you want the latest bison sources, issue the
following command:

  git submodule -q foreach git pull -q origin master

To obtain the latest version of bison from the its git repository
(using anonymus checkout) issue:

  git clone git://git.savannah.gnu.org/bison.git

Then, change directory to the bison directory. Typically, building
bison, ammounts to:

  ./bootstrap
  ./configure
  make
  
Then, place the generated bison executable (tests/bison) somewhere
in your PATH, so that it is picked up before any other bison executable
that may reside on your system. I place it under $ROOT/bin, which is in
my PATH; see Section 2.6.1. for a discusion about environment variables.

On Windows, you can build an executable using GNU tools from a cygwin
bash terminal. Unfortunately, bootstrap fails, but I found a workaround.
The issue is described at
http://stackoverflow.com/questions/17143607/i-cannot-build-bison-on-cygwin-using-bootstrap/17157991?noredirect=1#17157991. In short,
- Edit the file bootstrap.conf and search for the statement:

  gnulib_tool_option_extras='--symlink --makefile-name=gnulib.mk'
  
remove '--symlink' option. Then, run the bootstrap script while forcing
(hard) copies over symbolic links and procede as above:

  ./bootstrap --copy
  ./configure
  make

2.5.2.2. flex (Fast Lexical Analyzer)
Flex is a fast lexical analyser generator. It is a tool for generating
programs that perform pattern-matching on text. Flex is a free (but
non-GNU) implementation of the original Unix lex program. To download
the software, get the documentation, and more, see the Flex home
page at http://flex.sourceforge.net/.

2.5.3. Magick++ (ImageMagick)
ImageMagick® is a software suite to create, edit, compose, or convert
bitmap images; see http://www.imagemagick.org/

If ImageMagick is not installed in a standard place, you must set the
environment variable MAGICK_DIR to point at the directory where
ImageMagick is installed, e.g.,

  MAGICK_DIR=d:/ImageMagick-6.8.3-Q16

2.5.4. CGAL
The Computational Geometry Algorithms Library (CGAL) is an open
source software library that provides industrial and academic users
with easy access to reliable implementations of efficient geometric
algorithms.

Ideally, building CGAL amounts to:

  cd CGAL-4.2                                 # go to CGAL directory
  cmake .                                     # configure CGAL
  make                                        # build the CGAL libraries

For more information see
http://www.cgal.org/Manual/latest/doc_html/installation_manual/contents.html

Many SGAL features supported by the SCGAL library of SGAL depend on the
latest release and on features developed in various branches. A feature
of SGAL can be (typically) enabled and disabled by setting an
appropriate environment variable. For example, consider the
"Lines Through Segments" feature. You need to set the environment
variable USE_LTS to 1 to enable this feature, and enable the
corresponding CGAL feature. The corresponding CGAL feature has been
developed in the CGAL branch origin/Lines_through_segments-pmoeller.
You need to merge the source of this branch with the sources you use to
build CGAL. The sequence of commands bellow will merge all CGAL branches
used to develope relevant CGAL features.

  git checkout -b combined origin/master
  git merge origin/Aos_2-remove_opt-efif
  git merge origin/Lines_through_segments-pmoeller
  git merge origin/Envelope_voronoi_2-ophirset
  git merge origin/Cubical_gaussian_map-efif

If CGAL is not installed in a standard place (and it is probably nor in
Linux neither in Windows), you must set the environment variable CGAL_DIR
to point at the directory where CGAL is installed, e.g.,

  CGAL_DIR=/usr/local/CGAL/CGAL-combined
  
2.5.5. GMP and MPFR
The SGAL library SCGAL requires CGAL, and CGAL in turn requires GMP and
MPFR, which are libraries for multi precision integers and rational
numbers, and for multi precision floating point numbers.

CGAL combines floating point arithmetic with exact arithmetic, in order
to be efficient and reliable. Cgal has a built-in number type for that,
but GMP and MPFR provide a faster solution, and we recommend to use them.

Having GMP version 4.2 or higher and MPFR version 2.2.1 or higher installed
is recommended. These libraries can be obtained from http://gmplib.org/ and
http://www.mpfr.org/, respectively.

As Visual C++ is not properly supported by the GMP and MPFR projects,
CGAL provides precompiled versions of GMP and MPFR, which can be downloaded
with the installer of CGAL-4.2-Setup.exe.

If GMP and MPFR are not installed in a standard place, you must set the
environment variables GMP_DIR and MPFR to point at the directories where
GMP and MPFR are installed, e.g.,

  GMP_DIR=D:/GMP/5_0_1

If GMP and MPFR are installed under the same directory, it is sufficient
to set only one of environment variables.

2.5.6. OpenGL
OpenGL (Open Graphics Library) provides an API for applications that produce
2D and 3D computer graphics. SGAL depends on OpenGL. There is no alternative.
That is, SGAL does not use Direct 3D (on Windows) or any other proprietary
API.

Typically, OpenGL is preinstalled on systems; if not, it can be downloaded
from http://www.opengl.org/. 

2.5.7. gl2ps
GL2PS is a C library providing vector output for any OpenGL application.
For more information see http://www.geuz.org/gl2ps/.

If GL2PS is not installed in a standard place, you must set the
environment variable GL2PS_DIR to point at the directory where GL2PS
is installed, e.g.,

  GL2PS_DIR=D:/GL2PS/1_3_8

2.5.8 Some X libraries (forgot the name)
This applies to Linux only. These libraries are not installed by default.
You will get an error message during link if they are not installed.

2.6. Building SGAL

2.6.1. Environment Variables

2.6.1.2. PLATFORM
The PLATFORM environment variable indicates the combination of operating
system and development environment. Theoretically there are four options
listed below. In practice only the first two are trully supported.

1. linux - Linux + GNU (g++, etc).
2. console - Windows + MSVC (cl, etc) console application
3. cygwin - Windows + GNU
4. windows - Windows + MSVC windows application

Note: In Visual Studio there are two kinds of *.exe binaries, a windows
application and a console application. A Windows Application is a Windows
program that typically has a GUI. Console applications, on the other
hand, are supposed to be run from the console. For developers, the main
difference is that a windows application's entry point is WinMain(),
whereas a console application's entry point is main().

Under Linux I set PLATFORM as follows:
  
  PLATFORM=linux
   
2.6.1.2. ROOT
The ROOT environment variable points at the root directory of the
directory structure where all files generated during the build are
installed into. You can have several roots and switch between them.

Under Linux I set ROOT as follows:

  ROOT=$HOME//roots/$PLATFORM
  
Using different roots for different platforms allows using a file
system that is shared between different platforms (for example, if the
Windows root and the Linux root end up on the same disk.)

2.6.1.3. USE_<some-feature>
Recall that the environment variable CGAL_DIR points at the directory
where CGAL is installed. The version of CGAL installed may combine
several features that are possibly developed in different branches.

Let FTR ne an acronym of a feature. Set the environment variable
USE_FTR to enable the feature. If the feature requires a corresponding
CGAL feature, you must also install an appropriate version of CGAL that
supports the corresponding CGAL feature.
   
   USE_NGM=1  # Nef Gaussian Map (CGAL)
   USE_LTS=1  # Lines Through Segments
   USE_AOS=1  # Arrangement On Surface
   USE_VOS=1  # Voronoi On Sphere
   USE_CGAL=1 # CGAL
   USE_SGM=1  # Spherical Gaussian Map
   USE_NEF=1  # Nef
   USE_CGM=1  # Cubical Gaussian Map
   USE_GMP=1  # Link with GMP & MPFR

   USE_LTS=1 requires origin/Lines_through_segments-pmoeller
   USE_CGM=1 requires origin/Cubical_gaussian_map-efif
   USE_VOS=1 requires remotes/origin/Envelope_voronoi_2-ophirset
   USE_AOS=1 requires origin/Aos_2-remove_opt-efif

2.6.2. Script

cd make && make install && cd -
cd src/libs && make install && cd -
cd src/cmds && make install && cd -

2.7. Testing

----------------------------------

# Introduction
The current goal of EGO project is to provide the ability to transform 3D
models into lego.

# Algorithm
First we decide that we implement a way to convert to model into a grid model...
