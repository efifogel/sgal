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
// $Revision: 1311 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_BIT_MASK_HPP
#define SGAL_BIT_MASK_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Bit_mask {
private:
  Double_word m_mask;

public:
  inline Bit_mask() { m_mask = 0; };
  inline Bit_mask(Boolean initial_val) { set(initial_val); }
  inline Bit_mask(const Bit_mask & bm) { set(bm); }

  inline Bit_mask & on_bit(int bit)
  { m_mask |= ((Double_word)1 << bit); return * this; }

  inline Bit_mask & off_bit(int bit)
  { m_mask &= ~((Double_word)1 << bit); return * this; }

  inline Bit_mask & and_not_bit(int bit) { return off_bit(bit); }

  inline Boolean get_bit(int bit)
  { return (m_mask & ((Double_word)1 << bit)) != 0; }

  inline void on() { m_mask = ~(Double_word)0; }

  inline void off() { m_mask = 0; }

  inline Boolean not_zero() { return m_mask != 0; }

  inline void set(bool true_false)
  { m_mask = (true_false) ? ~(Double_word)0 : 0; }

  // void Write(EOutput * out);

  inline Bit_mask & set(const Bit_mask & bm)
  { m_mask = bm.m_mask; return * this; }

  inline Double_word get_mask() const { return m_mask; }
    
  inline void set_mask(const Double_word mask) { m_mask = mask; }
    
  inline Bit_mask & and_equal(const Bit_mask & bm)
  { m_mask &= bm.m_mask; return * this; }

  inline Bit_mask & and_not_equal(const Bit_mask & bm)
  { m_mask &= ~bm.m_mask; return * this; }

  inline Bit_mask & or_equal(const Bit_mask & bm)
  { m_mask |= bm.m_mask; return * this; }

  inline Bit_mask & or_not_equal(const Bit_mask & bm)
  { m_mask |= ~bm.m_mask; return * this; }

  inline Bit_mask & xor_equal(const Bit_mask & bm)
  { m_mask ^= bm.m_mask; return * this; }

  inline Bit_mask & not_equal(const Bit_mask & bm)
  { m_mask = ~bm.m_mask; return * this; }

public:
  inline int operator==(const Bit_mask & bm) { return m_mask == bm.m_mask; }
};

SGAL_END_NAMESPACE

#endif
