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
// File          : demos/xqvisualization/include/Quadric.h
// QdX_release   : $Name:  $
// Revision      : $Revision: 5272 $
// Revision_date : $Date: 2007-10-22 15:08:57 +0200 (Mon, 22 Oct 2007) $
//
// Author(s)     : Roman Tulchyner <rtulchy@mpi-inf.mpg.de>
//
//============================================================================


#ifndef QUADRIC_H
#define QUADRIC_H

//#include <QdX/basic.h>

#include <vector>
#include "Vector3d.hpp"

#include <QdX/Quadric_3.h>
#include <NiX/Arithmetic_traits.h>

#include "typedefs.hpp"

class Quadric{
	//nötige "forward declarations"
	class Uniform_quadric;

	public:
//		Quadric();
		Quadric(const Quadric_3& quadric_3);
		~Quadric();
		void draw();
		Quadric_3 get_quadric_3() const;
		const char* get_quadric_type() { return quadric_type; };
	private:
		const char* quadric_type;
		void set_transformation_Matrix(Matrix_real& matrix_u_4x4);
		Matrix_rat gramm_schmidt_diagonalize( Matrix_rat& matrix_a);
		Matrix_real diagonal_matrix_normalize( Matrix_real& matrix_d);
		Matrix_real set_proper_order( Matrix_real& matrix_u_3x3, Matrix_real& matrix_a_3x3 );

		double m[16]; //transformation_Matrix	
		Uniform_quadric* uniform_quadric;
		Quadric_3 quadric_3;
		//für display lists
                std::pair< bool, int > display_list_option;

	class Uniform_quadric{
		public:
			virtual ~Uniform_quadric();
			virtual void draw();
			void swap_vertex_normals();
	
		protected:
	
			void set_surface_by_rotation( const std::vector<Vector3d>& kontur, int fragmentation, double angle, Axis axis );
			void set_surface_by_translation( const std::vector<Vector3d>& kontur, int fragmentation, double distance, Axis axis );
	
			void set_faces( unsigned int kontur_size, unsigned int fragmentation );
			virtual void set_vertex_normals();
	
			std::vector<Vector3d> vertex;
			std::vector<Vector3d> vertex_normal;
			std::vector< std::vector< int > > face;
	};
	
	class Not_drawable_quadric: public Uniform_quadric{
		public:
			Not_drawable_quadric();
			void draw();
	};
	
	class Point: public Uniform_quadric{
		public:
			Point();
			void draw();
	};
	
	class Line: public Uniform_quadric{
		public:
			Line(double z1, double z2);
			void draw();
		private:
			double z1;
			double z2;
	};
	
	class Uniform_hyperbolic_paraboloid: public Uniform_quadric{
		public:
			Uniform_hyperbolic_paraboloid( double x1, double x2, double y1, double y2, unsigned int aufl_x, unsigned int aufl_y );
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_parabolic_cylinder: public Uniform_quadric{
		public:
			Uniform_parabolic_cylinder( double x1, double x2, double z1, double z2, unsigned int aufl_z, unsigned int aufl_x );
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_hyperbolic_cylinder: public Uniform_quadric{
		public:
			Uniform_hyperbolic_cylinder( double x1, double x2, double y1, double y2, double z1, double z2, unsigned int aufl_z, unsigned int aufl_x );
			void draw();
			void swap_vertex_normals();
		protected:
			std::vector<Vector3d> vertex1;
			std::vector<Vector3d> vertex_normal1;
			std::vector< std::vector< int > > face1;
	
			void set_vertex_normals();
	};
	
	class Sphere: public Uniform_quadric{
		public:
			Sphere( double y1, double y2, unsigned int aufl_x, unsigned int aufl_y, double angle );
			void draw();
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_hyperboloid_of_one_sheet: public Uniform_quadric{
		public:
			Uniform_hyperboloid_of_one_sheet( double z1, double z2, unsigned int aufl_z, unsigned int aufl_y, double angle );
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_cone: public Uniform_quadric{
		public:
			Uniform_cone( double z1, double z2, unsigned int aufl_z, unsigned int aufl_y, double angle );
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_elliptic_paraboloid: public Uniform_quadric{
		public:
			Uniform_elliptic_paraboloid( double z1, double z2, unsigned int aufl_y, unsigned int aufl_z, double angle );
		protected:
			void set_vertex_normals();
	};
	
	class Cylinder: public Uniform_quadric{
		public:
			Cylinder( double z1, double z2, unsigned int aufl_z, unsigned int aufl_y, double angle );
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_hyperboloid_of_two_sheets: public Uniform_quadric{
		public:
			Uniform_hyperboloid_of_two_sheets( double y1, double y2, unsigned int aufl_z, unsigned int aufl_y, double angle );
			void draw();
			void swap_vertex_normals();
		protected:
			std::vector<Vector3d> vertex1;
			std::vector<Vector3d> vertex_normal1;
			std::vector< std::vector< int > > face1;
	
			void set_vertex_normals();
	};
	
	class Uniform_intersecting_planes: public Uniform_quadric{
		public:
			Uniform_intersecting_planes( double x1, double x2, double y1, double y2, double z1, double z2, unsigned int aufl_x, unsigned int aufl_y, unsigned int aufl_z);
			void draw();
			void swap_vertex_normals();
		protected:
			std::vector<Vector3d> vertex1;
			std::vector<Vector3d> vertex_normal1;
			std::vector< std::vector< int > > face1;
	
			void set_vertex_normals();
	};
	
	class Uniform_parallel_planes: public Uniform_quadric{
		public:
			Uniform_parallel_planes( double y1, double y2, double z1, double z2, unsigned int aufl_y, unsigned int aufl_z);
			void draw();
		protected:
			void set_vertex_normals();
	};
	
	class Uniform_coincident_planes: public Uniform_quadric{
		public:
			Uniform_coincident_planes( double y1, double y2, double z1, double z2, unsigned int aufl_y, unsigned int aufl_z);
		protected:
			void set_vertex_normals();
	};
};

#endif // QUADRIC_H
 
