# EGO project

## Introduction

The current goal of EGO project is to provide the ability to convert 3D models
into lego.

## Algorithm

First we decide that we implement a way to convert to model into a grid model...

## Player

The visualization is performed by a graphic player written by...
It provide the ability to read VRML files.

## Compiling - current instructions

You need to compile:
1) Libs: gl2ps, SGAL, SCGAL, and SEGO
2) Cmds: player

Set the following env. variables:

   PLATFORM=linux # Options are linux, windows, cygwin, console

   # I set ROOT to ..../linux_ego (and not just .../ego) just in case the windows root and the linux root end up in the same disk:
   ROOT=/home/[user]/roots/linux_ego

   # This points to the directory where CGAL is installed. It combines several branches according to the env. var. below.
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
