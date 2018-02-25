#!/usr/bin/python

from __future__ import print_function
import os.path
import argparse
import json
import shlex
from sets import Set
import ConfigParser, os
import ast
import distutils.util

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

# A set of types defined in SGAL:
s_sgal_types = {
  'Color_array',
  'Coord_array_1d',
  'Coord_array_2d',
  'Coord_array_3d',
  'Coord_array',
  'Normal_array',
  'Tex_coord_array_2d',
  'Tex_coord_array_3d',
  'Tex_coord_array_4d',
  'Tex_coord_array'
}

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
  'Vector4f_array': [ True, True ],
  'Rotation_array': [ True, True ],
  'Bounding_sphere_array': [ True, True ],
  'String_array': [ False, False ],
  'Shared_container_array': [ False, False ]
}

# Map from C++ types to feild size
s_field_size = {
  # Single field
  'Boolean': 1,
  'Float': 1,
  'Uint': 1,
  'Int32': 1,
  'Scene_time': 1,
  'Vector2f': 2,
  'Vector3f': 3,
  'Vector4f': 4,
  'Rotation': 4,
  'Bounding_sphere': 4,
  'String': 1,
  'Shared_container': 1,
  # Multi field
  'Boolean_array': 0,
  'Float_array': 0,
  'Uint_array': 0,
  'Int32_array': 0,
  'Scene_time_array': 0,
  'Vector2f_array': 0,
  'Vector3f_array': 0,
  'Vector4f_array': 0,
  'Rotation_array': 0,
  'Bounding_sphere_array': 0,
  'String_array': 0,
  'Shared_container_array': 0
}

#! Determines whether the fields contain a geometry node
def has_geometry_type(fields):
  for field in fields[:]:
    geometry = field['geometry']
    if geometry:
      return True
  return False

#! A geometry node is either a geometry container, or a container that
# inherits from a geometry node, or a container node that contains
# at least one geometry node.
# Each field has a Boolean atribute called 'geometry' that determines whether
# the field is a geometry node. In addition the configuration file of each node
# has an attribute called 'geometry' that determines wheher the node inherits
# from a geometry node.
def is_geometry_node(config, fields):
  if has_geometry_type(fields):
    return True

  if not config.has_option('class', 'geometry'):
    return False

  opt = config.get('class', 'geometry')
  return distutils.util.strtobool(opt)

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
  print ('{}{}'.format(indent * ' ', line), file=out, end='\n' if eol else '')
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
  forward_types = set()
  for field in fields[:]:
    type = field['type']
    if type.startswith('Shared_'):
      main_type = get_single_type(type[7:])
      forward_types.add(main_type)

  if 0 == len(forward_types):
    return

  for item in forward_types:
    print_line(out, 'class {};'.format(item.capitalize()))
  print_empty_line(out)

# Print typedef definitions of shared containers.
def print_shared_typedefs(out, fields):
  shared_container_types = set()
  for field in fields[:]:
    type = field['type']
    if type.startswith('Shared_'):
      main_type = type[7:]
      if main_type.endswith('_array'):
        main_type = main_type[:-6]
      shared_container_types.add(main_type)

  if 0 == len(shared_container_types):
    return

  for item in shared_container_types:
    statement = 'typedef boost::shared_ptr<{}>'.format(item.capitalize())
    statement = '{:<46}Shared_{};'.format(statement, item)
    print_line(out, statement)
  print_empty_line(out)

# Print typedef definitions of vectors of shared container.
def print_array_typedefs(out, fields):
  shared_array_container_types = set()
  for field in fields[:]:
    type = field['type']
    if type.startswith('Shared_'):
      main_type = type[7:]
      if main_type.endswith('_array'):
        main_type = main_type[:-6]
        shared_array_container_types.add(main_type)

  if 0 == len(shared_array_container_types):
    return

  for item in shared_array_container_types:
    statement = 'typedef std::vector<Shared_{}>'.format(item)
    statement = '{:<46}Shared_{}_array;'.format(statement, item)
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
def print_attribute_handling_declaration(out):
  print_line(out, "virtual void set_attributes(Element* elem);")

#! Print field handling declarations
def print_fields_handling_declarations(out, args):
  fields = args[0]
  for field in fields[:]:
    type = field['type']
    name = field['name']
    print_line(out, "%s* %s_handle(const Field_info*) { return &m_%s; }" % (type, name, name));

#! Print setter and declaration
def print_field_setter_declaration(out, field):
  type = field['type']
  name = field['name']
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

def print_field_adder_declaration(out, field):
  name = field['name']
  type = field['type']
  desc = field['desc']
  print_line(out, '/*! Add a {} to the array of {}. */'.format(desc[:-1], desc))
  print_line(out, 'void add_{}({} {});'.format(name[:-1], type[:-6], name[:-1]));

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
def print_field_setter_definition(out, inlining, class_name, field):
  type = field['type']
  name = field['name']
  inline = 'inline ' if inlining else ""
  contaminated = field['contaminated']
  general_type = get_general_type(type)
  pass_method = s_field_passing_method[general_type]
  desc = field['desc']
  print_line(out, '//! \\brief sets the {}.'.format(desc))
  if pass_method == 'value':
    print_line(out, '{}void {}::set_{}({} {})'.format(inline, class_name, name, type, name))
  elif pass_method == 'reference':
    print_line(out, '{}void {}::set_{}(const {}& {})'.format(inline, class_name, name, type, name))
  else:
    raise Exception('Pass method {} is invalid!'.format(pass_method))
  print_line(out, "{", inc=True)
  for item in contaminated.split():
    print_line(out, 'm_dirty_{} = true;'.format(item))
  print_line(out, 'm_{} = {};'.format(name, name))
  print_line(out, "}", dec=True)

#! Print setter definition
def print_hpp_field_adder_definition(out, class_name, field):
  type = field['type']
  name = field['name']
  desc = field['desc']
  print_line(out, '//! \\brief adds a {} to the array of {}..'.format(desc[:-1], desc))
  print_line(out, 'inline void {}::add_{}({} {})'.format(class_name, name[:-1], type[:-6], name[:-1]))
  print_line(out , '{{ m_{}.push_back({}); }}'.format(name, name[:-1]))

#! Print getter definition
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
def print_field_manipulators_declarations(out, field, last=False):
  print_field_setter_declaration(out, field)
  print_empty_line(out)
  type = field['type']
  general_type = get_general_type(type)
  field_element_type = s_field_element_type[general_type]
  if field_element_type == 'multi-container':
    print_field_adder_declaration(out, field)
    print_empty_line(out)
  print_field_getter_declaration(out, field)
  if not last: print_empty_line(out)

#! Print getters and setters.
def print_fields_manipulators_declarations(out, args):
  fields = args[0]
  for field in fields[:-1]:
    print_field_manipulators_declarations(out, field)
  field = fields[-1]
  print_field_manipulators_declarations(out, field, last=True)

#! Print the field data members.
def print_field_data_members(out, fields):
  for field in fields[:]:
    name = field['name']
    type = field['type']
    desc = field['desc']
    print_line(out, "/*! The %s. */" % desc)
    print_line(out, "%s m_%s;" % (type, name))
    print_empty_line(out)

def print_protected_data_members(config, out):
  protected_data = {}
  if config.has_option('class', 'protected_data'):
    protected_data = ast.literal_eval(config.get('class', 'protected_data'))
  for datum in protected_data:
    desc = config.get(datum, 'desc')
    declaration = config.get(datum, 'declaration')
    print_line(out, '/* {} */'.format(desc))
    print_line(out, '{};'.format(declaration))
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

#! Print the setter, getter, adder, and remover function definition.
# As a convension, if the field has geometry, the definition of the
# corresponding setter, adder, remover are placed in the cpp file.
def print_hpp_field_manipulators_definitions(out, class_name, field):
  geometry = field['geometry']
  contaminated = field['contaminated']
  if not contaminated:
    print_field_setter_definition(out, True, class_name, field)
    print_empty_line(out)
  type = field['type']
  general_type = get_general_type(type)
  field_element_type = s_field_element_type[general_type]
  if not geometry and field_element_type == 'multi-container':
    print_hpp_field_adder_definition(out, class_name, field)
    print_empty_line(out)
  print_field_getter_definition(out, class_name, field)
  print_empty_line(out)

#! Print declarations of field setters, getters, adders, and removers:
def print_hpp_fields_manipulators_definitions(out, class_name, fields):
  for field in fields[:]:
    print_hpp_field_manipulators_definitions(out, class_name, field)

#! Print get_tag() definition.
def print_get_tag_definition(out, class_name):
  print_line(out, "//! \\brief obtains the tag (type) of the container.")
  print_line(out, "inline const String& %s::get_tag() const { return s_tag; }" % class_name)

#! Print the include directives in the hpp
def print_hpp_include_directives(out, library, derived_class_name):
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
  if 'SGAL' != library:
    print_empty_line(out)
    print_line(out, '#include \"FPG/basic.hpp\"')

# Print additional public functions:
def print_public_functions(config, out):
  public_functions = {}
  if config.has_option('class', 'public_functions'):
    public_functions = ast.literal_eval(config.get('class', 'public_functions'))
  for fnc in public_functions:
    desc = config.get(fnc, 'desc')
    signature = config.get(fnc, 'signature')
    print_line(out, '/* {}'.format(desc))
    print_line(out, ' */')
    print_line(out, '{};'.format(signature))
    print_empty_line(out)

# Prototype description.
field_changed_desc = '''/*! Process change of field.
 * \param field_info The information record of the field that changed.
 */
'''
#! Print the declaration of the field_changed() member function:
def print_field_changed_declaration(out):
  print_block(out, field_changed_desc)
  print_line(out, "virtual void field_changed(const Field_info* field_info);")
  print_empty_line(out)

#! Generate the .hpp file
#! \param config the configuration dictionary of the node.
#! \param fields the fields of the node.
#! \param out the output stream.
def generate_hpp(library, config, fields, out):
  print_line(out, copyright_text)
  print_line(out, "// Generated by " + os.path.basename(__file__))
  print_empty_line(out)
  print_line(out, '#ifndef {}_{}_HPP'.format(library, class_name.upper()))
  print_line(out, '#define {}_{}_HPP'.format(library, class_name.upper()))
  print_empty_line(out)
  print_hpp_include_directives(out, library, derived_class_name)
  print_empty_line(out)
  print_line(out, "SGAL_BEGIN_NAMESPACE")
  print_empty_line(out)
  print_forward_declarations(out, fields)

  print_line(out, 'class SGAL_{}_DECL {} : public {} {{'.format(library, class_name, derived_class_name))
  print_line(out, "public:")
  increase_indent()

  print_field_enumeration(out, derived_class_name, fields)	# enumerations
  print_shared_typedefs(out, fields)				# typedefs
  print_array_typedefs(out, fields)				# typedefs

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
              print_attribute_handling_declaration)

  # Print field handling declarations:
  print_group(out, "Field handlers",
              print_fields_handling_declarations, fields)

  # Print declarations of field setters and getters:
  print_group(out, "getters & setters",
              print_fields_manipulators_declarations, fields)

  #! Print the declaration of the field_changed() member function:
  if is_geometry_node(config, fields):
    print_field_changed_declaration(out)

  # Print additional public functions:
  print_public_functions(config, out)

  decrease_indent()
  print_line(out, "protected:")
  increase_indent()

  print_protected_data_members(config, out)
  print_field_data_members(out, fields)
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

  # Print declarations of fields manipulators:
  print_hpp_fields_manipulators_definitions(out, class_name, fields)

  print_get_tag_definition(out, class_name)

  print_empty_line(out)
  print_line(out, "SGAL_END_NAMESPACE")
  print_empty_line(out)
  print_line(out, "#endif")

#! Generate the .hpp file
#! \param config the configuration dictionary of the node.
#! \param fields the fields of the node.
#! \param output_path the output path.
def generate_hpp_from_path(library, config, fields, output_path):
  class_name = config.get('DEFAULT', 'name')
  derived_class_name = config.get('class', 'base')

  hpp_filename = os.path.join(output_path, class_name + ".hpp")
  with open(hpp_filename, 'w') as out:
    generate_hpp(library, config, fields, out)

def print_cpp_include_directives(out, fields):
  include_sstream = False
  include_iterator = False
  include_algorithm = False
  include_std = False
  include_boost_algorithm_string = False
  include_boost_lexical_case = False
  include_boost = False

  include_multi_istream_iterator = False;

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
            include_multi_istream_iterator = True

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

  # Print include SGAL headers:
  print_line(out, "#include \"SGAL/Element.hpp\"")
  print_line(out, "#include \"SGAL/Container_proto.hpp\"")
  print_line(out, "#include \"SGAL/Field_infos.hpp\"")
  print_line(out, "#include \"SGAL/Field.hpp\"")
  if include_multi_istream_iterator:
    print_line(out, "#include \"SGAL/multi_istream_iterator.hpp\"")
  for item in set(field_names_lexical_cast):
    print_line(out, '#include \"SGAL/lexical_cast_{}.hpp\"'.format(item))
  for item in set(field_names_shared_container):
    if item in s_sgal_types:
      print_line(out, '#include \"SGAL/{}.hpp\"'.format(item))
  print_empty_line(out)

  # Print include library headers:
  print_line(out, '#include \"{}/{}.hpp\"'.format(library, class_name))
  for item in set(field_names_shared_container):
    if item not in s_sgal_types:
      print_line(out, '#include \"{}/{}.hpp\"'.format(library, item))
  print_empty_line(out)

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
def print_constructor_definition(config, out, class_name, derived_class_name, fields):
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
    protected_data = {}
    if config.has_option('class', 'protected_data'):
      protected_data = ast.literal_eval(config.get('class', 'protected_data'))
      for datum in protected_data:
        init = config.get(datum, 'initialization')
        print_line(out, '{},'.format(init))

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
  size = s_field_size[single_type]
  statement = '''std::transform(multi_istream_iterator<{}>(ss),
                     multi_istream_iterator<{}>(),
                     std::back_inserter(m_{}),
                     &boost::lexical_cast<{}, const String&>);'''.format(size, size, name, single_type)
  print_call(out, statement)

#! Print the code that handles a multi-string attribute.
def print_set_field_from_multi_string(out, filed):
  name = field['name']
  contaminated = field['contaminated']
  print_line(out, 'm_{}.resize(value.size());'.format(name))
  print_line(out, 'auto tit = m_{}.begin();'.format(name))
  print_line(out, 'for (auto sit : value) *tit++ = std::move(*sit);')
  # The above two line could be replaced by the single statement below using
  # C++17. It requires 'auto' of formal variable of lambda functions.
  # std::transform(value.begin(), value.end(), m_{}.begin(),
  #                [](auto s) -> std::string { return std::move(*s); });
  for item in contaminated.split():
    print_line(out, 'm_dirty_{} = true;'.format(item))

#! Print the code that handles a container attribute.
def print_set_field_from_container(out, field):
  name = field['name']
  type = field['type']
  field_class_type = type[7:].capitalize()
  print_line(out, 'set_{}(boost::dynamic_pointer_cast<{}>(cont));'.format(name, field_class_type))

def print_set_field_from_multi_container(out, field):
  name = field['name']
  type = field['type']
  field_class_type = type[7:][:-6].capitalize()
  print_line(out, "for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {", inc=True)
  print_line(out, 'add_{}(boost::dynamic_pointer_cast<{}>(*ci));'.format(name[:-1], field_class_type))
  print_line(out, "}", dec=True)

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

  # Multi-string fields:
  if 0 < len(multi_string_fields):
    print_line(out, "for (auto msai = elem->multi_str_attrs_begin(); msai != elem->multi_str_attrs_end(); ++msai) {", inc=True)
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

  #! Multi-container fields
  if 0 < len(multi_container_fields):
    print_line(out, "for (auto mcai = elem->multi_cont_attrs_begin(); mcai != elem->multi_cont_attrs_end(); ++mcai) {", inc=True)
    print_line(out, "const auto& name = elem->get_name(mcai);")
    print_line(out, "auto& cont_list = elem->get_value(mcai);")
    for field in multi_container_fields[:]:
      print_set_field(out, field, 'mcai', print_set_field_from_multi_container)
    print_line(out, "}", dec=True)
    print_empty_line(out)

  print_line(out, "// Remove all the deleted attributes:")
  print_line(out, "elem->delete_marked();")

  print_line(out, "}", dec=True)
  print_empty_line(out)

#! Print statements that register an observer
def print_register_observer(out, field, single=True):
  name = field['name']
  type = field['type']
  print_line(out, "const auto* field_info = get_field_info(BOUNDING_SPHERE);")
  print_line(out, "Observer observer(this, field_info);")
  if single:
    single_name = name
    if type.endswith('_array'):
      single_name = name[:-1]
    print_line(out, '{}->register_observer(observer);'.format(single_name))
  else:
    print_line(out, 'for (auto it = m_{}.begin(); it != m_{}.end(); ++it)'.format(name, name))
    increase_indent()
    print_line(out, "(*it)->register_observer(observer);")
    decrease_indent()
  print_line(out, "field_changed(field_info);")

#! Print setter definition
def print_cpp_field_adder_definition(out, class_name, field):
  type = field['type']
  name = field['name']
  desc = field['desc']
  print_line(out, '//! \\brief adds a {} to the array of {}..'.format(desc[:-1], desc))
  print_line(out, 'void {}::add_{}({} {})'.format(class_name, name[:-1], type[:-6], name[:-1]))
  print_line(out, "{", inc=True)
  print_line(out , 'm_{}.push_back({});'.format(name, name[:-1]))
  print_register_observer(out, field, single=True)
  print_line(out, "}", dec=True)

#! Print the setter, getter, adder, and remover function definition.
# As a convension, if the field contaminates other when it is assigned, the
# definition of the corresponding setter, adder, remover are placed in the
# cpp file.
def print_cpp_field_manipulators_definitions(out, class_name, field):
  geometry = field['geometry']
  contaminated = field['contaminated']
  if contaminated:
    print_field_setter_definition(out, False, class_name, field)
    print_empty_line(out)
  type = field['type']
  general_type = get_general_type(type)
  field_element_type = s_field_element_type[general_type]
  if geometry and field_element_type == 'multi-container':
    print_cpp_field_adder_definition(out, class_name, field)
    print_empty_line(out)

#! Print declarations of field setters and getters:
def print_cpp_fields_manipulators_definitions(out, class_name, fields):
  for field in fields[:]:
    print_cpp_field_manipulators_definitions(out, class_name, field)

 #! Print the definition of the field_changed() member function.
def print_field_changed_definition(out, class_name):
  print_line(out, "//! \\brief processes change of field.")
  print_line(out, 'void {}::field_changed(const Field_info* field_info)'.format(class_name))
  print_line(out, "{", inc=True)
  print_line(out, "switch (field_info->get_id()) {", inc=True)
  print_line(out, "case BOUNDING_SPHERE: m_dirty_bounding_sphere = true; break;")
  print_line(out, "default: break;")
  print_line(out, "}", dec=True)
  print_line(out, "Node::field_changed(field_info);")
  print_line(out, "}", dec=True)
  print_empty_line(out)

#! Generate the .cpp file
#! \param config
#! \param fields
#! \param out
def generate_cpp(config, fields, out):
  print_line(out, copyright_text)
  print_line(out, "// Generated by " + os.path.basename(__file__))
  print_empty_line(out)
  print_cpp_include_directives(out, fields)
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
  print_constructor_definition(config, out, class_name, derived_class_name, fields)
  print_destructor_definition(out, class_name, fields)
  print_prototype_handling_definitions(out, class_name, derived_class_name, fields)
  print_set_attributes_definition(out, class_name, derived_class_name, fields)

  # Print getters, setters, adders, and removers:
  cpp_fields = []
  for field in fields[:]:
    contaminated = field['contaminated']
    if contaminated:
      cpp_fields.append(field)

  if 0 < len(cpp_fields):
    print_cpp_fields_manipulators_definitions(out, class_name, cpp_fields)
    # print_cpp_field_draw_definitions()
    # print_cpp_field_cull_definitions()
    # print_cpp_field_isect_definitions()

  if is_geometry_node(config, fields):
    print_field_changed_definition(out, class_name)

  print_line(out, "SGAL_END_NAMESPACE")

#! Generate the .cpp file
#! \param config
#! \param fields
#! \param output_path
def generate_cpp_from_path(library, config, fields, output_path):
  class_name = config.get('DEFAULT', 'name')
  derived_class_name = config.get('class', 'base')

  hpp_filename = os.path.join(output_path, class_name + ".cpp")
  with open(hpp_filename, 'w') as out:
    generate_cpp(config, fields, out)

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
  parser.add_argument('--library', default='SGAL', help='The library name')
  args = parser.parse_args()

  # Extract library name:
  library = args.library

  # Extract node name:
  name = args.name
  if not name:
    parser.error("The class name is missing!")
    exit(-1)

  # Extract file name:
  filename = None
  if args.filename:
    filename = args.filename
  else:
    filename = name + ".cfg"

  # Extract configuration full file name:
  fullname = None
  for path in args.input_paths:
    tmp = os.path.join(path, filename)
    if os.path.exists(tmp):
      fullname = tmp
      break

  if not fullname:
    parser.error("The file %s cannot be found!" % filename)
    exit(-1)

  config = ConfigParser.ConfigParser()
  config.readfp(open(fullname))

  with open(fullname, 'r') as confis:
    config.readfp(confis)
    class_name = config.get('DEFAULT', 'name')
    derived_class_name = config.get('class', 'base')
    fields_filename = config.get('fields', 'spec')

    # Extract field specification full file name:
    fields_fullname = None
    for path in args.input_paths:
      tmp = os.path.join(path, fields_filename)
      if os.path.exists(tmp):
        fields_fullname = tmp
        break

    if not fields_fullname:
      parser.error('The file {} cannot be found!"'.format(fields_filename))
      exit(-1)

    with open(fields_fullname, 'r') as ins:
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
        contaminated = vals[5]
        desc = vals[6].strip('\"')
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
          'contaminated': contaminated,
          'desc': desc,
          'geometry': False
        }
        fields.append(field)

      # map(lambda x: print(x), fields)
      generate_hpp_from_path(library, config, fields, args.output_inc_path)
      generate_cpp_from_path(library, config, fields, args.output_lib_path)

  exit()
