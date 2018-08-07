// Copyright (c) 2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_OPTION_MAPPER_HPP
#define SGAL_OPTION_MAPPER_HPP

#include <map>

#include <boost/icl/interval_set.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Option_mapper {
public:
  //! A pre-defined code for the invalid option:
  enum Base_code { INVALID = 0, START_CODE };

  typedef std::map<String, size_t>                              Map;

  /*! Construct.
   */
  Option_mapper();

  /*! Destruct.
   */
  virtual ~Option_mapper();

  /*! Obtain the number of options.
   */
  size_t options_size() const;

  /*! Find the code of an option.
   * \param[in] option the option
   * \return the code.
   */
  size_t find_code(const String& option) const;

  /*! Find the option of a code.
   * \param[in] code the code.
   * \return the option.
   */
  const String& find_option(size_t code) const;

  /*! Obtain the options container.
   */
  const Map& get_options();

  /*! Register a option.
   * \param[in] the new option.
   * \return the code allocated for the external option.
   */
  size_t register_option(const String& option);

  /*! Unregister a option.
   * \param[in] the option to remove.
   */
  void unregister_option(const String& option);

protected:
  /*! Set the options.
   * \pre options must contain the INVALID code.
   */
  void set_options(Map&& options);

  //! The options.
  Map m_options;

private:
  typedef boost::icl::interval_set<size_t>      set_t;
  typedef set_t::interval_type                  ival;

  //! Free codes arranged in a set of intervals.
  set_t m_free_codes;
};

SGAL_END_NAMESPACE

#endif
