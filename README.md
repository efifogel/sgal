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
using MSVC and Unix-like systems using gcc. It is not supported on Mac OS.

Ideally, setting up SGAL amounts to:

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
ssh key for every machine you would like to access the repository via the
cclone command below. Once you have an account, an admin of the
repository, e.g., Efi (efifogel@gmail.com), can grant you the appropriate
access permissions. Currently, the repository is closed to the public.

  git clone git@bitbucket.org:efogel/ego.git

If you are on Windows you may not download and run SGAL-1.0-Setup.exe, as
there such does not exist yet.

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
C++ compiler. SGAL 1.0 is supported for the following compilers/operating
systems:

Compiler                      | Operating System
------------------------------|-------------------------------------
Gnu g++ 4.2 or later          | Linux
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
specialized libraries and software for other tasks.

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

As on Windows there is no canonical directory for where to find Boost,
you must define the environment variable BOOST_DIR and set
it to where you have installed Boost, e.g., C:\boost\boost_1_41_0; see
Section TBD.

2.5.2. bison and flex

2.5.2.1. bison

2.5.2.2. flex

2.5.3. Magick++ (ImageMagick)

2.5.4. GMP and MPFR
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

2.5.5. OpenGL
OpenGL (Open Graphics Library) provides an API for applications that produce
2D and 3D computer graphics. SGAL depends on OpenGL. There is no alternative.
That is, SGAL does not use Direct 3D (on Windows) or any other proprietary
API.

Typically, OpenGL is preinstalled on systems; if not, it can be downloaded
from http://www.opengl.org/. 

2.6. Building SGAL

2.6.1. Environment Variables

   PLATFORM=linux # Options are linux, windows, cygwin, console

   # I set ROOT to $HOME/roots/linux_ego (and not just .../ego) just in case
   the windows root and the linux root end up in the same disk:
   ROOT=/home/[user]/roots/linux_ego

   # This points to the directory where CGAL is installed. It combines
   several branches according to the env. var. below.
   
   CGALROOT=/usr/local/CGAL/CGAL-combined

   USE_NGM=1  # Nef Gaussian Map (CGAL)
   USE_LTS=0  # Lines Through Segments
   USE_CORE=1 # link with the CORE library
   USE_AOS=1  # Arrangement On Surface
   USE_VOS=0  # Voronoi On Sphere
   USE_CGAL=1 # CGAL
   USE_SGM=1  # Spherical Gaussian Map
   USE_NEF=1  # Nef
   USE_CGM=0  # Cubical Gaussian Map
   USE_GMP=1  # Link with GMP & MPFR

   USE_LTS=1 requires origin/Lines_through_segments-pmoeller
   USE_CGM=1 requires origin/Cubical_gaussian_map-efif
   USE_VOS=1 requires remotes/origin/Envelope_voronoi_2-ophirset
   USE_AOS=1 requires origin/Aos_2-remove_opt-efif

   USE_EGO=1  #  Ego

2.6.2. Script

cd make && make install && cd -
cd src/libs && make install && cd -
cd src/cmds && make install && cd -

2.7. Testing


----------------------------------

You need to compile:
1) Tools: bison - in branch
2) Libs: SGAL, SCGAL, and SEGO
3) Cmds: player

1) Bison - first install automake, autopoint, & texinfo. Then, run ./bootstrap from the bison directory. Then the regular ./configure and make, make install.

Set the following env. variables:

It is suggested that before you build (your) ego, build a fresh version of CGAL that combines the branches above and based on 'master'. (This is what I did.)

  git checkout -b combined origin/master
  git merge origin/Aos_2-remove_opt-efif
  git merge origin/Lines_through_segments-pmoeller
  git merge origin/Envelope_voronoi_2-ophirset
  git merge origin/Cubical_gaussian_map-efif

Dependencies: Boost, CGAL, Gmp, Mpfr, Imagemagic, Qt4, and some X libraries that are not installed by default (forgot the name, but you'll get an error message during link if they are not installed).

### List of Ubuntu 12.04 packages I needed to install
* libmagick++-dev
* libgl2ps-dev
* libxxf86vm-dev

## List of selected packages in cygwin (PLATFORM=windows)
* ImageMagick
* ImageMagick devel
* pkg-config
* g++
* flex
* bison
Don't forget to set the BOOST_INC_DIR to the path of the boost include.
Don't forget to set the MAGICK_DIR
Don't forget to set the GL2PS_DIR

# To obtain the latest version of bison from the its git repository
# (using anonymus checkout) issue:
git clone git://git.savannah.gnu.org/bison.git

# To update all submodules, issue:
git submodule -q foreach git pull -q origin master

# To build bison from scratch, install via cygwin:
# gcc, autopoint,  make, automake, autoconf, texinfo, and rsync,
# Then, issue:
./bootstrap
./configure
make # make install

# Introduction
The current goal of EGO project is to provide the ability to transform 3D
models into lego.

# Algorithm
First we decide that we implement a way to convert to model into a grid model...
