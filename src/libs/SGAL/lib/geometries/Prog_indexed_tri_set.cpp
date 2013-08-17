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

#include "Scene_graphPch.h"
#include "ProgIndexedTriSet.h"
#include "Indexed_face_setGeometry.h"
#include "CoordSet.h"
#include "Normal_set.h"
#include "TexCoordSet.h"
#include "ExecutionCoordinator.h"
#include "Draw_action.h"
#include "Trace.h"

using namespace SGAL;

std::string Prog_indexed_tri_set::s_tag = "enbProgIndexedTriSet";

REGISTER_TO_FACTORY(Prog_indexed_tri_set, "Prog_indexed_tri_set");

/*! Constructor */
Prog_indexed_tri_set::Prog_indexed_tri_set() : Indexed_face_set(),
  m_currentLevel(-1),
  m_decNormal_set(0),
  m_decCoordIndices(0),
  m_decTexCoordIndices(0),
  m_decNormalIndices(0),
  m_decTriStripLengths(0),
  m_memoryBase(0)
{
  REGISTER_DEBUG_CLASS(DBG_PROGINDEXEDTRISET, "EAIndFaceSet")
  m_isProgressive = true;
}

/*! Destructor */
Prog_indexed_tri_set::~Prog_indexed_tri_set()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Prog_indexed_tri_set ...");
  DestroyData();
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

void Prog_indexed_tri_set::DestroyData()
{
  int i = 0;
  for ( i = 0 ; i < m_coordIndicesBuffers.size() ; i++ ) {
    DELETE_ARRAY ( m_coordIndicesBuffers[i] );
  }
  m_coordIndicesBuffers.clear();

  for ( i = 0 ; i < m_normalset_buffers.size() ; i++ ) {
    DELETE_OBJECT( m_normalset_buffers[i] );
  }
  m_normalset_buffers.clear();

  for ( i = 0 ; i < m_normalIndicesBuffers.size() ; i++ ) {
    DELETE_ARRAY ( m_normalIndicesBuffers[i] );
  }
  m_normalIndicesBuffers.clear();

  for ( i = 0 ; i < m_texCoordIndicesBuffers.size() ; i++ ) {
    DELETE_ARRAY ( m_texCoordIndicesBuffers[i] );
  }
  m_texCoordIndicesBuffers.clear();

  for ( i = 0 ; i < m_triStripLengthsBuffers.size() ; i++ ) {
    DELETE_ARRAY ( m_triStripLengthsBuffers[i] );
  }
  m_triStripLengthsBuffers.clear();

  DELETE_ARRAY ( m_decCoordIndices );
  DELETE_ARRAY ( m_decTexCoordIndices );
  DELETE_OBJECT( m_decNormal_set );
  DELETE_ARRAY ( m_decNormalIndices );
  DELETE_ARRAY ( m_decTriStripLengths );

  m_coordIndices = 0;
  m_normalSet = 0;
  m_normalIndices = 0;
  m_texCoordIndices = 0;
  m_triStripLengths = 0;

  if ( m_useDisplayList ) {
    for ( i = 0 ; i < m_displayListArray.size() ; i++ ) {
      if ( m_displayListArray[i] != -1 ) {
        glDeleteLists(m_displayListArray[i], 1);
      }
    }
    m_displayListId = -1;
  }
  m_displayListArray.clear();
}

/**
 * Allocate the memory for the geometries for both buffers 
 */
void Prog_indexed_tri_set::allocate_memory()
{
  bool isMultiple = false;
  m_geometry.get_value(Indexed_face_setGeometry::MULTIPLE_NORMAL_PER_VERTEX, (bool)isMultiple);
  bool isFlat = false;
  m_geometry.get_value(Indexed_face_setGeometry::FLAT_SHADE, (bool)isFlat);
  m_geometry.get_value(Indexed_face_setGeometry::MULTIPLE_UV_PER_VERTEX, (bool)m_isMultipleUV);
  bool hasTexture = false;
  m_geometry.get_value(Indexed_face_setGeometry::HAS_TEXTURE, (bool)hasTexture);

  int triNum = m_geometry.get_triangleNum();
  int vertNum = m_geometry.get_vertexNum();

  // --  allocate one set of the coordSet and the texCoordSet
  m_memoryBase += vertNum*sizeof(Vector3f);
  m_coordSet = new Coord_set(vertNum);
  m_geometry.SetCoordSet(m_coordSet->get_points());
  
  if ( hasTexture ) {  // FIX this for multiple uv coordinates
    if ( m_isMultipleUV ) {
      m_memoryBase += (3*triNum+vertNum)*sizeof(Vector2f);
      m_texCoordSet = new ETexCoordSet(3*triNum+vertNum);
    }
    else
    {
      m_memoryBase += vertNum*sizeof(Vector2f);
      m_texCoordSet = new ETexCoordSet(vertNum);
    }
    m_geometry.SetTexCoordSet(m_texCoordSet->get_points());
  }
  
  // --  create the indices arrays
  m_decCoordIndices = new Int[3*triNum];
  m_geometry.SetCoordIndices(m_decCoordIndices);


  // --  create the normal set
  if ( isFlat ) {
    m_decNormal_set = new Normal_set(triNum);
  } else {
    if ( isMultiple ) {
      m_decNormal_set = new Normal_set(3*triNum);
      m_decNormalIndices = new Int[3*triNum];
    } else {
       m_decNormal_set = new Normal_set(vertNum);
    }
  }
  m_geometry.SetNormal_set(m_decNormal_set->get_normals());
  m_geometry.SetNormalsIndices(m_decNormalIndices);

  // -- create the texture indices
  if ( m_isMultipleUV ) {
    m_decTexCoordIndices = new Int[3*triNum];
    m_geometry.SetTexCoordIndices(m_decTexCoordIndices);
  } // else no need to allocate texture coordinate indices

  m_decTriStripLengths = new Int[triNum+1];
  m_geometry.SetTriStripLengths(m_decTriStripLengths);

  // After we have used the vertex num and the triangle num to allocate 
  // the necessary memory, we zero these values and they will be reset
  // after each level with the right value for each level.
  m_geometry.set_vertexNum(0);
  m_geometry.SetTriangleNum(0);

  m_executionCoordinator->AddMemory(m_memoryBase);

}

/**
 * This method is called when a decoding of a level in completed (including
 * the refinement). Therefore we swap the buffers of the data.
 */
void Prog_indexed_tri_set::Update(Boolean is_last)
{
  m_executionCoordinator->AddPolygons(m_geometry.get_triangleNum() - m_currNumPolygons);
  m_currNumPolygons = m_geometry.get_triangleNum();

  // allocate memory for the next level and fill in the data 
  // from what the decoder has produced
  AllocateLevel();
  int currentLevel = m_coordIndicesBuffers.size() - 1;
  m_normalset_buffers[currentLevel]->CopyNormals(m_decNormal_set->get_normals());
  memcpy(m_coordIndicesBuffers[currentLevel], m_decCoordIndices, 3*m_numPolygons*sizeof(Int));
  bool isMultiple = false;
  m_geometry.get_value(Indexed_face_setGeometry::MULTIPLE_NORMAL_PER_VERTEX, (bool)isMultiple);
  bool isFlat = false;
  m_geometry.get_value(Indexed_face_setGeometry::FLAT_SHADE, (bool)isFlat);
  if ( !isFlat && isMultiple ) {
    memcpy(m_normalIndicesBuffers[currentLevel], m_decNormalIndices, 3*m_numPolygons*sizeof(Int));
  }

  if ( m_isMultipleUV ) { 
    memcpy(m_texCoordIndicesBuffers[currentLevel], m_decTexCoordIndices, 3*m_numPolygons*sizeof(Int));
  }
  memcpy(m_triStripLengthsBuffers[currentLevel], m_decTriStripLengths, (m_numPolygons+1)*sizeof(Int));

  // set the pointer for the rendering of this level
  m_currentLevel++;

  // figure out whether to keep this level after the loading is done
  bool keep = IsKeepLOD(currentLevel, is_last);
  m_keepLevel.push_back(keep);
  if ( keep ) {
    m_lodIndex2Level.push_back(currentLevel);
  }

  SetRenderingPointers(false);

  // if display list are used, create them here and dispose the data
  // we create display lists only for levels that should be kept
  if ( m_useDisplayList && keep )
  {
    m_displayListId = CreateDisplayList();
    m_displayListArray.push_back(m_displayListId);
    DeleteLastLevel();
  } else {
    m_displayListArray.push_back(-1);
  }

  // if this is the last level, we delete all the buffers used by the decoder alone.
  // we also delete all the levels that need not to be stored 
  if ( is_last ) 
  {
    DELETE_ARRAY ( m_decCoordIndices );
    DELETE_ARRAY ( m_decTexCoordIndices );
    DELETE_OBJECT( m_decNormal_set );
    DELETE_ARRAY ( m_decNormalIndices );
    DELETE_ARRAY ( m_decTriStripLengths );
    if ( m_useDisplayList ) {
      DELETE_OBJECT(m_coordSet);
      DELETE_OBJECT(m_texCoordSet);
    }

    for ( int i = 0 ; i < m_keepLevel.size() ; i++ ) {
      if ( m_keepLevel[i] == false ) {
        DeleteLevel(i);
      }
    }
  }

  m_isSphereBoundDirty = true;

  // Refresh display for level 0
  if (currentLevel >= 0)  {
    if (m_executionCoordinator) {
      m_executionCoordinator->SetRenderingRequired();
    }
  }
}

/**
 * Allocate the next set of arrays for the level just benn updated 
 */
void Prog_indexed_tri_set::AllocateLevel() 
{
  int triNum = m_geometry.get_triangleNum();
  int vertNum = m_geometry.get_vertexNum();
  m_numPolygons = triNum;
  m_numPolygonsArray.push_back(triNum);
  int bytes= 0;

  m_coordSet->SetSize(vertNum);

  bool isMultiple = false;
  m_geometry.get_value(Indexed_face_setGeometry::MULTIPLE_NORMAL_PER_VERTEX, (bool)isMultiple);
  bool isFlat = false;
  m_geometry.get_value(Indexed_face_setGeometry::FLAT_SHADE, (bool)isFlat);

  bytes += 3*triNum*sizeof(Int);
  Int *coordIndices = new Int[3*triNum];
  m_coordIndicesBuffers.push_back(coordIndices);

  Normal_set *normalSet = 0;
  Int *normalIndices  = 0;
  if ( isFlat ) {
    normalSet = new Normal_set(triNum);
    bytes += triNum*sizeof(Vector3f);
  } else {
    if ( isMultiple ) {
      normalSet = new Normal_set(3*triNum);
      normalIndices = new Int[3*triNum];
      bytes += 3*triNum*sizeof(Vector3f);
      bytes += 3*triNum*sizeof(Int);
    } else {
       normalSet = new Normal_set(vertNum);
      bytes += vertNum*sizeof(Vector3f);
    }
  }

  // even if it is null, we put it in to keep the various buffers syncronized
  m_normalset_buffers.push_back(normalSet); 
  m_normalIndicesBuffers.push_back(normalIndices);

  if ( m_isMultipleUV ) {
    Int *texCoordIndices = new Int[3*triNum];
    bytes += 3*triNum*sizeof(Int);
    m_texCoordIndicesBuffers.push_back(texCoordIndices);
  } // else no need to allocate texture coordinate indices

  // we allocate the maximum number of triangle strips possible 
  // plus one to indicate the number

  Int *triStripLength = new Int[triNum+1];
  bytes += (triNum+1)*sizeof(Int);
  m_triStripLengthsBuffers.push_back(triStripLength);
  memset(triStripLength, 0, triNum+1);

  // update the memory usage
  m_executionCoordinator->AddMemory(bytes);
  // keep the amount of memory per level so we can deduct it later
  m_bytes.push_back(bytes); 
}

/**
 * Deletes the last level in the buffers. This is used after a level 
 * has been stored as a display list
 */
void Prog_indexed_tri_set::DeleteLastLevel() 
{
  int level = m_coordIndicesBuffers.size() - 1;
  DELETE_ARRAY(m_coordIndicesBuffers[level]);
  
  DELETE_OBJECT(m_normalset_buffers[level]);
  DELETE_OBJECT(m_normalIndicesBuffers[level]);
  if ( m_isMultipleUV ) {
    DELETE_ARRAY(m_texCoordIndicesBuffers[level]);
  }
  DELETE_ARRAY(m_triStripLengthsBuffers[level]);
}

/**
 * Deletes the data (geometry and/or display list data of a given level
 */
void Prog_indexed_tri_set::DeleteLevel(const int level) 
{
  DELETE_ARRAY(m_coordIndicesBuffers[level]);
  
  DELETE_OBJECT(m_normalset_buffers[level]);
  DELETE_OBJECT(m_normalIndicesBuffers[level]);
  if ( m_isMultipleUV ) {
    DELETE_ARRAY(m_texCoordIndicesBuffers[level]);
  }
  DELETE_ARRAY(m_triStripLengthsBuffers[level]);

  if ( m_displayListArray[level] != -1 ) {
    glDeleteLists(m_displayListArray[level], 1);
  }

  m_executionCoordinator->AddMemory(-1*m_bytes[level]);
  m_bytes[level] = 0;
}

/**
 * Returns true if the specified level should be kept in memory after loading is done.
 */
Boolean Prog_indexed_tri_set::IsKeepLOD(Int level, Boolean is_last) const
{
  int i = 0;
  switch ( m_lodKeepType ) {
  case lkALL:
    return true;
    break;

  case lkDEFAULT:
    if ( is_last || (level == m_lowLevel) ) {
      return true;
    } 
    break;

  case lkCUSTOMIZE:
    for ( i = 0 ; i < m_lodList.size() ; i++ ) {
      if ( m_lodList[i] == level ) {
        return true;
      }
    }
    break;
  default:
    break;
  }

  return false;
}


/**
 If the object is currently being manipulated (dragging is locked)
 we set the current geometry to the low level geometry. Otherwise,
 we set it to the last level that was loaded.
 If isDynamic is false, the most current level is set. Otherwise, 
 framerate is taken into considiration.
 */
void Prog_indexed_tri_set::SetRenderingPointers(Boolean isDynamic) 
{
  int level = GetLevelToRender();
  
  if ( !isDynamic ) {
    level = m_currentLevel;
  }
  if ( level == -1 ) {
    return;
  }

  m_numPolygons = m_numPolygonsArray[level];

  m_normalSet = m_normalset_buffers[level];
  m_coordIndices = m_coordIndicesBuffers[level];
  m_normalIndices = m_normalIndicesBuffers[level];

  bool isMultiple = false;
  m_geometry.get_value(Indexed_face_setGeometry::MULTIPLE_NORMAL_PER_VERTEX, (bool)isMultiple);
  if ( m_isMultipleUV ) {
    m_texCoordIndices = m_texCoordIndicesBuffers[level];
  }
  m_triStripLengths = m_triStripLengthsBuffers[level];

  m_geometry.get_value(Indexed_face_setGeometry::FLAT_SHADE, (bool)m_normalPerVertex);
  m_normalPerVertex =! m_normalPerVertex;
}

/**
 * Set the display list id to use for rendering.
 */
void Prog_indexed_tri_set::SetDisplayListId() 
{
  int level = GetLevelToRender();
  m_displayListId = m_displayListArray[level];
}

/**
 * Returns he current levelt o render.
 * if a level was explicitly specified, that is the level that is returned 
 * (unless invalid, and then it is clamped).
 * Otherwise, determines whether to return the finest level or the lowLevel 
 * according to the frame rate.
 */
int Prog_indexed_tri_set::GetLevelToRender() const
{

  bool useLowLevel = false;
  useLowLevel =  m_lowLevel != -1 && 
        m_currentLevel > m_lowLevel && 
        m_executionCoordinator->LowerLevel() &&
        m_executionCoordinator->IsDraggingLocked();

  int level = 0;
  if ( useLowLevel ) {
    level = m_lowLevel;
  } else {
    //currentLevel = m_coordIndicesBuffers.size() - 1;
    level = m_currentLevel;
  }

  // overwrite whatever chosen with the specified LOD
  if ( m_lod != -1 ) {
    if ( m_lodIndex2Level.size() == 0 ) {
      level = -1;
    } else if ( m_lod >= m_lodIndex2Level.size() ) {
      level = m_lodIndex2Level[m_lodIndex2Level.size() - 1];
    } else if ( m_lod < 0 ) {
      level = m_lodIndex2Level[0];
    } else {
      level = m_lodIndex2Level[m_lod];
    }
  }

  return level;
}

/**
 * Delete the geometry data and reset the node status to the state in
 * the end of creation
 */
void Prog_indexed_tri_set::delete_data()
{
  // decrease the number of polygons being rendered
  m_executionCoordinator->AddPolygons(-1*m_currNumPolygons);

  // update memory allocation status
  for (int i = 0 ; i < m_bytes.size() ; i++ ) {
    m_executionCoordinator->AddMemory(-1*m_bytes[i]);
  }
  m_executionCoordinator->AddMemory(-1*m_memoryBase);
  m_bytes.clear();
  m_memoryBase = 0;

  // delete all relevant data and intialize members
  DestroyData();
  Indexed_face_set::DestroyData();
  m_currentLevel = -1;
    m_lod = -1;
    m_lodIndex2Level.clear();
  m_keepLevel.clear();
  m_currNumPolygons = 0;
  m_numPolygonsArray.clear();

}
