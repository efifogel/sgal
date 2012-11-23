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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_INDEXED_FACE_SET_GEOMETRY_HPP
#define SGAL_INDEXED_FACE_SET_GEOMETRY_HPP

/*! \file
 * contains pointers to data sets that are part of indexed face set.
 *
 * This class is used in ProgIndexedTriSet to group the various data 
 * sets that make an indexed face set, such as list of coordinates, 
 * colors, normals, texture coordinats and the corresponding index arrays. 
 * This class is the data sets passed between the decoder and the 
 * IndexedFaceSet class. The pointers are pointing to chuncks of memory 
 * that was allocated somewhere else and no memory should be freed 
 * in this class.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Item_primitive.hpp"

SGAL_BEGIN_NAMESPACE

class Vector3f;
class Vector2f;

class Indexed_face_set_geometry : public Item_primitive {
public:
  Indexed_face_set_geometry() : 
    m_coord_array(NULL), 
    m_normal_array(NULL), 
    m_tex_coord_array(NULL),
    m_coord_indices(NULL),
    m_tex_coord_indices(NULL),
    m_normals_indices(NULL)
  {
    m_attributes.push_back("0");        // num vertices
    m_attributes.push_back("0");        // num triangles
    m_attributes.push_back("0");        // num normals
    m_attributes.push_back("FALSE");    // has textures
    m_attributes.push_back("0");        // default crease angle 
    m_attributes.push_back("FALSE");    // flat shading (normal per vertex) 
    m_attributes.push_back("TRUE");     // normal per vertex
    m_attributes.push_back("TRUE");     // true if the vertices are given in
                                        // CCW 
    m_attributes.push_back("FALSE");    // true if triStrips are used in the
                                        // geometry being rendered
    m_attributes.push_back("FALSE");    // true if triStrips are used in the
                                        // geometry being decoded
    m_attributes.push_back("0, 0, 0");  // the bbox center
    m_attributes.push_back("0, 0, 0");  // the bbox size
    m_attributes.push_back("7");        // forced maximum levels
                                        // (-1 == decide automatically) 
    m_attributes.push_back("0.0");      // visual quality (0 == worst,
                                        // 1==best, -2 == no visual quality
                                        // considerations)
    m_attributes.push_back("0");        // smoothness (0 == no smooth)
    m_attributes.push_back("0");        // max triangles in low level
    m_attributes.push_back("1");        // default compression mode of mesh0
    m_attributes.push_back("TRUE");     // default remove identical vertices
    m_attributes.push_back("FALSE");    // default single uv per vertex
    m_attributes.push_back("0");
    m_attributes.push_back("12");       // default quantization factor
  }

  virtual ~Indexed_face_set_geometry() {}

  // This is the list of attributes. If you add a new value to this
  // enum, make sure you insert a default value in the attribute
  // array in the constructor. Make sure that the order in which the 
  // attribute is inserted, is the same as the order of values in
  // the enum.
  enum {
    VERTEX_NUM,
    TRIANGLE_NUM,
    NORMAL_NUM,
    HAS_TEXTURE,
    CREASE_ANGLE,
    FLAT_SHADE,
    MULTIPLE_NORMAL_PER_VERTEX,
    IS_CCW,
    IS_TRI_STRIP,
    IS_TRI_STRIP_DECODED,
    BBOX_CENTER,
    BBOX_SIZE,
    MAX_LEVELS,
    VISUAL_QUALITY,
    SMOOTHNESS,
    MAX_TRIANGLES,
    MESH0_MODE,      // old/new mesh0 compression
    ECONOMIC_MODE,    // remove identical vertices
    MULTIPLE_UV_PER_VERTEX,
    MAX_UV_NUM,
    QUANTIZATION_FACTOR,
    LAST
  };
    
  void set_vertex_num(int vertex_num);
  int get_vertex_num();

  void set_triangle_num(int triangle_num);
  int get_triangle_num();

  void set_coord_array(Coord_array * coord_array)
  { m_coord_array = coord_array; }
  Coord_array* get_coord_array() const { return m_coord_array; }

  void set_normal_array(Normal_array * normal_array)
  { m_normal_array = normal_array; }
  Normal_array* get_normal_array() const { return m_normal_array; }

  void set_tex_coord_array(Tex_coord_array* tc_array)
  { m_tex_coord_array = tc_array; }
  Tex_coord_array* get_tex_coord_array() const { return m_tex_coord_array; }

  void set_coord_indices(Int* indices) { m_coord_indices = indices; }
  Int* get_coord_indices() const { return m_coord_indices; }

  void set_tex_coord_indices(Int* indices) { m_tex_coord_indices = indices; }

  void set_normals_indices(Int* indices) { m_normals_indices = indices; }
  Int* get_normals_indices() const { return m_normals_indices; }

  void set_tri_strip_lengths(Int* tsl) { m_tri_strip_length = tsl; }
  Int* get_tri_strip_lengths() { return m_tri_strip_length; }

  Int* get_tex_coord_indices() { return m_tex_coord_indices; }

private:
  /*! number of vertices */
  int m_vertex_num;

  /*! number of triangles */
  int m_triangle_num;

  /* the points array */  
  Coord_array* m_coord_array;

  /*! A list of normals */
  Normal_array* m_normal_array;

  /*! A list of texture coordinates */
  Tex_coord_array* m_tex_coord_array;

  /*! A list of triangles which is an index array into the coordinate list. */
  Int* m_coord_indices;

  /*! An index array to specify texture coordinates per triangle. */
  Int* m_tex_coord_indices;

  /*! An index array to specify normals per vertex per triangle. */
  Int* m_normals_indices;

  /*! Length of strips : the first number is the number of strips and the
   * following numbers are strips length 
   */
  Int* m_tri_strip_length;

  Indexed_face_set_geometry(const Indexed_face_set_geometry &) {}
};

/*!
 */
inline void Indexed_face_set_geometry::set_vertex_num(int /* vertex_num */) 
{ 
  char* buffer = new char[32];
  //! \todo m_attributes[VERTEX_NUM] = _gcvt((double)vertex_num, 7, buffer); 
  delete buffer;
}

/*!
 */
inline int Indexed_face_set_geometry::get_vertex_num() 
{ 
  return atoi(m_attributes[VERTEX_NUM].c_str());
}

/*!
 */
inline void Indexed_face_set_geometry::set_triangle_num(int /* triangle_num */) 
{ 
  char* buffer = new char[32];
  //! \todo m_attributes[TRIANGLE_NUM] = _gcvt((double)triangle_num, 7, buffer); 
  delete buffer;
}

/*!
 */
inline int Indexed_face_set_geometry::get_triangle_num() 
{ 
  return atoi(m_attributes[TRIANGLE_NUM].c_str());
}

SGAL_END_NAMESPACE

#endif
