//============================================================================
//
// Copyright (c) 2001-2006 Max-Planck-Institut Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of EXACUS (http://www.mpi-inf.mpg.de/projects/EXACUS/);
// you may redistribute it under the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with EXACUS.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
//----------------------------------------------------------------------------
//
// Library       : QdX
// File          : demos/xqvisualization/Quadric.C
// QdX_release   : $Name:  $
// Revision      : $Revision: 5272 $
// Revision_date : $Date: 2007-10-22 15:08:57 +0200 (Mon, 22 Oct 2007) $
//
// Author(s)     : Roman Tulchyner <rtulchy@mpi-inf.mpg.de>
//
//============================================================================

#include <QdX/basic.h>

#include "Quadric.hpp"
#include "Vector3d.hpp"

#include <vector>

#include <cmath>
#include <math.h>

#include <GL/gl.h>

#include <iostream>

#include <QdX/Quadric_3.h>
#include <NiX/Arithmetic_traits.h>

Quadric::Quadric(const Quadric_3 & quadric_3)
{
  this->quadric_3 = quadric_3;
  //hole matrix darstellung der quadric aus ganzen zahlen
  Matrix_int matrix_int_a_4x4 = quadric_3.matrix();

  //die 4x4 matrix A aus reelen zahlen:
  Matrix_real matrix_a_4x4( 4, 4 );
  for (unsigned int i=0; i<4; i++){
    for (unsigned int j=0; j<4; j++) {
      matrix_a_4x4[i][j] =
        static_cast< Field_with_sqrt >(matrix_int_a_4x4[i][j]);
    }
  }

  //die obere 3x3 matrix A aus reelen zahlen:
  Matrix_real matrix_a_3x3( 3, 3 );
  for (unsigned int i=0; i<3; i++) {
    for (unsigned int j=0; j<3; j++) {
      matrix_a_3x3[i][j] = matrix_a_4x4[i][j];
    }
  }

  //die obere 3x3 matrix A aus rationalen zahlen:
  Matrix_rat matrix_rat_a_3x3( 3, 3 );
  for (unsigned int i=0; i<3; i++) {
    for (unsigned int j=0; j<3; j++) {
      matrix_rat_a_3x3[i][j] = Compose()( matrix_int_a_4x4[i][j], 1);
    }
  }

  //3x3 matrix U, so dass für 3x3 matrix A gilt: UtAU - Diagonalmatrix
  Matrix_rat matrix_rat_u_3x3( 3, 3 );
  matrix_rat_u_3x3 = gramm_schmidt_diagonalize(matrix_rat_a_3x3);
  //Gram-Schmidt wird noch auf rationalen Zahlen durchgefuehrt, was den 
  //Ereignisbaum der reelen Zahlen, die später benutzt werden, klein haelt. 
  Matrix_real matrix_u_3x3( 3, 3 );
  for (unsigned int i=0; i<3; i++) {
    for (unsigned int j=0; j<3; j++) {
      matrix_u_3x3[i][j] = matrix_rat_u_3x3[i][j];
    }
  }
  //Diagonalmatrix UtAU
  Matrix_real matrix_d_3x3 = 
          matrix_u_3x3.transpose() * matrix_a_3x3 * matrix_u_3x3; 
  //3x3 matrix S, so dass für 3x3 matrix A und U gilt: StUtAUS - Orthonormal
  Matrix_real matrix_s_3x3 = diagonal_matrix_normalize( matrix_d_3x3 ); 
  //3x3 matrix U, so dass für 3x3 matrix A UtAU - Orthonormal
  matrix_u_3x3 = matrix_u_3x3 * matrix_s_3x3; 
  //Orthonormale Matrix
  Matrix_real matrix_o_3x3 = 
    matrix_u_3x3.transpose()*matrix_a_3x3*matrix_u_3x3; 

  //FALLS ALLE DIAGONALWERTE "NEGIERT" => A NEGIERT
  if (matrix_o_3x3[0][0] + matrix_o_3x3[1][1] + matrix_o_3x3[2][2] <= 0) {
    Matrix_real matrix_minus_identity_4x4( 4, 4, Field_with_sqrt(0) );
    matrix_minus_identity_4x4[0][0] = Field_with_sqrt(-1);
    matrix_minus_identity_4x4[1][1] = Field_with_sqrt(-1);
    matrix_minus_identity_4x4[2][2] = Field_with_sqrt(-1);
    matrix_minus_identity_4x4[3][3] = Field_with_sqrt(-1);
    matrix_a_4x4 = matrix_a_4x4 * matrix_minus_identity_4x4;
  
    //die obere 3x3 matrix A aus reelen zahlen:
    for (unsigned int i=0; i<3; i++) {
      for (unsigned int j=0; j<3; j++) {
        matrix_a_3x3[i][j] = matrix_a_4x4[i][j];
      }
    }
  }

  //stelle sicher dass U die orthonormale matrix erzeugt mit eigenwerten in 
  //richtiger ordnung
  matrix_u_3x3 = set_proper_order( matrix_u_3x3, matrix_a_3x3 );

  matrix_o_3x3 = matrix_u_3x3.transpose() * matrix_a_3x3 * matrix_u_3x3;

  //erzeuge 4x4 matrix U mit ersten 3x3 einträgen aus 3x3 matrix U und 
  //eintrag bei 4. zeile und 4. spalte =1
  Matrix_real matrix_u_4x4( 4, 4, Field_with_sqrt(0) );
  for (unsigned int i=0; i<3; i++){
    for (unsigned int j=0; j<3; j++){
      matrix_u_4x4[i][j] = matrix_u_3x3[i][j];
    }
  }
  matrix_u_4x4[3][3] = Field_with_sqrt(1);
  
  //matrix UtAU
  Matrix_real matrix_uau =
    matrix_u_4x4.transpose() * matrix_a_4x4 * matrix_u_4x4;

  Field_with_sqrt x(matrix_uau[0][3]);
  Field_with_sqrt y(matrix_uau[1][3]);
  Field_with_sqrt z(matrix_uau[2][3]);
  Field_with_sqrt w(matrix_uau[3][3]);

  std::pair<int, int> inertia;
  inertia = quadric_3.get_inertia();

  std::pair<int, int> upper_left_inertia;
  upper_left_inertia = quadric_3.get_upper_left_inertia();

  Field_with_sqrt t11(1), t22(1);
  double angle = (2.0 * pi);// * (3/3.0);

  //klassifizierung der quadricken nach dem Silvester's Law:
  if ( inertia == std::pair<int,int> (4,0) ){
    quadric_type = "imaginary ellipsoid";
    uniform_quadric = new Not_drawable_quadric();
  } else if ( inertia == std::pair<int,int> (3,1) ) {
    if ( upper_left_inertia == std::pair<int,int> (3,0) ) {
      quadric_type =  "ellipsoid";
      uniform_quadric = new Sphere( -1.0, 1.0, 60, 60, angle );
      w = x*x + y*y + z*z - w;
      x = -x / NiX::sqrt( NiX::abs(w) );
      y = -y / NiX::sqrt( NiX::abs(w) );
      z = -z / NiX::sqrt( NiX::abs(w) );
      w = Field_with_sqrt(1) / NiX::sqrt( NiX::abs(w) ) ;
    } else if ( upper_left_inertia == std::pair<int,int> (2,1) ) {
      quadric_type = "hyperboloid of two sheets";
      uniform_quadric =
        new Uniform_hyperboloid_of_two_sheets(-10.0, 10.0, 60, 60, angle );
      w = -x*x - y*y + z*z + w;
      x = -x / NiX::sqrt( NiX::abs(w) );
      y = -y / NiX::sqrt( NiX::abs(w) );
      z =  z / NiX::sqrt( NiX::abs(w) );
      w = Field_with_sqrt(1) / NiX::sqrt( NiX::abs(w) ) ;
    } else if ( upper_left_inertia == std::pair<int,int> (2,0) ){
      quadric_type = "elliptic paraboloid";
      uniform_quadric =
        new Uniform_elliptic_paraboloid(0.0, 10.0, 60, 60, angle );
      w = -x*x - y*y + w;
      x = -x;
      y = -y;
      t22 = Field_with_sqrt(1)/ z;
      z = Field_with_sqrt(-0.5) * ( w / z );
      w = Field_with_sqrt(1);
    }
  } else if ( inertia == std::pair<int,int> (2,2) ){
    if ( upper_left_inertia == std::pair<int,int> (2,1) ){
      quadric_type = "hyperboloid of one sheet";
      uniform_quadric =
        new Uniform_hyperboloid_of_one_sheet(-10.0, 10.0, 60, 60, angle );
      w = x*x + y*y - z*z - w;
      x = Field_with_sqrt(-1) * ( x / NiX::sqrt( NiX::abs(w) ) );
      y = Field_with_sqrt(-1) * ( y / NiX::sqrt( NiX::abs(w) ) );
      z =  z / NiX::sqrt( NiX::abs(w) );
      w = Field_with_sqrt(1) / NiX::sqrt( NiX::abs(w) ) ;
    } else if ( upper_left_inertia == std::pair<int,int> (1,1) ){
      quadric_type = "hyperbolic paraboloid";
      uniform_quadric =
        new Uniform_hyperbolic_paraboloid(-10.0, 10.0, -10.0, 10.0, 60, 60);
      w = -x*x + y*y + w;
      x = -x;
      //y = y;
      t22 = Field_with_sqrt(1) / z; //z ist für hyperbolic paraboloid != 0
      z = Field_with_sqrt(-0.5) * (w / z);
      w = Field_with_sqrt(1);
    }
  } else if ( inertia == std::pair<int,int> (3,0) ) {
    if ( upper_left_inertia == std::pair<int,int> (3,0) ) {
      quadric_type = "singular point";
      uniform_quadric = new Point();
      x = -x;
      y = -y;
      z = -z;
      w = Field_with_sqrt(1);
    } else if ( upper_left_inertia == std::pair<int,int> (2,0) ) {
      quadric_type = "imaginary elliptic cylinder";
      uniform_quadric = new Not_drawable_quadric();
    }
  } else if ( inertia == std::pair<int,int> (2,1) ) {
    if ( upper_left_inertia == std::pair<int,int> (2,1) ) {
      quadric_type =  "cone";
      uniform_quadric = new Uniform_cone( -10.0, 10.0, 60, 60, angle );
      w =  -x*x + - y*y + z*z + w;
      x = -x;
      y = -y;
      //z = z;
      w = Field_with_sqrt(1);
    } else if ( upper_left_inertia == std::pair<int,int> (2,0) ){
      quadric_type = "elliptic cylinder";
      uniform_quadric = new Cylinder( -10.0, 10.0, 60, 60, angle );
      w = x*x + y*y - w;
      x = -x / NiX::sqrt( NiX::abs(w) );
      y = -y / NiX::sqrt( NiX::abs(w) );
      //z = z; z = Field_with_sqrt(1); //spielt überhaupt keine rolle
      w = Field_with_sqrt(1) / NiX::sqrt( NiX::abs(w) );
      t22 = w;//wichtig um bez. der z-Achse auf standart grösse zu bringen
    } else if ( upper_left_inertia == std::pair<int,int> (1,1) ){
      quadric_type = "hyperbolic cylinder";
      uniform_quadric =
        new Uniform_hyperbolic_cylinder(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0, 60, 60 );
      w = x*x - y*y - w;
      x = -x / NiX::sqrt( NiX::abs(w) );
      y = y / NiX::sqrt( NiX::abs(w) );
      //z = Field_with_sqrt(0); //spielt überhaupt keine rolle
      w = Field_with_sqrt(1) / NiX::sqrt( NiX::abs(w) );
      t22 = w;//wichtig um bez. der z-Achse auf standart grösse zu bringen
    } else if ( upper_left_inertia == std::pair<int,int> (1,0) ){
      quadric_type = "parabolic cylinder";
      uniform_quadric =
        new Uniform_parabolic_cylinder(-10.0, 10.0, -10.0, 10.0, 60, 60 );
      w = - x*x + w;
      x = -x;
      t11 = Field_with_sqrt(1) / y;
      y = Field_with_sqrt(-0.5) * ( w / y );
      //z = Field_with_sqrt(0); // spielt überhaupt keine rolle
      w = Field_with_sqrt(1);
    }
  } else if ( inertia == std::pair<int,int> (2,0) ){
    if ( upper_left_inertia == std::pair<int,int> (2,0) ){
      quadric_type = "line";
      uniform_quadric = new Line(-10.0,10.0);
      x = -x;
      y = -y;
      //z spielt keine rolle
      w = Field_with_sqrt(1);
    } else if ( upper_left_inertia == std::pair<int,int> (1,0) ){
      quadric_type = "imaginary parallel planes";
      uniform_quadric = new Not_drawable_quadric();
    }
  } else if ( inertia == std::pair<int,int> (1,1) ){
    if ( upper_left_inertia == std::pair<int,int> (1,1) ){
      quadric_type = "intersecting planes";
      uniform_quadric =
        new Uniform_intersecting_planes(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0, 60, 60, 60 );
      x = -x;
      //y = y;
      //z = z;
      w = Field_with_sqrt(1);
    } else if ( upper_left_inertia == std::pair<int,int> (1,0) ){
      quadric_type = "parallel planes";
      uniform_quadric =
        new Uniform_parallel_planes(-10.0, 10.0, -10.0, 10.0, 60, 60 );
      w = x*x - w;
      x = -x / NiX::sqrt( NiX::abs(w) );
      w = Field_with_sqrt(1) / NiX::sqrt( NiX::abs(w) ) ;
      t11 = w;//wichtig um bez. der y-Achse auf standart grösse zu bringen
      t22 = w;//wichtig um bez. der z-Achse auf standart grösse zu bringen
    } else if ( upper_left_inertia == std::pair<int,int> (0,0) ){
      quadric_type = "simple plane";
      uniform_quadric =
        new Uniform_coincident_planes(-10.0, 10.0, -10.0, 10.0, 60, 60 );
      w = Field_with_sqrt(1);
      x = -x;
    }
  } else if ( inertia == std::pair<int,int> (1,0) ){
    if ( upper_left_inertia == std::pair<int,int> (1,0) ){
      quadric_type = "double plane";
      uniform_quadric =
        new Uniform_coincident_planes(-10.0, 10.0, -10.0, 10.0, 60, 60 );
      w = Field_with_sqrt(1);
      x = -x;
    } else
    if ( upper_left_inertia == std::pair<int,int> (0,0) ){
      quadric_type = "empty space";
      uniform_quadric = new Not_drawable_quadric();
    }
  };
    
  //std::cout << "Quadric type: " << quadric_type << std::endl;

  //Transformation matrix T , so dass TtUtAUT = Orthonormal und eigenwerte 
  //in der richt. ordnung
  Matrix_real t( 4, 4, Field_with_sqrt(0) );

  t[0][0] = Field_with_sqrt(1);
  t[0][3] = x;

  t[1][1] = t11;
  t[1][3] = y;

  t[2][2] = t22;
  t[2][3] = z;
  t[3][3] = w;

  //setze eine 4x4 transformations matrix U, so dass UtAU = Orthonormal und 
  //eigenw. in der für einheitsquadricken richtigen ordnung
  matrix_u_4x4 = matrix_u_4x4 * t;

  //Orthonormale Matrix = Einheitsquadric matrix (zu Debug Zwecken)
  Matrix_real
    matrix_o_4x4(matrix_u_4x4.transpose() * matrix_a_4x4 * matrix_u_4x4 );

  set_transformation_Matrix( matrix_u_4x4 );

  Field_with_sqrt det_u_4x4 = LA_field_with_sqrt::Det()( matrix_u_4x4 );
  //ist die det(transformations matrix)<0, so tritt infolge von transformation
  //(um die einheitsquadric zur gegebenen quadric zu bringen) eine Spiegelung
  //ein. Damit diese aber dennoch richtig beleuchtet wird, müssen deren 
  //normalen gespiegelt werden.
  if ( det_u_4x4 < 0 ) 
    uniform_quadric->swap_vertex_normals();
//std::cout <<  = matrix_u_4x4 << std::endl;
}

Quadric::~Quadric()
{
  delete uniform_quadric;
  if ( display_list_option.first && glIsList(display_list_option.second) ){
    glDeleteLists(display_list_option.second, 1);
  }
}

void Quadric::draw()
{
/*
  glPushMatrix();
  glMultMatrixd(m);
  uniform_quadric->draw();
  glPopMatrix();*/

  if ( !display_list_option.first ){
    //std::cout << "DrawQ ..." << std::endl;
    display_list_option.second = glGenLists(1);
    glNewList(display_list_option.second, GL_COMPILE_AND_EXECUTE);
      glPushMatrix();
      glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_NORMALIZE);
        glMultMatrixd(m);
        uniform_quadric->draw();
      glPopAttrib();
      glPopMatrix();
    glEndList();
    display_list_option.first = true;
  }else{
    glCallList(display_list_option.second);
    //std::cout << "DrawQ by DL ..." << std::endl;
  }
}

Quadric_3 Quadric::get_quadric_3() const{
  return this->quadric_3;
}

void Quadric::set_transformation_Matrix(Matrix_real& matrix_u_4x4)
{
  //bestimme die transformations Matrix als Matrix aus double zahlen:
//  std::cout << "U aus doubles" << std::endl;
  double u[4][4];
  for (unsigned int i=0; i<4; i++) {
    for (unsigned int j=0; j<4; j++) {
      u[i][j] = NiX::to_double( matrix_u_4x4[i][j] );
//      std::cout <<  = u[i][j] << '\t';
    }  
//    std::cout <<  = std::endl;
  }
//  std::cout <<  = std::endl;

  //setze die transformations Matrix m in der form wie das mit 
  //glMultMatrixd() benutzt wird:
  m[0] = u[0][0]; m[1] = u[1][0]; m[2] = u[2][0]; m[3] = u[3][0];
  m[4] = u[0][1]; m[5] = u[1][1]; m[6] = u[2][1]; m[7] = u[3][1]; 
  m[8] = u[0][2]; m[9] = u[1][2]; m[10] = u[2][2]; m[11] = u[3][2];
  m[12] = u[0][3]; m[13] = u[1][3]; m[14] = u[2][3]; m[15] = u[3][3];
}

Matrix_real Quadric::set_proper_order( Matrix_real& transformation_matrix_3x3, 
                                       Matrix_real& matrix_real_a_3x3 )
{
  //stelle durch geeignete swaps sicher, dass U3x3 eigenwerte in der richt. 
  //ordnung enthält
  Matrix_real swap_matrix_raw0_raw1( 3, 3, Field_with_sqrt(0) );
  swap_matrix_raw0_raw1[0][1] = Field_with_sqrt(1);
  swap_matrix_raw0_raw1[1][0] = Field_with_sqrt(1);
  swap_matrix_raw0_raw1[2][2] = Field_with_sqrt(1);

  Matrix_real swap_matrix_raw0_raw2( 3, 3, Field_with_sqrt(0) );
  swap_matrix_raw0_raw2[0][2] = Field_with_sqrt(1);
  swap_matrix_raw0_raw2[1][1] = Field_with_sqrt(1);
  swap_matrix_raw0_raw2[2][0] = Field_with_sqrt(1);

  Matrix_real swap_matrix_raw1_raw2( 3, 3, Field_with_sqrt(0) );
  swap_matrix_raw1_raw2[0][0] = Field_with_sqrt(1);
  swap_matrix_raw1_raw2[1][2] = Field_with_sqrt(1);
  swap_matrix_raw1_raw2[2][1] = Field_with_sqrt(1);

  Matrix_real matrix_orthonormal_3x3( transformation_matrix_3x3.transpose() *
                    matrix_real_a_3x3 * 
                    transformation_matrix_3x3 );
  Matrix_real matrix_result( transformation_matrix_3x3 );

  //mit folgendem code wird sichergestellt, dass eigenwert 1 immer über dem 
  //-1 oder 0 steht ist der dritte eigenwert eine 1 und der erste nicht, so 
  //werden diese "swaped" indem die transformation matrix mit der 
  //entsprechenden "swap matrix" multipliziert wird
  if ( matrix_orthonormal_3x3[2][2] == Field_with_sqrt(1) &&
     matrix_orthonormal_3x3[0][0] != Field_with_sqrt(1) ){
    matrix_result = matrix_result * swap_matrix_raw0_raw2;
    matrix_orthonormal_3x3 = matrix_result.transpose() * 
                 matrix_real_a_3x3 * matrix_result;
  }
  //ist der dritte eigenwert eine 1 und der zweite nicht, so werden diese 
  //"swaped" indem die transformation matrix mit der entsprechenden 
  //"swap matrix" multipliziert wird
  if ( matrix_orthonormal_3x3[2][2] == Field_with_sqrt(1) &&
     matrix_orthonormal_3x3[1][1] != Field_with_sqrt(1) ){
    matrix_result = matrix_result * swap_matrix_raw1_raw2;
    matrix_orthonormal_3x3 = matrix_result.transpose() * 
                 matrix_real_a_3x3 * matrix_result;
  }
  //ist der zweite eigenwert eine 1 und der erste nicht, so werden diese 
  //"swaped" indem die transformation matrix mit der entsprechenden 
  //"swap matrix" multipliziert wird
  if ( matrix_orthonormal_3x3[1][1] == Field_with_sqrt(1) &&
     matrix_orthonormal_3x3[0][0] != Field_with_sqrt(1) ){
    matrix_result = matrix_result * swap_matrix_raw0_raw1;
    matrix_orthonormal_3x3 = matrix_result.transpose() * 
                 matrix_real_a_3x3 * matrix_result;
  }

//mit folgendem code wird sichergestellt, dass eigenwert != 0 immer 
//über dem 0 steht (der erste eigenwert ist hier so oder so eine 1)
  //ist der dritte eigenwert nicht 0 und der zweite 0, so werden diese 
  //"swaped" indem die transformation matrix mit der entsprechenden 
  //"swap matrix" multipliziert wird
  if ( matrix_orthonormal_3x3[2][2] != Field_with_sqrt(0) &&
     matrix_orthonormal_3x3[1][1] == Field_with_sqrt(0) ){
    matrix_result = matrix_result * swap_matrix_raw1_raw2;
    matrix_orthonormal_3x3 = matrix_result.transpose() * 
                 matrix_real_a_3x3 * matrix_result;
  }

  return matrix_result;
}

Matrix_rat Quadric::gramm_schmidt_diagonalize(Matrix_rat& matrix_a)
{
  unsigned int dimension = matrix_a.row_dimension();

  std::vector< Vector_rat >  a; //eine liste der (am anfang) linear unabh. vektoren a
  for (unsigned int i=0; i < dimension; i++){
    Vector_rat dummy( dimension, Compose()(0,1) );
    dummy[i] = Compose()(1,1);
    a.push_back( dummy );
  }

//  Gramm-Schmidt verfahren;
  bool no_suitable_ak=true;

  while ( no_suitable_ak ) {
    for (unsigned int k=0; k<dimension; k++) {
      Rational akAak( (matrix_a.transpose()*a[k]) * a[k] );
      if ( akAak != Compose()(0,1) ){
        no_suitable_ak = false;
        for (unsigned int j=k+1; j<dimension; j++){
          Rational ajAak( matrix_a.transpose()*a[j]*a[k] );
          a[j] = a[j] - ajAak/ akAak * a[k] ;
        }
      } 
    }
    if ( no_suitable_ak ){ //suche passende linearkombination (ai + aj)
      for (unsigned int i=0; i<dimension; i++) {
        for (unsigned int j=i; j<dimension; j++) {
          Vector_rat ai_plus_aj( a[i] + a[j] );
          Rational ai_plus_ajAai_plus_aj((matrix_a.transpose()*
                          ai_plus_aj)*ai_plus_aj);
          if ( ai_plus_ajAai_plus_aj != Compose()(0,1) ){
            a[i] = ai_plus_aj; //eine passende vector paar gefunden,
            i = j = dimension; // bringe die schleifenpaar zuende.
          }   
        }  
      }
      no_suitable_ak = false; //es gibt keine passende vector paar 
                  //-wir sind fertig
    }
  }

  Matrix_rat matrix_u(dimension);
  for (unsigned int i=0; i<dimension; i++){
    matrix_u.column(i)=a[i];
  }

  return matrix_u;
}

Matrix_real Quadric::diagonal_matrix_normalize( Matrix_real& matrix_d)
{
  unsigned int dimension = matrix_d.row_dimension();
  //Diagonalmatrix S, so dass St*D*S = Orthonormal:
  Matrix_real matrix_s(dimension, dimension, Field_with_sqrt(0));
  for (unsigned int i=0; i<dimension; i++){
    if ( matrix_d[i][i] != Field_with_sqrt(0) )
      matrix_s[i][i] =
        NiX::sqrt((Field_with_sqrt)NiX::abs(Field_with_sqrt(1)/matrix_d[i][i]));
    else matrix_s[i][i] = Field_with_sqrt(1);
  }
  
  return matrix_s;
}

Quadric::Uniform_quadric::~Uniform_quadric(){}

void Quadric::Uniform_quadric::
set_surface_by_rotation(const std::vector<Vector3d>& kontur, int fragmentation, 
                        double angle, Axis axis )
{
//std::cout << "erstelle Rotaionskörper...\n";
  vertex = std::vector<Vector3d>(kontur.size()*(fragmentation + 1));  
  //so viele vertexes wird man brauchen

  for (unsigned int i=0; i<kontur.size(); i++) {vertex.at(i) = kontur.at(i);}  //die ersten vertexes kommen aus der kontur, die man rotieren will

  double dCos=cos(angle/fragmentation);
  double dSin=sin(angle/fragmentation);
    
  //mit diesem Block bestimmt man die restlichen vertexes durch rotation um 
  //die angegebene Achse - axis
  for (unsigned int i=kontur.size(); i<vertex.size(); i++){
    Vector3d dummy( vertex.at(i-kontur.size()) ) ;
    switch (axis){
    case Y:
      vertex.at(i) = dummy.rotate_y(dSin, dCos);
      break;
    case X:
      vertex.at(i) = dummy.rotate_x(dSin, dCos);
      break;
    case Z:
      vertex.at(i) = dummy.rotate_z(dSin, dCos);
      break;
    };
  };

  set_faces( kontur.size(), fragmentation);
  this->set_vertex_normals();
}

void Quadric::Uniform_quadric::set_faces( unsigned int kontur_size, 
                                          unsigned int fragmentation)
{
  face = std::vector< std::vector< int > > ( (kontur_size-1) * 
      fragmentation, std::vector< int >(4) );  
      //pro Flaeche sind die indizies der 4 vertexes zu merken
  int face_number = 0; //nummer der akt. fläche
  for ( unsigned int i=0; i<vertex.size()-kontur_size; i=i+kontur_size ){
    for( unsigned int j=0; j<kontur_size-1; j++) {  
      unsigned int nStartVertex=i+j;  //nummer des vertex oben rechts der betroffenen fläche (mit dem man anfängt)

//   COUNTERCLOCKWISE
      face[face_number][0]= ( nStartVertex + (int)kontur_size ) % vertex.size();
      face[face_number][1]= nStartVertex % vertex.size();
      face[face_number][2]= ( nStartVertex + 1 ) % vertex.size();
      face[face_number][3]= ( nStartVertex + kontur_size + 1 ) % vertex.size();

/* //CLOCKWISE
      face[face_number][0]= nStartVertex % vertex.size();
      face[face_number][1]= ( nStartVertex + (int)kontur_size ) % vertex.size();
      face[face_number][2]= ( nStartVertex + kontur_size + 1 ) % vertex.size();
      face[face_number][3]= ( nStartVertex + 1 ) % vertex.size();
*/
      face_number++;
    }
  }
}

void Quadric::Uniform_quadric::
set_surface_by_translation( const std::vector<Vector3d>& kontur,
                            int fragmentation, double distance, Axis axis )
{
  vertex = std::vector<Vector3d>(kontur.size()*(fragmentation + 1));  //so viele vertexes wird man brauchen

  for (unsigned int i=0; i<kontur.size(); i++) {vertex.at(i) = kontur.at(i);}  //die ersten vertexes kommen aus dem kontur, den man translieren will

  for (unsigned int i=kontur.size(); i<vertex.size(); i++){
    Vector3d dummy( vertex.at(i-kontur.size()) ) ;
    double d = distance/(double)fragmentation; //
    switch (axis){
    case Y:
      vertex.at(i) = dummy.translate( 0.0, d, 0.0);
      break;
    case X:
      vertex.at(i) = dummy.translate( d, 0.0, 0.0);
      break;
    case Z:
      vertex.at(i) = dummy.translate( 0.0, 0.0, d);
      break;
    };
  };

  set_faces( kontur.size(), fragmentation);
  set_vertex_normals();
}

void Quadric::Uniform_quadric::draw()
{  
//std::cout << "draw() zu Uniform_quadric...\n\n";
  Vector3d v;
  for ( unsigned int i=0; i<face.size(); i++ ){
    glBegin(GL_TRIANGLE_STRIP);
        v= vertex_normal[ face[i][0] ];
        glNormal3d( v.x(), v.y(), v.z());
        v= vertex[ face[i][0] ];
        glVertex3d(v.x(), v.y(), v.z());

        v= vertex_normal[ face[i][1] ];
        glNormal3d( v.x(), v.y(), v.z());
        v= vertex[ face[i][1] ];
        glVertex3d(v.x(), v.y(), v.z());

        v= vertex_normal[ face[i][3] ];
        glNormal3d( v.x(), v.y(), v.z());
        v= vertex[ face[i][3] ];
        glVertex3d(v.x(), v.y(), v.z());

        v= vertex_normal[ face[i][2] ];
        glNormal3d( v.x(), v.y(), v.z());
        v= vertex[ face[i][2] ];
        glVertex3d(v.x(), v.y(), v.z());
    glEnd();
  }
}

void Quadric::Uniform_quadric::set_vertex_normals(){}

void Quadric::Uniform_quadric::swap_vertex_normals()
{
  for (unsigned int i=0; i<vertex_normal.size(); i++) {
    vertex_normal[i][0] *= -1;
    vertex_normal[i][1] *= -1;
    vertex_normal[i][2] *= -1;
  }
}

//Not_drawable_quadric
Quadric::Not_drawable_quadric::Not_drawable_quadric(){}

void Quadric::Not_drawable_quadric::draw(){}

//x^2+y^2+z^2 = 0
Quadric::Point::Point(){}

void Quadric::Point::draw()
{
  glBegin(GL_POINTS);
    glVertex3d(0,0,0);
  glEnd();
}

//x^2+y^2 = 0
Quadric::Line::Line(double z1, double z2)
{
  this->z1 = z1;
  this->z2 = z2;
}

void Quadric::Line::draw()
{
  glBegin(GL_LINES);
    glVertex3d(0,0,z1);
    glVertex3d(0,0,z2);
  glEnd();
}

//x^2 + y^2 + z^2 = 1
Quadric::Sphere::Sphere( double y1, double y2, unsigned int aufl_x, unsigned int aufl_y, double angle_x ){
//std::cout << "erstelle Sphere... \n";
  if (y1 >= -1.0 && y2 <= 1.0){
    Vector3d kontur_anfang( -sqrt( 1.0 - pow( y2, 2 ) ), y2, 0.0 );
    Vector3d kontur_ende( -sqrt( 1.0 - pow( y1, 2 ) ), y1, 0.0 );
    double angle_y = kontur_anfang.angle(kontur_ende);
    
    std::vector<Vector3d> kontur(aufl_x + 1);
    kontur.at(0) = Vector3d( kontur_anfang );

    double dCos=cos( angle_y / (double)aufl_x);
    double dSin=sin( angle_y / (double)aufl_x);
    for (unsigned int i=1; i<(aufl_x + 1); i++){
      kontur.at(i) = kontur.at(i-1).rotate_z( dSin, dCos );
    }
    this->set_surface_by_rotation( kontur, aufl_y, angle_x, Y );
  }
}

void Quadric::Sphere::set_vertex_normals()
{
//std::cout << "setze normals..." << std::endl;
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ) {
    vertex_normal.at(i) = vertex.at(i).normalize();  
  }
//std::cout << "normals gesetzt..." << std::endl; 
}

void Quadric::Sphere::draw()
{
/*  GLUquadricObj* quad = gluNewQuadric( );
  gluSphere( quad, 1.0, 60, 60 );*/
  Uniform_quadric::draw();
}

//x^2 + y^2 - z^2 = 1
Quadric::Uniform_hyperboloid_of_one_sheet::Uniform_hyperboloid_of_one_sheet( double z1, double z2, unsigned int aufl_z, unsigned int aufl_y, double angle ){
  std::vector<Vector3d> kontur(aufl_z + 1);
  double step = fabs(z2 - z1)/(double)aufl_z;
  double z = z2;
  for (unsigned int i=0; i<(aufl_z + 1); i++) {
    kontur[i] = Vector3d( 0.0 , -sqrt( 1 + z*z ), z );
    z -= step;
  }
  this->set_surface_by_rotation( kontur, aufl_y , angle, Z );
}

void Quadric::Uniform_hyperboloid_of_one_sheet::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ) {
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 2*v.y(), -2*v.z() );
    vertex_normal[i] = Vector3d( partielle_ableitung.normalize() );  
  }
}

//x^2 + y^2 - z^2 = 0
Quadric::Uniform_cone::Uniform_cone(double z1, double z2, unsigned int aufl_z,
                                    unsigned int aufl_y, double angle )
{
  std::vector<Vector3d> kontur(aufl_z + 1);
  double step = fabs(z2 - z1)/(double)aufl_z;
  double z = z2;
  for (unsigned int i=0; i<(aufl_z + 1); i++){
    kontur[i] = Vector3d( 0.0, -1*fabs(z), z );
    z -= step;
  }
  this->set_surface_by_rotation( kontur, aufl_y , angle, Z );
}

void Quadric::Uniform_cone::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 2*v.y(), -2*v.z() );
    vertex_normal[i] = Vector3d( partielle_ableitung.normalize() );  
  }
}

//x^2 + y^2 + 2*z = 0
Quadric::Uniform_elliptic_paraboloid::
Uniform_elliptic_paraboloid( double z1, double z2, unsigned int aufl_y,
                             unsigned int aufl_z, double angle )
{
  if (z1 >= 0 && z2 > 0 && z2 > z1){
    
    double y1= -sqrt(2*z1);
    double y2= -sqrt(2*z2);

    double step = fabs(y1-y2)/(double)(aufl_y);
    double y = y1;
    std::vector<Vector3d> dummy;
    while (y >= y2){
      dummy.push_back( Vector3d( 0.0, y, 0.5*y*y ) );
      y -= step;
    };
    
    std::vector<Vector3d> kontur(dummy.size());
    for (unsigned int j=0; j<dummy.size(); j++){
      kontur[j] = dummy[dummy.size() -1 - j];
    };
  
    this->set_surface_by_rotation( kontur, aufl_z , angle, Z );
  }
}

void Quadric::Uniform_elliptic_paraboloid::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 2*v.y(), -2 );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

//x^2 + y^2 = 1
Quadric::Cylinder::Cylinder(double z1, double z2, unsigned int aufl_z,
                            unsigned int aufl_y, double angle )
{
  std::vector<Vector3d> kontur(aufl_z + 1);
  double step = fabs(z2 - z1)/(double)aufl_z;
  double z = z2;
  for (unsigned int i=0; i<(aufl_z + 1); i++){
    kontur[i] = Vector3d( 0.0, -1.0, z );
    z -= step;
  }
  this->set_surface_by_rotation( kontur, aufl_y , angle, Z );
}

void Quadric::Cylinder::set_vertex_normals(){
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 2*v.y(), 0 );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

//x^2 + y^2 - z^2 = -1
Quadric::Uniform_hyperboloid_of_two_sheets::
Uniform_hyperboloid_of_two_sheets(double z1, double z2, unsigned int aufl_z,
                                  unsigned int aufl_y, double angle )
{
  double y1=0.0; if (z1 < -1 || z1 > 1) y1=sqrt( z1*z1 - 1);
  double y2=0.0; if (z2 < -1 || z2 > 1) y2=sqrt( z2*z2 - 1);

  double step = (y1 + y2) / (double)aufl_z;
  if (z2 < -1 || z1 > 1) step = (fabs(y1-y2)) / (double)aufl_z;

  //linke schale
  if (z1 < -1){
    std::vector<Vector3d> kontur;
    double y = 0.0;
    if (z2 < -1) y=y2;
    while (y <= y1){
      kontur.push_back( Vector3d( 0.0, -y, -sqrt( y*y +1 ) ) );
      y += step;
    }
    this->set_surface_by_rotation( kontur, aufl_y , angle, Z );
    vertex1 = vertex;
    vertex_normal1 = vertex_normal;
    face1 = face;
  }

  //rechte schale
  if (z2 > 1){
    double y = 0.0;
    if (z1 > 1) y=y1;
    std::vector<Vector3d> dummy;
    while (y <= y2){
      dummy.push_back( Vector3d( 0.0, -y, sqrt( y*y +1 ) ) );
      y += step;
    };
    
    std::vector<Vector3d> kontur1(dummy.size());
    for (unsigned int j=0; j<dummy.size(); j++){
      kontur1[j] = dummy[dummy.size() -1 - j];
    };  

    this->set_surface_by_rotation( kontur1, aufl_y , angle, Z );
  }
}

void Quadric::Uniform_hyperboloid_of_two_sheets::set_vertex_normals()
{
//std::cout << " test set normals hyp. of 2 s. "<< std::endl;
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 2*v.y(), -2*v.z() );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

void Quadric::Uniform_hyperboloid_of_two_sheets::draw()
{
//std::cout << " test draw hyp. of 2 s. "<< std::endl;
  Uniform_quadric::draw();

  std::vector<Vector3d> dummy_vertex;
  std::vector<Vector3d> dummy_vertex_normal;
  std::vector< std::vector< int > > dummy_face;  

  dummy_vertex = vertex;
  dummy_vertex_normal = vertex_normal;
  dummy_face = face;

  vertex = vertex1;
  vertex_normal = vertex_normal1;
  face = face1;

  vertex1 = dummy_vertex;
  vertex_normal1 = dummy_vertex_normal;
  face1 = dummy_face;

  Uniform_quadric::draw();
}

void Quadric::Uniform_hyperboloid_of_two_sheets::swap_vertex_normals()
{
  Uniform_quadric::swap_vertex_normals();

  for (unsigned int i=0; i<vertex_normal1.size(); i++){
    vertex_normal1[i][0] *= -1;
    vertex_normal1[i][1] *= -1;
    vertex_normal1[i][2] *= -1;
  }
}

//x^2 + 2y = 0
Quadric::Uniform_parabolic_cylinder::
Uniform_parabolic_cylinder(double x1, double x2, double z1, double z2,
                           unsigned int aufl_x, unsigned int aufl_z )
{
  double step = fabs(x2 - x1)/(double)aufl_x;
  double x = x2;
  std::vector<Vector3d> kontur;
  while (x >= x1){
    kontur.push_back( Vector3d( x, -0.5*x*x, z1 ) );
    x -= step;
  };
  
  double distance = z2 - z1;
  this->set_surface_by_translation( kontur, aufl_z , distance, Z );
}

void Quadric::Uniform_parabolic_cylinder::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 2, 0 );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

//x^2 - y^2 = 1
Quadric::Uniform_hyperbolic_cylinder::
Uniform_hyperbolic_cylinder(double x1, double x2, double y1, double y2,
                            double z1, double z2, unsigned int aufl_z,
                            unsigned int aufl_y )
{  
  //linkes teil
  if ( x1 < -1 ){
    std::vector<Vector3d> dummy;
    double step = fabs(y2 - y1)/(double)aufl_y;
    double y = y1;
    for (unsigned int i=0; i<(aufl_y + 1); i++){
      double x = -sqrt( 1 + y*y );
      if (x>=x1 and x<=x2) dummy.push_back( Vector3d( x, y, z2 ) );
      y = y + step;
    }
    std::vector<Vector3d> kontur;
    for (unsigned int i=0; i<dummy.size(); i++){
      kontur.push_back( dummy[dummy.size() -1 - i] );
    }
    double distance = z1 - z2;
    this->set_surface_by_translation( kontur, aufl_z , distance, Z );
    vertex1 = vertex;
    vertex_normal1 = vertex_normal;
    face1 = face;
  }

  //rechtes teil
  if ( x2 > 1 ){
    std::vector<Vector3d> kontur1;
    double step = fabs(y2 - y1)/(double)aufl_y;
    double y = y2;
    for (unsigned int i=0; i<(aufl_y + 1); i++){
      double x = sqrt( 1 + y*y );
      if (x>=x1 and x<=x2) kontur1.push_back( Vector3d( x, y, z2 ) );
      y = y - step;
    }
    double distance = z1 - z2;
    this->set_surface_by_translation( kontur1, aufl_z , distance, Z );
  }
}

void Quadric::Uniform_hyperbolic_cylinder::set_vertex_normals(){
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), -2*v.y(), 0 );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

void Quadric::Uniform_hyperbolic_cylinder::draw()
{
  Uniform_quadric::draw();

  std::vector<Vector3d> dummy_vertex;
  std::vector<Vector3d> dummy_face_normal;
  std::vector<Vector3d> dummy_vertex_normal;
  std::vector< std::vector< int > > dummy_face;  

  dummy_vertex = vertex;
  dummy_vertex_normal = vertex_normal;
  dummy_face = face;

  vertex = vertex1;
  vertex_normal = vertex_normal1;
  face = face1;

  vertex1 = dummy_vertex;
  vertex_normal1 = dummy_vertex_normal;
  face1 = dummy_face;

  Uniform_quadric::draw();
}

void Quadric::Uniform_hyperbolic_cylinder::swap_vertex_normals()
{
  Uniform_quadric::swap_vertex_normals();

  for (unsigned int i=0; i<vertex_normal1.size(); i++){
    vertex_normal1[i][0] *= -1;
    vertex_normal1[i][1] *= -1;
    vertex_normal1[i][2] *= -1;
  }
}

//x^2 - y^2 + 2z = 0
Quadric::Uniform_hyperbolic_paraboloid::
Uniform_hyperbolic_paraboloid(double x1, double x2, double y1, double y2,
                              unsigned int aufl_x, unsigned int aufl_y )
{
  double x = x1;
  double step_x = fabs( x1 - x2 )/(double)aufl_x;
  double y = y1;
  double step_y = fabs( y1 - y2 )/(double)aufl_y;

  for ( unsigned int i=0; i<aufl_y+1; i++){
    for ( unsigned int j=0; j<aufl_x+1; j++){
      vertex.push_back( Vector3d(x, y, 1/2.0*( y*y -x*x ) ) );
      x += step_x;
    }
    y += step_y;
    x = x1;
  } 

  set_faces( aufl_x + 1, aufl_y);
  set_vertex_normals();
}

void Quadric::Uniform_hyperbolic_paraboloid::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), -2*v.y(), 2 );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

//x^2 - y^2 = 0
Quadric::Uniform_intersecting_planes::
Uniform_intersecting_planes(double x1, double x2, double y1, double y2,
                            double z1, double z2, unsigned int aufl_x,
                            unsigned int aufl_y, unsigned int aufl_z)
{
  //X-Z Ebene:
  if (x1<x2){
    double z = z2;
    double step = fabs( z1 - z2 )/(double)aufl_z;
  
    std::vector<Vector3d> kontur;
    for ( unsigned int i=0; i<aufl_z+1; i++){
      kontur.push_back( Vector3d( x1, 0.0, z ) );
      z -= step;
    } 
    double distance = x2 - x1;
    this->set_surface_by_translation( kontur, aufl_x , distance, X );
  
    vertex1 = vertex;
    vertex_normal1 = vertex_normal;
    face1 = face;
  }

  //Y-Z Ebene:
  if (y1<y2){
    double z = z2;
    double step = fabs( z1 - z2 )/(double)aufl_z;
  
    std::vector<Vector3d> kontur1;
    for ( unsigned int i=0; i<aufl_z+1; i++){
      Vector3d dummy(0.0, y1, z);
      kontur1.push_back( Vector3d( 0.0, y1, z ) );
      z -= step;
    } 
    double distance = y2 - y1;
    this->set_surface_by_translation( kontur1, aufl_y , distance, Y );
  }
}

void Quadric::Uniform_intersecting_planes::draw()
{
  //das gebilde muss noch um pi/4 um die Z-Achse rotiert werden um x^2 - y^2 = 0 zu sein
  glPushMatrix();
  glRotated( 45, 0, 0, 1);

  Uniform_quadric::draw();

  std::vector<Vector3d> dummy_vertex;
  std::vector<Vector3d> dummy_face_normal;
  std::vector<Vector3d> dummy_vertex_normal;
  std::vector< std::vector< int > > dummy_face;  

  dummy_vertex = vertex;
  dummy_vertex_normal = vertex_normal;
  dummy_face = face;

  vertex = vertex1;
  vertex_normal = vertex_normal1;
  face = face1;

  vertex1 = dummy_vertex;
  vertex_normal1 = dummy_vertex_normal;
  face1 = dummy_face;

  Uniform_quadric::draw();
  glPopMatrix();
}

void Quadric::Uniform_intersecting_planes::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d normal( fabs(v.y()), -fabs(v.x()), 0.0 );
    vertex_normal[i] = normal.normalize();  
  }
}

void Quadric::Uniform_intersecting_planes::swap_vertex_normals()
{
  Uniform_quadric::swap_vertex_normals();

  for (unsigned int i=0; i<vertex_normal1.size(); i++){
    vertex_normal1[i][0] *= -1;
    vertex_normal1[i][1] *= -1;
    vertex_normal1[i][2] *= -1;
  }
}

//x^2 = 1
Quadric::Uniform_parallel_planes::
Uniform_parallel_planes(double y1, double y2, double z1, double z2,
                        unsigned int aufl_y, unsigned int aufl_z)
{

  //x = 1 Ebene
  if (y1<y2){
    double z = z2;
    double step = fabs( z1 - z2 )/(double)aufl_z;
  
    std::vector<Vector3d> kontur;
    for ( unsigned int i=0; i<aufl_z+1; i++){
      kontur.push_back( Vector3d( 1, y1, z ) );
      z -= step;
    } 
    double distance = y2 - y1;
    this->set_surface_by_translation( kontur, aufl_y , distance, Y );
  }
}

void Quadric::Uniform_parallel_planes::draw(){
  glPushMatrix();
  Uniform_quadric::draw();

  glTranslated(-2, 0, 0);
  Uniform_quadric::draw();

  glPopMatrix();
}

void Quadric::Uniform_parallel_planes::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d v(vertex[i]);
    Vector3d partielle_ableitung( 2*v.x(), 0.0, 0.0 );
    vertex_normal[i] = partielle_ableitung.normalize();  
  }
}

//x^2 = 0
Quadric::Uniform_coincident_planes::
Uniform_coincident_planes(double y1, double y2, double z1, double z2,
                          unsigned int aufl_y, unsigned int aufl_z)
{
  if (y1<y2){
    double z = z2;
    double step = fabs( z1 - z2 )/(double)aufl_z;
  
    std::vector<Vector3d> kontur;
    for ( unsigned int i=0; i<aufl_z+1; i++){
      kontur.push_back( Vector3d( 0.0, y1, z ) );
      z -= step;
    } 
    double distance = y2 - y1;
    this->set_surface_by_translation( kontur, aufl_y , distance, Y );
  }
}

void Quadric::Uniform_coincident_planes::set_vertex_normals()
{
  vertex_normal = std::vector<Vector3d>( vertex.size() );  //so viele vertex normalen wird man brauchen (eine pro vertex)

  for ( unsigned int i=0; i<vertex.size(); i++ ){
    Vector3d normal( 1.0, 0.0, 0.0 );
    vertex_normal[i] = normal.normalize();  
  }
}
