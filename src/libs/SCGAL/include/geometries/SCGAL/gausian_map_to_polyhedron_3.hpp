// Copyright (c) 2004 Israel.
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
// $Source$
// $Revision: 5323 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GAUSIAN_MAP_TO_POLYHEDRON_3_HPP
#define SGAL_GAUSIAN_MAP_TO_POLYHEDRON_3_HPP

#include <CGAL/Modifier_base.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

template <class Kernel, class HDS>
class gausian_map_to_polyhedron_3 : public CGAL::Modifier_base<HDS> {

  typedef CGAL::Nef_gaussian_map<Kernel> Gausian_map;
  typedef typename Gausian_map::SFace_const_iterator SFace_const_iterator;
  typedef typename Gausian_map::SVertex_const_iterator SVertex_const_iterator;
  typedef typename Gausian_map::SHalfedge_around_svertex_const_circulator 
    SHalfedge_around_svertex_const_circulator;
  
  const Gausian_map& G;
  CGAL::Unique_hash_map<SFace_const_iterator, int> SFace2int;
  
 public:
  gausian_map_to_polyhedron_3(const Gausian_map& Gin) : G(Gin) {}
    
    void operator()( HDS& hds) {
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( G.number_of_sfaces(), G.number_of_svertices(), G.number_of_shalfedges());
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;

	int i = 0;
	SFace_const_iterator fi;
	for(fi = G.sfaces_begin(); fi != G.sfaces_end(); ++fi) {
	  B.add_vertex(fi->mark());
	  SFace2int[fi] = i++;
	}

	SVertex_const_iterator vi;
	for(vi = G.svertices_begin(); vi != G.svertices_end(); ++vi) {
	  SHalfedge_around_svertex_const_circulator 
	    svc(G.first_out_edge(vi)),
	    svend(svc);
	  B.begin_facet();
	  CGAL_For_all(svc,svend)
	    B.add_vertex_to_facet(SFace2int[svc->incident_sface()]);
	  B.end_facet();
	}
	B.end_surface();
    }
};

#endif
