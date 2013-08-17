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
// $Id: $
// $Revision: 1313 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

/*! The indices of the 6 faces */
enum {MINX = 0, MINY, MINZ, MAXX, MAXY, MAXZ, NUM_PROJECTIONS};

unsigned int s_mask[] = {
  0x1 << MINX, 0x1 << MINY, 0x1 << MINZ, 0x1 << MAXX, 0x1 << MAXY, 0x1 << MAXZ
};

#if defined(_MSC_VER)
#define DIR_SEP '\\'
#else
#define DIR_SEP '/'
#endif

void print_help(const char * prog_name)
{
}

void get_next_pm_vertex_ids(unsigned int cur_pm_id,
                            unsigned int cur_vertex_id,
                            unsigned int * pm_id,
                            unsigned int * vertex_id)
{
  if (cur_pm_id < 3) {
    switch(cur_vertex_id) {
     case 0: *pm_id = (cur_pm_id + 1) % 3; *vertex_id = 0; return;
     case 1: *pm_id = (cur_pm_id + 2) % 3; *vertex_id = 2; return;
     case 2: *pm_id = ((cur_pm_id + 2) % 3) + 3; *vertex_id = 0; return;
     case 3: *pm_id = ((cur_pm_id + 1) % 3) + 3; *vertex_id = 2; return;
    } 
  } else {
    switch(cur_vertex_id) {
     case 0: *pm_id = (cur_pm_id + 2) % 3; *vertex_id = 1; return;
     case 1: *pm_id = (cur_pm_id + 1) % 3; *vertex_id = 3; return;
     case 2: *pm_id = ((cur_pm_id + 1) % 3) + 3; *vertex_id = 1; return;
     case 3: *pm_id = ((cur_pm_id + 2) % 3) + 3; *vertex_id = 3; return;
    }
  }
}

unsigned int get_index(unsigned int mask, unsigned int id)
{
  // unsigned int i = (id + 0) % 3;
  unsigned int j = (id + 1) % 3;
  unsigned int k = (id + 2) % 3;

  if (id < 3) std::swap(j,k);
  return ((mask & s_mask[j]) ? 0 : (mask & s_mask[j+3]) ? 2 : 4) +
    ((mask & s_mask[k]) ? 0 : (mask & s_mask[k+3]) ? 1 : 4);
}

int main(int argc, char * argv[])
{
  int c;
  unsigned int mask;
  char * prog_name = strrchr(argv[0], DIR_SEP);
  prog_name = (prog_name) ? prog_name+1 : argv[0];
  unsigned int id;

  while ((c = getopt(argc, argv, "hi:m:")) != EOF) {
    switch (c) {
     case 'h': print_help(prog_name); return 0;
     case 'm': mask = strtoul(optarg, 0, 0); break;
     case 'i': id = atoi(optarg); break;
    }
  }

  typedef std::pair<unsigned int, unsigned int> Mask_id_pair;
  Mask_id_pair table[24];
  table[0] = Mask_id_pair(0x7,0);
  table[1] = Mask_id_pair(0x7,1);
  table[2] = Mask_id_pair(0x7,2);

  table[3] = Mask_id_pair(0x23,0);
  table[4] = Mask_id_pair(0x23,1);
  table[5] = Mask_id_pair(0x23,5);

  table[6] = Mask_id_pair(0x15,0);
  table[7] = Mask_id_pair(0x15,2);
  table[8] = Mask_id_pair(0x15,4);

  table[9] = Mask_id_pair(0x31,0);
  table[10] = Mask_id_pair(0x31,4);
  table[11] = Mask_id_pair(0x31,5);

  table[12] = Mask_id_pair(0x38,3);
  table[13] = Mask_id_pair(0x38,4);
  table[14] = Mask_id_pair(0x38,5);
  
  table[15] = Mask_id_pair(0x1c,2);
  table[16] = Mask_id_pair(0x1c,3);
  table[17] = Mask_id_pair(0x1c,4);

  table[18] = Mask_id_pair(0x2a,1);
  table[19] = Mask_id_pair(0x2a,3);
  table[20] = Mask_id_pair(0x2a,5);

  table[21] = Mask_id_pair(0xe,1);
  table[22] = Mask_id_pair(0xe,2);
  table[23] = Mask_id_pair(0xe,3);

  unsigned int i;
  for (i = 0; i < 24; i++) {
    std::cout << i << ": 0x" << std::hex
              << table[i].first << "," << table[i].second
              << " -> " << get_index(table[i].first, table[i].second)
              << std::endl;
  }
  
  std::cout << std::endl;

  for (i = 0; i < 6; i++) {
    unsigned int j;
    for (j = 0; j < 4; j++) {
      unsigned int new_i, new_j;
      get_next_pm_vertex_ids(i, j, &new_i, &new_j);
      std::cout << "(" << i << "," << j << ") -> "
                << "(" << new_i << "," << new_j << ")"
                << std::endl;
    }
  }
  
  return 0;
}
