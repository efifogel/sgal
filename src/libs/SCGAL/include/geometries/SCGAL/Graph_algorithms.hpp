// Copyright (c) 2008 Israel.
// All rights reserved.
//
// This file is part of SGAL; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the
// License. See the file LICENSE.LGPL distributed with SGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the
// software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Author(s): Tzvika Geft <zvigreg@mail.tau.ac.il>

#ifndef SCGAL_GRAPH_ALGORITHMS_HPP
#define SCGAL_GRAPH_ALGORITHMS_HPP

#include <vector>
#include <algorithm>
#include <cmath>

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_matrix.hpp>

SGAL_BEGIN_NAMESPACE

const int leafsize = 16; // Value below which the naive algorithms are used

typedef boost::adjacency_matrix<boost::directedS> Graph;

inline void ikjalgorithm(std::vector< std::vector<int> >& A,
                         std::vector< std::vector<int> >& B,
                         std::vector< std::vector<int> >& C, int n)
{
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      C[i][j] = 0; // In case it wasn't 0
      for (int k = 0; k < n; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

inline unsigned int nextPowerOfTwo(int n) {
  return pow(2, int(ceil(log2(n))));
}

inline void sum(std::vector< std::vector<int> >& A,
                std::vector< std::vector<int> >& B,
                std::vector< std::vector<int> >& C, int size)
{
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      C[i][j] = A[i][j] + B[i][j];
    }
  }
}

inline void subtract(std::vector< std::vector<int> >& A,
                     std::vector< std::vector<int> >& B,
                     std::vector< std::vector<int> >& C, int size)
{
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      C[i][j] = A[i][j] - B[i][j];
    }
  }
}

inline void or_inplace(std::vector< std::vector<int> >& A,
                       std::vector< std::vector<int> >& B,
                       int size)
{
    int i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            A[i][j] = (A[i][j] > 0) || (B[i][j] > 0);
        }
    }
}

inline void mult(std::vector< std::vector<int> >& A,
                 std::vector< std::vector<int> >& B,
                 std::vector< std::vector<int> >& C, int size,
                 bool naive)
{
  if (naive) {
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        C[i][j] = 0; // In case it wasn't 0
        for (int k = 0; k < size; k++) {
          if (A[i][k] && B[k][j]){
            C[i][j] = 1;
            break;
          }
        }
      }
    }
    return;
  }

  if (size <= leafsize) {
    ikjalgorithm(A, B, C, size);
    return;
  }

  else {
    int newSize = size/2;
    std::vector<int> inner (newSize);
    std::vector< std::vector<int> >
      a11(newSize,inner), a12(newSize,inner),
      a21(newSize,inner), a22(newSize,inner),
      b11(newSize,inner), b12(newSize,inner),
      b21(newSize,inner), b22(newSize,inner),
      c11(newSize,inner), c12(newSize,inner),
      c21(newSize,inner), c22(newSize,inner),
      p1(newSize,inner), p2(newSize,inner),
      p3(newSize,inner), p4(newSize,inner),
      p5(newSize,inner), p6(newSize,inner), p7(newSize,inner),
      aResult(newSize,inner), bResult(newSize,inner);

    int i, j;

    //dividing the matrices in 4 sub-matrices:
    for (i = 0; i < newSize; i++) {
      for (j = 0; j < newSize; j++) {
        a11[i][j] = A[i][j];
        a12[i][j] = A[i][j + newSize];
        a21[i][j] = A[i + newSize][j];
        a22[i][j] = A[i + newSize][j + newSize];

        b11[i][j] = B[i][j];
        b12[i][j] = B[i][j + newSize];
        b21[i][j] = B[i + newSize][j];
        b22[i][j] = B[i + newSize][j + newSize];
      }
    }

    // Calculating p1 to p7:

    sum(a11, a22, aResult, newSize); // a11 + a22
    sum(b11, b22, bResult, newSize); // b11 + b22
    mult(aResult, bResult, p1, newSize, false); // p1 = (a11+a22) * (b11+b22)

    sum(a21, a22, aResult, newSize); // a21 + a22
    mult(aResult, b11, p2, newSize, false); // p2 = (a21+a22) * (b11)

    subtract(b12, b22, bResult, newSize); // b12 - b22
    mult(a11, bResult, p3, newSize, false); // p3 = (a11) * (b12 - b22)

    subtract(b21, b11, bResult, newSize); // b21 - b11
    mult(a22, bResult, p4, newSize, false); // p4 = (a22) * (b21 - b11)

    sum(a11, a12, aResult, newSize); // a11 + a12
    mult(aResult, b22, p5, newSize, false); // p5 = (a11+a12) * (b22)

    subtract(a21, a11, aResult, newSize); // a21 - a11
    sum(b11, b12, bResult, newSize); // b11 + b12
    mult(aResult, bResult, p6, newSize, false); // p6 = (a21-a11) * (b11+b12)

    subtract(a12, a22, aResult, newSize); // a12 - a22
    sum(b21, b22, bResult, newSize); // b21 + b22
    mult(aResult, bResult, p7, newSize, false); // p7 = (a12-a22) * (b21+b22)

    // calculating c21, c21, c11 e c22:

    sum(p3, p5, c12, newSize); // c12 = p3 + p5
    sum(p2, p4, c21, newSize); // c21 = p2 + p4

    sum(p1, p4, aResult, newSize); // p1 + p4
    sum(aResult, p7, bResult, newSize); // p1 + p4 + p7
    subtract(bResult, p5, c11, newSize); // c11 = p1 + p4 - p5 + p7

    sum(p1, p3, aResult, newSize); // p1 + p3
    sum(aResult, p6, bResult, newSize); // p1 + p3 + p6
    subtract(bResult, p2, c22, newSize); // c22 = p1 + p3 - p2 + p6

    // Grouping the results obtained in a single matrix:
    for (i = 0; i < newSize ; i++) {
      for (j = 0 ; j < newSize ; j++) {
        C[i][j] = c11[i][j];
        C[i][j + newSize] = c12[i][j];
        C[i + newSize][j] = c21[i][j];
        C[i + newSize][j + newSize] = c22[i][j];
      }
    }
  }
}

inline void transitive_closureR(std::vector< std::vector<int> > &M, int size,
                                bool naive)
{
  if (size <= leafsize) {
    // Repeated squaring (with boolean multiplication)
    for (int c=0; c<int(ceil(log2(size))); c++){
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          for (int k = 0; k < size; k++) {
            if (M[i][k] && M[k][j]){
              M[i][j] = 1;
              break;
            }
          }
        }
      }
    }
  }


  else {
    int newSize = size/2;
    std::vector<int> inner (newSize);
    std::vector< std::vector<int> >
      A(newSize,inner), B(newSize,inner), C(newSize,inner), D(newSize,inner),
      aux(newSize,inner);

    int i, j;

    // Dividing the matrices in 4 sub-matrices:
    for (i = 0; i < newSize; i++) {
      for (j = 0; j < newSize; j++) {
        A[i][j] = M[i][j];
        B[i][j] = M[i][j + newSize];
        C[i][j] = M[i + newSize][j];
        D[i][j] = M[i + newSize][j + newSize];
      }
    }
    // The RHS in the comments always refers to the original A,B,C,D
    transitive_closureR(D, newSize, naive); // D = D*
    mult(B, D, aux, newSize, naive); // aux = BD*
    B = aux; // B = BD*

    mult(B, C, aux, newSize, naive); // aux = BD*C
    or_inplace(A, aux, newSize); // A = (A || BD*C)
    transitive_closureR(A, newSize, naive); // E := A = (A || BD*C)*

    mult(D, C, aux, newSize, naive); // aux = D*C
    mult(aux, A, C, newSize, naive); // G := C = D*CE

    mult(C, B, aux, newSize, naive); // aux = GBD*
    or_inplace(D, aux, newSize); // D = (D* || GBD*)

    mult(A, B, aux, newSize, naive); // aux = EBD*
    B = aux; // B = EBD*

    // Grouping the results obtained in a single matrix:
    for (i = 0; i < newSize ; i++) {
      for (j = 0 ; j < newSize ; j++) {
        M[i][j] = A[i][j];
        M[i][j + newSize] = B[i][j];
        M[i + newSize][j] = C[i][j];
        M[i + newSize][j + newSize] = D[i][j];
      }
    }
  }
}

inline void transitive_closure(Graph& g, bool naive = false)
{
  unsigned int orig_n = boost::num_vertices(g);
  if (orig_n <= 1) return;

  unsigned int n = (orig_n > leafsize) ? nextPowerOfTwo(orig_n) : orig_n; // Only pad if there will be recursive calls
  std::vector<int> inner(n);
  std::vector< std::vector<int> > MPrep(n, inner);

  for(unsigned int i=0; i<orig_n; i++) {
    for (unsigned int j=0; j<orig_n; j++) {
      if (i == j) {
        MPrep[i][j] = 1;
        continue;
      }

      if (boost::edge(i, j, g).second) {
        MPrep[i][j] = 1;
      }
    }
  }

  transitive_closureR(MPrep, n, naive);

  for (unsigned int i=0; i<orig_n; i++) {
    for (unsigned int j=0; j<orig_n; j++) {
      if (MPrep[i][j] > 0) {
        boost::add_edge(i, j, g);
      }
    }
  }
}

SGAL_END_NAMESPACE

#endif
