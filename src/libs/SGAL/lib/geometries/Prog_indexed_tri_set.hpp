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
// $Source: $
// $Revision: 4966 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/**
 * @class: Prog_indexed_tri_set
 *
 * Purpose: An indexed face set geometry that is loaded asynchronously.
 *
 * Description:
 *
 *  Inherits from Indexed_face_set and Load_int.
 */

#include "Indexed_face_set.h"
#include "Load_int.h"
#include "Indexed_face_setGeometry.h"
#include "Debug.h"

class Prog_indexed_tri_set : public Indexed_face_set , public Load_int
{
  DECLARE_DEBUG_CLASS(DBG_PROGINDEXEDTRISET);

public:
  Prog_indexed_tri_set();
  virtual ~Prog_indexed_tri_set();

  enum { FIRST = 0 , SECOND = 1 };

  virtual void AllocateMemory();

  // ---- Load_int methods  ----
  virtual Item_primitive* get_data() { return &m_geometry; }
  virtual void Convert() {}
  virtual void Update(Boolean is_last);
  virtual void delete_data();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

private:
  short m_currentLevel;

  vector<Normal_set *> m_normalset_buffers;
  vector<Int *> m_coordIndicesBuffers;
  vector<Int *> m_texCoordIndicesBuffers;
  vector<Int *> m_normalIndicesBuffers;
  vector<Int *> m_triStripLengthsBuffers;
  vector<Int>   m_displayListArray;
  vector<Int>   m_numPolygonsArray;
  vector<Boolean>  m_keepLevel;

  // used to keep track of memory allocations
  vector<Int>   m_bytes;
  Int m_memoryBase;

  // these are used by the decoder
  Normal_set *m_decNormal_set;
  Int *m_decCoordIndices;
  Int *m_decTexCoordIndices;
  Int *m_decNormalIndices;
  Int *m_decTriStripLengths;


  void AllocateLevel();
  void DeleteLastLevel();
  void DeleteLevel(const int level);
  Boolean IsKeepLOD(Int level, Boolean is_last) const;
  virtual void SetRenderingPointers(Boolean isDynamic=True);
  virtual void SetDisplayListId();
  int GetLevelToRender() const;
  Prog_indexed_tri_set(const Prog_indexed_tri_set&) {}
  void DestroyData();
};

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Prog_indexed_tri_set::create()
{ return new Prog_indexed_tri_set(); }
