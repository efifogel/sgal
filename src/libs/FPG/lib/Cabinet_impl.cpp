// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_triangle_indices.hpp"

#include "FPG/Cabinet.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief processes change of structure.
void Cabinet::structure_changed(const Field_info* field_info)
{
  clear_coord_array();
  clear_facet_coord_indices();
  field_changed(field_info);
}

//! \brief cleans (generate) the coordinate array.
void Cabinet::clean_coords()
{
  m_dirty_coord_array = false;
  coord_content_changed(get_field_info(COORD_ARRAY));
}

void Cabinet::clean_facet_coord_indices()
{
  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;
}

SGAL_END_NAMESPACE
