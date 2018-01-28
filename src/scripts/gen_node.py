#!/usr/bin/python

from __future__ import print_function
import os.path
import argparse
import json
import shlex
from sets import Set

def is_valid_file(parser, arg):
  if not os.path.exists(arg):
    parser.error("The file %s does not exist!" % arg)
  else:
    return open(arg, 'r')  # return an open file handle

def readable_dir(prospective_dir):
  if not os.path.isdir(prospective_dir):
    parser.error("readable_dir:{0} is not a valid path".format(prospective_dir))
  if os.access(prospective_dir, os.R_OK):
    return prospective_dir
  else:
    parser.error("readable_dir:{0} is not a readable dir".format(prospective_dir))

def find(name, path):
  for root, dirs, files in os.walk(path):
    if name in files:
      return os.path.join(root, name)

copyright_text = '''// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.
'''

indent = 0
delta = 2

# Map from C++ types to feild types
s_field_type = {
  # Single field
  'Boolean': 'SF_bool',
  'Float': 'SF_float',
  'Uint': 'SF_uint',
  'Int32': 'SF_int32',
  'Scene_time': 'SF_time',
  'Vector2f': 'SF_vector2f',
  'Vector3f': 'SF_vector3f',
  'Vector3f': 'SF_color',
  'Vector4f': 'SF_vector4f',
  'Rotation': 'SF_rotation',
  'Bounding_sphere': 'SF_bounding_sphere',
  'String': 'SF_string',
  'Shared_container': 'SF_shared_container',
  # Multi field
  'Boolean_array': 'MF_bool',
  'Float_array': 'MF_float',
  'Uint_array': 'MF_uint',
  'Int32_array': 'MF_int32',
  'Scene_time_array': 'MF_time',
  'Vector2f_array': 'MF_vector2f',
  'Vector3f_array': 'MF_vector3f',
  'Vector3f_array': 'MF_color',
  'Vector4f_array': 'MF_vector4f',
  'Rotation_array': 'MF_rotation',
  'Bounding_sphere_array': 'MF_bounding_sphere',
  'String_array': 'MF_string',
  'Shared_container_array': 'MF_shared_container'
}

# Map from C++ types to handle-function types
s_field_handle_type = {
  # Single field handle functions
  'Boolean': 'Boolean_handle_function',
  'Float': 'Float_handle_function',
  'Uint': 'Uint_handle_function',
  'Int32': 'Int32_handle_function',
  'Scene_time': 'Scene_time_handle_function',
  'Vector2f': 'Vector2f_handle_function',
  'Vector3f': 'Vector3f_handle_function',
  'Vector3f': 'Vector3f_handle_function',
  'Vector4f': 'Vector4f_handle_function',
  'Rotation': 'Rotation_handle_function',
  'Bounding_sphere': ' Bounding_sphere_handle_function',
  'String': 'String_handle_function',
  'Shared_container': 'Shared_container_handle_function',
  # Multi field handle functions
  'Boolean_array': 'Boolean_array_handle_function',
  'Float_array': 'Float_array_handle_function',
  'Uint_array': 'Uint_array_handle_function',
  'Int32_array': 'Int32_array_handle_function',
  'Scene_time_array': 'Scene_time_array_handle_function',
  'Vector2f_array': 'Vector2f_array_handle_function',
  'Vector3f_array': 'Vector3f_array_handle_function',
  'Vector3f_array': 'Vector3f_array_handle_function',
  'Vector4f_array': 'Vector4f_array_handle_function',
  'Rotation_array': 'Rotation_array_handle_function',
  'Bounding_sphere_array': 'Bounding_sphere_array_handle_function',
  'String_array': 'String_array_handle_function',
  'Shared_container_array': 'Shared_container_array_handle_function'
}

# Map from C++ types to parameter passing methods
s_field_passing_method = {
  # Single field handle functions
  'Boolean': 'value',
  'Float': 'value',
  'Uint': 'value',
  'Int32': 'value',
  'Scene_time': 'value',
  'Vector2f': 'reference',
  'Vector3f': 'reference',
  'Vector3f': 'reference',
  'Vector4f': 'reference',
  'Rotation': 'reference',
  'Bounding_sphere': 'reference',
  'String': 'reference',
  'Shared_container': 'value',
  # Multi field handle functions
  'Boolean_array': 'reference',
  'Float_array': 'reference',
  'Uint_array': 'reference',
  'Int32_array': 'reference',
  'Scene_time_array': 'reference',
  'Vector2f_array': 'reference',
  'Vector3f_array': 'reference',
  'Vector3f_array': 'reference',
  'Vector4f_array': 'reference',
  'Rotation_array': 'reference',
  'Bounding_sphere_array': 'reference',
  'String_array': 'reference',
  'Shared_container_array': 'reference'
}

# Map from C++ type to a Boolean indicating whether two getter functions
# should be supported: a plain getter and a getter with a const qualified
# return type.
s_field_return_qualification = {
  # Single field handle functions
  'Boolean': False,
  'Float': False,
  'Uint': False,
  'Int32': False,
  'Scene_time': False,
  'Vector2f': True,
  'Vector3f': True,
  'Vector3f': True,
  'Vector4f': True,
  'Rotation': True,
  'Bounding_sphere': True,
  'String': True,
  'Shared_container': True,
  # Multi field handle functions
  'Boolean_array': True,
  'Float_array': True,
  'Uint_array': True,
  'Int32_array': True,
  'Scene_time_array': True,
  'Vector2f_array': True,
  'Vector3f_array': True,
  'Vector3f_array': True,
  'Vector4f_array': True,
  'Rotation_array': True,
  'Bounding_sphere_array': True,
  'String_array': True,
  'Shared_container_array': True
}

# Map for C++ types to element types.
s_field_element_type = {
  # Single field handle functions
  'Boolean': 'string',
  'Float': 'string',
  'Uint': 'string',
  'Int32': 'string',
  'Scene_time': 'string',
  'Vector2f': 'string',
  'Vector3f': 'string',
  'Vector3f': 'string',
  'Vector4f': 'string',
  'Rotation': 'string',
  'Bounding_sphere': 'string',
  'String': 'string',
  'Shared_container': 'container',
  # Multi field handle functions
  'Boolean_array': 'string',
  'Float_array': 'string',
  'Uint_array': 'string',
  'Int32_array': 'string',
  'Scene_time_array': 'string',
  'Vector2f_array': 'string',
  'Vector3f_array': 'string',
  'Vector3f_array': 'string',
  'Vector4f_array': 'string',
  'Rotation_array': 'string',
  'Bounding_sphere_array': 'string',
  'String_array': 'multi-string',
  'Shared_container_array': 'multi-container'
}

# Map from a C++ type to a tuple:
# 1. Indicating whether lexical cast should be performed.
# 2. Indicating whether a dedicated lexical caster header should be included
s_field_lexical_cast = {
  # Single field handle functions
  'Boolean': [ True, True ],
  'Float': [ True, False ],
  'Uint': [ True, False ],
  'Int32': [ True, False ],
  'Scene_time': [ True, False ],
  'Vector2f': [ True, True ],
  'Vector3f': [ True, True ],
  'Vector3f': [ True, True ],
  'Vector4f': [ True, True ],
  'Rotation': [ True, True ],
  'Bounding_sphere': [ True, True ],
  'String': [ False, False ],
  'Shared_container': [ False, False ],
  # Multi field handle functions
  'Boolean_array': [ True, True ],
  'Float_array': [ True, False ],
  'Uint_array': [ True, False ],
  'Int32_array': [ True, False ],
  'Scene_time_array': [ True, False ],
  'Vector2f_array': [ True, True ],
  'Vector3f_array': [ True, True ],
  'Vector3f_array': [ True, True ],
  'Vector4f_array': [ True, True ],
  'Rotation_array': [ True, True ],
  'Bounding_sphere_array': [ True, True ],
  'String_array': [ False, False ],
  'Shared_container_array': [ False, False ]
}

#! Determines whether the fileds contains an array field
def has_array_type(fields):
  for field in fields[:]:
    type = field['type']
    if type.endswith('_array'):
      return True
  return False

def get_single_type(type):
  if type.endswith('_array'):
    return type[:-6]
  else:
    return type

#! Determines whether the fileds contains a shared-container field
def has_shared_type(fields):
  for field in fields[:]:
    type = field['type']
    if type.startswith('Shared_'):
      return True
  return False

# Obtain the general field type
def get_general_type(type):
  if type.startswith('Shared_'):
    if type.endswith('_array'):
      return 'Shared_container_array'
    else:
      return 'Shared_container'
  else:
    return type

#! Increase the indentation level.
def increase_indent():
  global indent
  indent += delta

#! Decrease the indentation level.
def decrease_indent():
  global indent
  indent -= delta

#! Print a single indented line.
def print_line(out, line, eol=True, inc=False, dec=False):
  if (dec): decrease_indent()
  print ("%s%s" % (indent * ' ', line), file=out, end='\n' if eol else '')
  if (inc): increase_indent()

#! Print a call to a function that might exceed 80 characters.
def print_call(out, statement):
  print_line(out, statement)

#! Print a block of lines equally indented.
def print_block(out, block):
  lines = block.expandtabs(delta).splitlines()
  for line in lines[:]:
    print_line(out, line)

#! Print an empty line.
def print_empty_line(out):
  print_line(out, '')

#! Print a block statements.
def print_group(out, desc, fnc, *args):
  print_block(out, "/// \\name %s" % desc)
  print_line(out, "//@{")
  if not args:
    fnc(out)
  else:
    fnc(out, args)
  print_line(out, "//@}")
  print_empty_line(out)

#! Print enumerations.
def print_field_enumeration(out, derived_class_name, fields):
  print_line(out, "enum {", inc=True)
  print_line(out, "FIRST = %s::LAST - 1," % derived_class_name)
  for field in fields[:]:
    print_line(out, field['name'].upper() + ',')
  print_line(out, "LAST")
  print_line(out, "};", dec=True)
  print_empty_line(out)

#! Print forward declarations
def print_forward_declarations(out, fields):
  print_line(out, 'class Container_proto;')
  print_line(out, 'class Element;')
  for field in fields[:]:
    type = field['type']
    if not type.startswith('Shared_'):
      continue
    main_type = get_single_type(type[7:])
    print_line(out, 'class {};'.format(main_type.capitalize()))
  print_empty_line(out)

# Print field typedefs
def print_typedefs(out, fields):
  hst = has_shared_type(fields)
  if (hst):
    for field in fields[:]:
      type = field['type']
      if not type.startswith('Shared_'):
        continue
      main_type = type[7:]
      if main_type.endswith('_array'):
        main_type = main_type[:-6]
      statement = 'typedef boost::shared_ptr<{}>'.format(main_type.capitalize())
      statement = '{:<46}Shared_{};'.format(statement, main_type)
      print_line(out, statement)
    print_empty_line(out)
    # Print declarations of vector arrays:
    if has_array_type(fields):
      for field in fields[:]:
        type = field['type']
        if not type.startswith('Shared_'):
          continue
        main_type = type[7:]
        if main_type.endswith('_array'):
          main_type = main_type[:-6]
          statement = 'typedef std::vector<Shared_{}>'.format(main_type)
          statement = '{:<46}Shared_{}_array;'.format(statement, main_type)
          print_line(out, statement)
      print_empty_line(out)

# Constructor description.
constructor_desc = '''/*! Construct from prototype.
 * \\param proto indicates whether a prototype should be constructed.
 */
'''

#! Print the constructor declaration.
def print_constructor_declaration(out, name):
  print_block(out, constructor_desc)
  print_line(out, "%s(Boolean proto = false);" % name)
  print_empty_line(out)

#! Print the destructor declaration.
def print_destructor_declaration(out, name):
  print_line(out, "/*! Destruct. */")
  print_line(out, "virtual ~%s();" % name)
  print_empty_line(out)

# Prototype description.
prototype_desc = '''/*! Construct the prototype.
 * \\return the prototype.
 */
'''

#! Print prototype() declaration.
def print_prototype_declaration(out, name):
  print_block(out, prototype_desc)
  print_line(out, "static %s* prototype();" % name)
  print_empty_line(out)

# Clone description.
clone_desc = '''/*! Clone.
 * \\return the cloned container.
 */
'''

#! Print clone() declaration.
def print_clone_declaration(out):
  print_block(out, clone_desc)
  print_line(out, "virtual Container* clone();")
  print_empty_line(out)

#! Print init_prototype() declaration.
def print_init_prototype_declaration(out):
  print_line(out, "/*! Initialize the node prototype. */")
  print_line(out, "virtual void init_prototype();")

def print_delete_prototype_declaration(out):
  print_line(out, "/*! Delete the node prototype. */")
  print_line(out, "virtual void delete_prototype();")

# get_prototype() description.
get_prototype_desc = '''/*! Obtain the node prototype.
 * \\return the node prototype.
 */
'''

#! Print get_prototype() declaration.
def print_get_prototype_declaration(out):
  print_block(out, get_prototype_desc);
  print_line(out, "virtual Container_proto* get_prototype();")

#! Print prototype handling declarations.
def print_prototype_handling_declarations(out):
  print_init_prototype_declaration(out)
  print_empty_line(out);
  print_delete_prototype_declaration(out)
  print_empty_line(out);
  print_get_prototype_declaration(out)

# set_attribute() description
get_prototype_desc = '''/*! Set the attributes of the transform.
 * \\param[in] elem contains lists of attribute name and value pairs.
 */
'''

#! Print attribute handling declarations.
def print_attribute_handlin_declaration(out):
  print_line(out, "virtual void set_attributes(Element* elem);")

#! Print field handling declarations
def print_field_handlin_declaration(out, args):
  fields = args[0]
  for field in fields[:]:
    type = field['type']
    name = field['name']
    print_line(out, "%s* %s_handle(const Field_info*) { return &m_%s; }" % (type, name, name));

#! Print setter declaration
def print_field_setter_declaration(out, field):
  type = field['type']
  name = field['name']
  type = field['type']
  general_type = get_general_type(type)
  pass_method = s_field_passing_method[general_type]
  desc = field['desc']
  print_line(out, "/*! Set the %s. */" % desc)
  if pass_method == 'value':
    print_line(out, "void set_%s(%s %s);" % (name, type, name));
  elif pass_method == 'reference':
    print_line(out, "void set_%s(const %s& %s);" % (name, type, name));
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

#! Print setter declaration
def print_field_getter_declaration(out, field):
  type = field['type']
  name = field['name']
  type = field['type']
  general_type = get_general_type(type)
  pass_method = s_field_passing_method[general_type]
  compound = s_field_return_qualification[general_type]
  desc = field['desc']
  print_line(out, "/*! Obtain the %s. */" % desc)
  if pass_method == 'value':
    if not compound:
      print_line(out, "%s get_%s() const;" % (type, name))
    else:
      print_line(out, "const %s get_%s() const;" % (type, name))
      print_line(out, "%s get_%s();" % (type, name))
  elif pass_method == 'reference':
    print_line(out, "const %s& get_%s() const;" % (type, name))
    print_line(out, "%s& get_%s();" % (type, name))
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

#! Print setter definition
def print_field_setter_definition(out, class_name, field):
  type = field['type']
  name = field['name']
  type = field['type']
  general_type = get_general_type(type)
  pass_method = s_field_passing_method[general_type]
  desc = field['desc']
  print_line(out, "//! \\brief sets the %s." % desc)
  if pass_method == 'value':
    print_line(out, "inline void %s::set_%s(%s %s)" % (class_name, name, type, name))
  elif pass_method == 'reference':
    print_line(out, "inline void %s::set_%s(const %s& %s)" % (class_name, name, type, name))
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)
  print_line(out , "{ m_%s = %s; }" % (name, name))

#! Print setter definition
def print_field_getter_definition(out, class_name, field):
  type = field['type']
  name = field['name']
  general_type = get_general_type(type)
  pass_method = s_field_passing_method[general_type]
  compound = s_field_return_qualification[general_type]
  desc = field['desc']
  ext_type = type
  if type.startswith('Shared_'):
    ext_type = class_name + '::' + type
  if pass_method == 'value':
    if not compound:
      print_line(out, '//! \\brief obtains the {}.'.format(desc))
      print_line(out, 'inline {} {}::get_{}() const'.format(ext_type, class_name, name))
      print_line(out, '{{ return m_{}; }}'.format(name))
    else:
      print_line(out, '//! \\brief obtains the {}.'.format(desc))
      print_line(out, 'inline const {} {}::get_{}() const'.format(ext_type, class_name, name))
      print_line(out, '{{ return m_{}; }}'.format(name))
      print_empty_line(out)
      print_line(out, '//! \\brief obtains the {}.'.format(desc))
      print_line(out, 'inline {} {}::get_{}()'.format(ext_type, class_name, name))
      print_line(out, '{{ return m_{}; }}'.format(name))
  elif pass_method == 'reference':
    print_line(out, '//! \\brief obtains the {}.'.format(desc))
    print_line(out, 'inline const {}& {}::get_{}() const'.format(ext_type, class_name, name))
    print_line(out, '{{ return m_{}; }}'.format(name))
    print_empty_line(out)
    print_line(out, '//! \\brief obtains the {}.'.format(desc))
    print_line(out, 'inline {}& {}::get_{}()'.format(ext_type, class_name, name))
    print_line(out, '{{ return m_{}; }}'.format(name))
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

#! Handle field setter & getter declarations
def print_field_setter_getter_declarations(out, field, last=False):
  print_field_setter_declaration(out, field)
  print_empty_line(out)
  print_field_getter_declaration(out, field)
  if not last: print_empty_line(out)

#! Print getters and setters.
def print_field_getters_setters_declarations(out, args):
  fields = args[0]
  for field in fields[:-1]:
    print_field_setter_getter_declarations(out, field)
  field = fields[-1]
  print_field_setter_getter_declarations(out, field, last=True)

#! Print the field data members.
def print_data_members(out, fields):
  for field in fields[:]:
    name = field['name']
    type = field['type']
    desc = field['desc']
    print_line(out, "/*! The %s. */" % desc)
    print_line(out, "%s m_%s;" % (type, name))
    print_empty_line(out)

def print_get_tag_declaration(out):
  print_line(out, '/*! Obtain the tag (type) of the container. */')
  print_line(out, 'virtual const String& get_tag() const;')
  print_empty_line(out)

#! Print the field static members
def print_static_member_declarations(out, fields):
  print_line(out, "// Default values:")
  for field in fields[:]:
    type = field['type']
    if type.startswith('Shared_'):
      continue
    name = field['name']
    desc = field['desc']
    print_line(out, "static const %s s_def_%s;" % (type, name))

def print_tag_data_member(out):
  print_line(out, "/*! The tag that identifies this container type. */")
  print_line(out, "static const String s_tag;")
  print_empty_line(out)

def print_prototype_data_member(out):
  print_line(out, "/*! The container prototype. */")
  print_line(out, "static Container_proto* s_prototype;");
  print_empty_line(out)

def print_prototype_definition(out, class_name):
  print_line(out, "//! \\brief constructs the prototype.")
  print_line(out, "inline %s* %s::prototype() { return new %s(true); }" %
             (class_name, class_name, class_name))

def print_clone_definition(out, class_name):
  print_line(out, "//! \\brief clones.")
  print_line(out, "inline Container* %s::clone() { return new %s(); }" %
             (class_name, class_name))

def print_field_setter_getter_definition(out, class_name, field, last=False):
  print_field_setter_definition(out, class_name, field)
  print_empty_line(out)
  print_field_getter_definition(out, class_name, field)
  if not last: print_empty_line(out)

#! Print declarations of field setters and getters:
def print_field_getters_setters_definitions(out, class_name, fields):
  for field in fields[:-1]:
    print_field_setter_getter_definition(out, class_name, field)
  field = fields[-1]
  print_field_setter_getter_definition(out, class_name, field, last=True)

#! Print get_tag() definition.
def print_get_tag_definition(out, class_name):
  print_line(out, "//! \\brief obtains the tag (type) of the container.")
  print_line(out, "inline const String& %s::get_tag() const { return s_tag; }" % class_name)

#! Print the include directives in the hpp
def print_hpp_include_directives(out, derived_class_name):
  hat = has_array_type(fields)
  print_line(out, '#include <string>')
  if (hat):
    print_line(out, '#include <vector>')
  print_empty_line(out)
  if has_shared_type(fields):
    print_line(out, '#include <boost/shared_ptr.hpp>')
    print_empty_line(out)
  print_line(out, '#include \"SGAL/basic.hpp\"')
  print_line(out, '#include \"SGAL/Types.hpp\"')
  if hat:
    print_line(out, '#include \"SGAL/Array_types.hpp\"')
  print_line(out, '#include \"SGAL/{}.hpp\"'.format(derived_class_name))
  print_empty_line(out)
  print_line(out, '#include \"FPG/basic.hpp\"')

#! Generate the .hpp file
#! \param name
#! \param derived_class_name
#! \param fields
def generate_hpp(output_path, class_name, derived_class_name, fields):
  hpp_filename = os.path.join(output_path, class_name + ".hpp")
  with open(hpp_filename, 'w') as out:
    print_line(out, copyright_text)
    print_line(out, "// Generated by " + os.path.basename(__file__))
    print_empty_line(out)
    print_line(out, "#ifndef FPG_%s_HPP" % class_name.upper())
    print_line(out, "#define FPG_%s_HPP" % class_name.upper())
    print_empty_line(out)
    print_hpp_include_directives(out, derived_class_name)
    print_empty_line(out)
    print_line(out, "SGAL_BEGIN_NAMESPACE")
    print_empty_line(out)
    print_forward_declarations(out, fields)

    print_line(out, "class SGAL_FPG_DECL %s : public %s {" % (class_name, derived_class_name))
    print_line(out, "public:")
    increase_indent()

    print_field_enumeration(out, derived_class_name, fields)	# enumerations
    print_typedefs(out, fields)					# typedefs

    # Print misc. member function declarations:
    print_constructor_declaration(out, class_name)
    print_destructor_declaration(out, class_name)
    print_prototype_declaration(out, class_name)
    print_clone_declaration(out)

    # Print prototype handling declarations:
    print_group(out, "Protoype handlers",
                print_prototype_handling_declarations)

    # Print attribute handling declarations:
    print_group(out, "Attribute handlers",
                print_attribute_handlin_declaration)

    # Print field handling declarations:
    print_group(out, "Field handlers",
                print_field_handlin_declaration, fields)

    # Print declarations of field setters and getters:
    print_group(out, "getters & setters",
                print_field_getters_setters_declarations, fields)

    decrease_indent()
    print_line(out, "protected:")
    increase_indent()

    print_data_members(out, fields)
    print_get_tag_declaration(out)

    decrease_indent()
    print_line(out, "private:")
    increase_indent()

    print_tag_data_member(out)
    print_prototype_data_member(out)
    print_static_member_declarations(out, fields)

    decrease_indent()
    print_line(out, "};")
    print_empty_line(out)

    print_prototype_definition(out, class_name)
    print_empty_line(out)
    print_clone_definition(out, class_name)
    print_empty_line(out)

    # Print declarations of field setters and getters:
    print_field_getters_setters_definitions(out, class_name, fields)
    print_empty_line(out)
    print_get_tag_definition(out, class_name)

    print_empty_line(out)
    print_line(out, "SGAL_END_NAMESPACE")
    print_empty_line(out)
    print_line(out, "#endif")

def print_cpp_include_directives(out, fields):
  include_sstream = False
  include_iterator = False
  include_algorithm = False
  include_std = False
  include_boost_algorithm_string = False
  include_boost_lexical_case = False
  include_boost = False

  field_names_shared_container = set()
  field_names_lexical_cast = set()
  for field in fields[:]:
    type = field['type']
    if type == 'String':
      include_boost_algorithm_string = True
    else:
      single_type = get_single_type(type)
      general_type = get_general_type(type)
      lexical_cast = s_field_lexical_cast[general_type]
      if lexical_cast[0]:
        if not lexical_cast[1]:
          include_boost_lexical_case = True
        else:
          single_type = get_single_type(type)
          field_names_lexical_cast.add(single_type.lower())
          if type.endswith('_array'):
            include_sstream = True
            include_iterator = True
            include_algorithm = True

      if single_type.startswith('Shared_'):
        field_class_type = single_type[7:].capitalize()
        if  'Container' != field_class_type:
          field_names_shared_container.add(field_class_type)

  if (include_sstream):
    print_line(out, '#include <sstream>')
    include_std = True
  if (include_iterator):
    print_line(out, '#include <iterator>')
    include_std = True
  if (include_algorithm):
    print_line(out, '#include <algorithm>')
    include_std = True
  if (include_std):
    print_empty_line(out)

  if (include_boost_algorithm_string):
    print_line(out, '#include <boost/algorithm/string.hpp>')
    include_boost = True
  if (include_boost_lexical_case):
    print_line(out, '#include <boost/lexical_cast.hpp>')
    include_boost = True
  if (include_boost):
    print_empty_line(out)

  print_line(out, "#include \"SGAL/Element.hpp\"")
  print_line(out, "#include \"SGAL/Container_proto.hpp\"")
  print_line(out, "#include \"SGAL/Field_infos.hpp\"")
  print_line(out, "#include \"SGAL/Field.hpp\"")
  for item in set(field_names_lexical_cast):
    print_line(out, '#include \"SGAL/lexical_cast_{}.hpp\"'.format(item))
  print_empty_line(out)
  print_line(out, '#include \"FPG/{}.hpp\"'.format(class_name))
  for item in set(field_names_shared_container):
    print_line(out, '#include \"FPG/{}.hpp\"'.format(item))

def print_static_member_definitions(out, class_name):
  print_line(out, "// Default values:")
  for field in fields[:]:
    type = field['type']
    if type.startswith('Shared_'):
      continue
    name = field['name']
    default_value = field['default-value']
    if default_value:
      print_line(out, 'const {} {}::s_def_{}({});'.format(type, class_name, name, default_value))
    else:
      print_line(out, 'const {} {}::s_def_{};'.format(type, class_name, name))
  print_empty_line(out)

#! Print constructor definition.
def print_constructor_definition(out, class_name, derived_class_name, fields):
  print_line(out, '//! \\brief construct from prototype.')
  initializable_fields = []
  for field in fields[:]:
    type = field['type']
    if not type.startswith('Shared_'):
      initializable_fields.append(field)

  if 0 == len(initializable_fields):
    print_line(out, '{}::{}(Boolean proto) : {}(proto) {{}}'.format(class_name, class_name, derived_class_name))
  else:
    print_line(out, '{}::{}(Boolean proto) :'.format(class_name, class_name),
               inc=True)
    print_line(out, '{}(proto),'.format(derived_class_name))
    for field in initializable_fields[:-1]:
      name = field['name']
      print_line(out, 'm_{}(s_def_{}),'.format(name, name))
    field = initializable_fields[-1]
    name = field['name']
    print_line(out, 'm_{}(s_def_{})'.format(name, name))
    print_line(out, "{}", dec=True)
  print_empty_line(out)

#! Print destructor definition.
def print_destructor_definition(out, class_name, fields):
  print_line(out, "//! \\brief destruct.")
  field_names = []
  for field in fields[:]:
    name = field['name']
    type = field['type']
    if type.endswith('_array'):
      field_names.append(name)
  if 0 == len(field_names):
    print_line(out, "%s::~%s() {}" % (class_name, class_name))
  else:
    print_line(out, "%s::~%s()" % (class_name, class_name))
    print_line(out, "{", inc=True)
    for field_name in field_names[:]:
      print_line(out, "m_%s.clear();" % field_name)
    print_line(out, "}", dec=True)
  print_empty_line(out)

def print_init_prototype_definition(out, class_name, derived_class_name, fields):
  print_line(out, "//! \\brief initilalizes the container prototype.")
  print_line(out, "void %s::init_prototype()" % class_name)
  print_line(out, "{", inc=True)

  print_line(out, "if (s_prototype) return;")
  print_line(out, "s_prototype = new Container_proto(%s::get_prototype());" %
             derived_class_name)
  print_empty_line(out)
  print_line(out, "// Add the field-info records to the prototype:")
  # print_line(out, "auto exec_func = static_cast<Execution_function>(&%s::structure_changed);" % class_name)
  print_line(out, 'Execution_function exec_func;')
  for field in fields[:]:
    name = field['name']
    type = field['type']
    rule = field['rule']
    default_value = field['default-value']
    execution_function = field['execution-function']
    tmp = name.replace('_', ' ').title().replace(' ', '')
    field_name = tmp[:1].lower() + tmp[1:]
    general_type = get_general_type(type)
    field_handle_type = s_field_handle_type[general_type]
    print_empty_line(out)
    print_line(out, "// %s" % field_name)
    if (execution_function):
      print_line(out, 'exec_func = static_cast<Execution_function>(&{});'.format(execution_function))
    print_line(out, 'auto {}_func = reinterpret_cast<{}>(&{}::{}_handle);'.format(name, field_handle_type, class_name, name))
    field_enum = name.upper()
    field_type = s_field_type[general_type]
    statement = 's_prototype->add_field_info(new {}({}, \"{}\",  Field_info::RULE_{}, {}_func'.format(field_type, field_enum, field_name, rule, name)
    if (default_value):
      statement = statement + ', s_def_{}'.format(name)
    if (execution_function):
      statement = statement + ', exec_func'
    statement = statement + '));'
    print_line(out, statement)
  print_line(out, "}", dec=True)
  print_empty_line(out)

def print_delete_prototype_definition(out):
  print_line(out, "//! \\brief deletes the node prototype.")
  print_line(out, "void %s::delete_prototype()" % class_name)
  print_line(out, "{", inc=True)
  print_line(out, "delete s_prototype;")
  print_line(out, "s_prototype = nullptr;")
  print_line(out, "}", dec=True)
  print_empty_line(out)

def print_get_prototype_definition(out):
  print_line(out, "//! \\brief obtains the node prototype.")
  print_line(out, "Container_proto* %s::get_prototype()" % class_name)
  print_line(out, "{", inc=True)
  print_line(out, 'if (s_prototype == nullptr) {}::init_prototype();'.format(class_name));
  print_line(out, "return s_prototype;")
  print_line(out, "}", dec=True)
  print_empty_line(out)

#! Print the definitions of the prototype handling functions.
def print_prototype_handling_definitions(out, class_name, derived_class_name, fields):
  print_init_prototype_definition(out, class_name, derived_class_name, fields)
  print_empty_line(out)
  print_delete_prototype_definition(out)
  print_empty_line(out)
  print_get_prototype_definition(out)

def print_set_field(out, field, iterator, op):
  name = field['name']
  type = field['type']
  tmp = name.replace('_', ' ').title().replace(' ', '')
  field_name = tmp[:1].lower() + tmp[1:]
  print_line(out, 'if (name == \"{}\") {{'.format(field_name), inc=True)
  op(out, field)
  print_line(out, 'elem->mark_delete({});'.format(iterator))
  print_line(out, "continue;")
  print_line(out, "}", dec=True)

def print_set_field_from_string(out, field):
  name = field['name']
  type = field['type']
  if type == 'String':
    print_line(out, 'set_{}(boost::algorithm::trim_copy(value));'.format(name))
    return

  general_type = get_general_type(type)
  lexical_cast = s_field_lexical_cast[general_type]
  if not lexical_cast[0]:
    raise Exception('I do not know how to set field {} of type {} from a string!'.format(name, type))
  if not type.endswith('_array'):
    print_line(out, 'set_{}(boost::lexical_cast<{}>(value));'.format(name, type))
    return

  print_line(out, 'std::stringstream ss(value);')
  single_type = get_single_type(type)
  statement = '''std::transform(std::istream_iterator<String>(ss),
                     std::istream_iterator<String>(),
                     std::back_inserter(m_{}),
                     &boost::lexical_cast<{}, String>);'''.format(name, single_type)
  print_call(out, statement)

#! Print the code that handles a multi-string attribute.
def print_set_field_from_multi_string(out, filed):
  name = field['name']
  print_line(out, 'if (name == \"{}\") {'.format(tmp))
  print_line(out, 'm_{}.resize(value.size());'.format(name))
  print_line(out, 'auto tit = m_{}.begin();'.format(name))
  print_line(out, 'for (auto sit : value) *tit++ = std::move(*sit);')

#! Print the code that handles a container attribute.
def print_set_field_from_container(out, field):
  name = field['name']
  type = field['type']
  field_class_type = type[7:].capitalize()
  print_line(out, 'set_{}(boost::dynamic_pointer_cast<{}>(cont));'.format(name, field_class_type))

def print_set_attributes_definition(out, class_name, derived_class_name, fields):
  print_line(out, "//! \\brief sets the attributes of this node.")
  print_line(out, "void %s::set_attributes(Element* elem)" % class_name)
  print_line(out, "{", inc=True)
  print_line(out, "%s::set_attributes(elem);" % derived_class_name)
  print_empty_line(out)

  string_fields = []
  multi_string_fields = []
  container_fields = []
  multi_container_fields = []
  for field in fields[:]:
    type = field['type']
    general_type = get_general_type(type)
    field_element_type = s_field_element_type[general_type]
    if field_element_type == 'string': string_fields.append(field)
    elif field_element_type == 'multi-string': multi_string_fields.append(field)
    elif field_element_type == 'container': container_fields.append(field)
    elif field_element_type == 'multi-container': multi_container_fields.append(field)
    else:
      raise Exception('Element type {} of field \"{}::{}\" is unknown!'.format(field_element_type, class_name, name))
  # String fields
  if 0 < len(string_fields):
    print_line(out, "for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {", inc=True)
    print_line(out, "const auto& name = elem->get_name(ai);")
    print_line(out, "const auto& value = elem->get_value(ai);")
    for field in string_fields[:]:
      print_set_field(out, field, 'ai', print_set_field_from_string)
    print_line(out, "}", dec=True)
    print_empty_line(out)

  # Multi string fields:
  if 0 < len(multi_string_fields):
    print_line(out, "for (auto msai = elem->multi_str_attrs_begin(); msai != elem->multi_str_attrs_end(); ++msai) {")
    print_line(out, "const auto& name = elem->get_name(msai);")
    print_line(out, "auto& value = elem->get_value(msai);")
    for field in multi_string_fields[:]:
      print_set_field(out, field, 'msai', print_set_field_from_multi_string)
    print_line(out, "}", dec=True)
    print_empty_line(out)

  # Container fields:
  if 0 < len(container_fields):
    print_line(out, "for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {", inc=True)
    print_line(out, "const auto& name = elem->get_name(cai);")
    print_line(out, "auto cont = elem->get_value(cai);")

    for field in container_fields[:]:
      print_set_field(out, field, 'cai', print_set_field_from_container)
    print_line(out, "}", dec=True)

  #! \todo multi-container fields

  print_line(out, "// Remove all the deleted attributes:")
  print_line(out, "elem->delete_marked();")

  print_line(out, "}", dec=True)
  print_empty_line(out)

#! Generate the .cpp file
#! \param name
#! \param derived_class_name
#! \param fields
def generate_cpp(output_path, class_name, derived_class_name, fields):
  hpp_filename = os.path.join(output_path, class_name + ".cpp")
  with open(hpp_filename, 'w') as out:
    print_line(out, copyright_text)
    print_line(out, "// Generated by " + os.path.basename(__file__))
    print_empty_line(out)
    print_cpp_include_directives(out, fields)
    print_empty_line(out)
    print_line(out, "SGAL_BEGIN_NAMESPACE")
    print_empty_line(out)

    # Print tag definition:
    # 1. Split class name to single words (separated by '_'),
    # 2. convert first character of each word to uppper case, and
    # 3. re-concatenate.
    node_name = class_name.replace('_', ' ').title().replace(' ', '')
    print_line(out, 'const String {}::s_tag = \"{}\";'.format(class_name, node_name))

    # Print prototype defintion:
    print_line(out, "Container_proto* %s::s_prototype(nullptr);" % class_name)
    print_empty_line(out)

    print_static_member_definitions(out, class_name)
    print_constructor_definition(out, class_name, derived_class_name, fields)
    print_destructor_definition(out, class_name, fields)
    print_prototype_handling_definitions(out, class_name, derived_class_name, fields)
    print_set_attributes_definition(out, class_name, derived_class_name, fields)

    print_line(out, "SGAL_END_NAMESPACE")

# Main function
if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Generate code of nodes.')
  parser.add_argument('filename', metavar="filename", nargs='?',
                      help='the specification file name')
  parser.add_argument('--input-path', type=readable_dir, nargs='*',
                      dest="input_paths", default='./')
  parser.add_argument('--output-inc-path', type=readable_dir,
                      dest='output_inc_path', default='./include/FPG')
  parser.add_argument('--output-lib-path', type=readable_dir,
                      dest='output_lib_path', default='./lib')
  parser.add_argument('--name', help='The node name')
  args = parser.parse_args()

  # Extract node name:
  class_name = args.name
  if not class_name:
    parser.error("The class name is missing!")
    exit(-1)

  # Extract file name:
  filename = None
  if args.filename:
    filename = args.filename
  else:
    filename = class_name + "_spec.txt"

  # Extract full file name:
  fullname = None
  for path in args.input_paths:
    tmp = os.path.join(path, filename)
    if os.path.exists(tmp):
      fullname = tmp
      break

  if not fullname:
    parser.error("The file %s cannot be found!" % filename)
    exit(-1)

  with open(fullname, 'r') as ins:
    derived_class_name = ins.readline().rstrip()

    fields = []
    for line in ins:
      if line[0] == '#':
        continue
      vals = shlex.split(line)
      vals = map(lambda s: s.strip(), vals)
      name = vals[0]
      type = vals[1]
      rule = vals[2]
      default_value = vals[3]
      execution_function = vals[4]
      desc = vals[5].strip('\"')
      # Assume that a type that starts with 'Shared' is either a shared
      # container or a shared container array.
      if not type.startswith('Shared_'):
        if not type in s_field_type:
          raise Exception('Type {} of field \"{}::{}\" is unknown!'.format(type, class_name, name))
      field = {
        'name': name,
        'type': type,
        'rule': rule,
        'default-value': default_value,
        'execution-function': execution_function,
        'desc': desc
      }
      fields.append(field)

    # map(lambda x: print(x), fields)
    generate_hpp(args.output_inc_path, class_name, derived_class_name, fields)
    generate_cpp(args.output_lib_path, class_name, derived_class_name, fields)

  exit()
