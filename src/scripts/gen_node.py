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
import collections

# Ordered set
class OrderedSet(collections.OrderedDict, collections.MutableSet):
  def update(self, *args, **kwargs):
    if kwargs:
      raise TypeError("update() takes no keyword arguments")

    for s in args:
      for e in s:
         self.add(e)

  def add(self, elem):
    self[elem] = None

  def discard(self, elem):
    self.pop(elem, None)

  def __le__(self, other):
    return all(e in other for e in self)

  def __lt__(self, other):
    return self <= other and self != other

  def __ge__(self, other):
    return all(e in self for e in other)

  def __gt__(self, other):
    return self >= other and self != other

  def __repr__(self):
    return 'OrderedSet([%s])' % (', '.join(map(repr, self.keys())))

  def __str__(self):
    return '{%s}' % (', '.join(map(repr, self.keys())))

  difference = property(lambda self: self.__sub__)
  difference_update = property(lambda self: self.__isub__)
  intersection = property(lambda self: self.__and__)
  intersection_update = property(lambda self: self.__iand__)
  issubset = property(lambda self: self.__le__)
  issuperset = property(lambda self: self.__ge__)
  symmetric_difference = property(lambda self: self.__xor__)
  symmetric_difference_update = property(lambda self: self.__ixor__)
  union = property(lambda self: self.__or__)

#
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
s_types = {
  'SGAL': {
    'Color_array',
    'Container',
    'Coord_array',
    'Coord_array_1d',
    'Coord_array_2d',
    'Coord_array_3d',
    "Group",
    'Indexed_face_set',
    'Node',
    'Normal_array',
    "Shape",
    'Tex_coord_array',
    'Tex_coord_array_2d',
    'Tex_coord_array_3d',
    'Tex_coord_array_4d'
  }
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

#! Obtain the complete type including the namespace or class of a given type.
def get_complete_type(type, library):
  for namespace, types in s_types.iteritems():
    if type in types:
      if namespace != library:
        return '{}::{}'.format(namespace, type)
      break;
  return type

#! Obtain the namespace of a given type
def get_type_namespace(type, library):
  for namespace, types in s_types.iteritems():
    if type in types:
      return namespace
  return library

#! Obtain the field handle type of a given type.
def get_field_handle_type(type):
  if 'SGAL' == library:
    return s_field_handle_type[type]
  else:
    return 'SGAL::' + s_field_handle_type[type]

#! Obtain the field type.
def get_field_type(type):
  if 'SGAL' == library:
    return s_field_type[type]
  else:
    return 'SGAL::' + s_field_type[type]

#! Obtain the type name and namespace
def get_type_attributes(type, library):
  try:
    namespace, name = type.split("::", 1)
    return name, namespace
  except ValueError:
    return type, library

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
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
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

#! print friend declarations.
def print_friends(config, out):
  if not config.has_option('class', 'friends'):
    return
  friends = ast.literal_eval(config.get('class', 'friends'))
  for friend in friends:
    print_line(out, 'friend class {};'.format(friend))
  print_empty_line(out)

#! Print enumerations.
def print_field_enumeration(out, derived_class, fields):
  print_line(out, "enum {", inc=True)
  print_line(out, "FIRST = %s::LAST - 1," % derived_class)
  for field in fields[:]:
    print_line(out, field['name'].upper() + ',')
  print_line(out, "LAST")
  print_line(out, "};", dec=True)
  print_empty_line(out)

#! Print an enumeration:
def print_enumeration(config, out, enum):
  desc = config.get(datum, 'desc')
  print_line(out, '//! {}.'.format(desc))
  print_line(out, 'enum {} {{'.format(enum), inc=True)
  tokens = ast.literal_eval(config.get(enum, 'tokens'))
  for token in tokens[:]:
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
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
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

#! print friend declarations.
def print_friends(config, out):
  if not config.has_option('class', 'friends'):
    return
  friends = ast.literal_eval(config.get('class', 'friends'))
  for friend in friends:
    print_line(out, 'friend class {};'.format(friend))
  print_empty_line(out)

#! Print enumerations.
def print_field_enumeration(out, derived_class, fields):
  print_line(out, "enum {", inc=True)
  print_line(out, "FIRST = %s::LAST - 1," % derived_class)
  for field in fields[:]:
    print_line(out, field['name'].upper() + ',')
  print_line(out, "LAST")
  print_line(out, "};", dec=True)
  print_empty_line(out)

#! Print an enumeration:
def print_enumeration(config, out, enum):
  desc = config.get(datum, 'desc')
  print_line(out, '//! {}.'.format(desc))
  print_line(out, 'enum {} {{'.format(enum), inc=True)
  tokens = ast.literal_eval(config.get(enum, 'tokens'))
  for token in tokens[:]:
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
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
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

#! print friend declarations.
def print_friends(config, out):
  if not config.has_option('class', 'friends'):
    return
  friends = ast.literal_eval(config.get('class', 'friends'))
  for friend in friends:
    print_line(out, 'friend class {};'.format(friend))
  print_empty_line(out)

#! Print enumerations.
def print_field_enumeration(out, derived_class, fields):
  print_line(out, "enum {", inc=True)
  print_line(out, "FIRST = %s::LAST - 1," % derived_class)
  for field in fields[:]:
    print_line(out, field['name'].upper() + ',')
  print_line(out, "LAST")
  print_line(out, "};", dec=True)
  print_empty_line(out)

#! Print an enumeration:
def print_enumeration(config, out, enum):
  desc = config.get(enum, 'desc')
  print_line(out, '//! {}.'.format(desc))
  print_line(out, 'enum {} {{'.format(enum), inc=True)
  tokens = ast.literal_eval(config.get(enum, 'tokens'))
  for token in tokens[:-1]:
    print_line(out, '{},'.format(token));
  print_line(out, tokens[-1]);
  print_line(out, "};", dec=True)

#! Print all enumerations
def print_enumerations(config, out, derived_class, fields):
  print_field_enumeration(out, derived_class, fields)
  if config.has_option('class', 'public-enumerations'):
    enumarations = ast.literal_eval(config.get('class', 'public-enumerations'))
    for enum in enumarations:
      print_enumeration(config, out, enum)
    print_empty_line(out)

#! Print forward declarations
def print_forward_declarations(config, out, fields):
  forward_types = collections.OrderedDict()
  forward_types['SGAL'] = OrderedSet()
  if not library in forward_types:
    forward_types[library] = OrderedSet()

  forward_types['SGAL'].add("Container_proto")
  forward_types['SGAL'].add("Element")

  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
      main_type = get_single_type(type_name[7:]).capitalize()

      # Override type_namespace:
      type_namespace = get_type_namespace(main_type, library)

      # Add to forward types:
      if not type_namespace in forward_types:
        forward_types[type_namespace] = OrderedSet()
      forward_types[type_namespace].add(main_type)

  if config.has_option('class', 'forward-declarations'):
    types = ast.literal_eval(config.get('class', 'forward-declarations'))
    for type in types:
      type_name, type_namespace = get_type_attributes(type, library)
      if not type_namespace in forward_types:
        forward_types[type_namespace] = OrderedSet()
      forward_types[type_namespace].add(type_name)

  if config.has_option('class', 'shared-types'):
    types = ast.literal_eval(config.get('class', 'shared-types'))
    for type in types:
      type_name, type_namespace = get_type_attributes(type, library)
      if not type_namespace in forward_types:
        forward_types[type_namespace] = OrderedSet()
      forward_types[type_namespace].add(type)

  # print:
  for key, value in forward_types.iteritems():
    if 0 != len(value):
      print_line(out, '{}_BEGIN_NAMESPACE'.format(key))
      print_empty_line(out)
      for item in value:
        print_line(out, 'class {};'.format(item))
      print_empty_line(out)
      print_line(out, '{}_END_NAMESPACE'.format(key))
      print_empty_line(out)

# Print typedef definitions
def print_typedefs(config, out, fields):
  print_shared_typedefs(config, out, fields)
  print_array_typedefs(out, fields)
  if config.has_option('class', 'public-typedefs'):
    typedefs = ast.literal_eval(config.get('class', 'public-typedefs'))
    for typedef in typedefs:
      print_line(out, '{};'.format(typedef))
    print_empty_line(out)

# Print typedef definitions of shared containers.
def print_shared_typedefs(config, out, fields):
  shared_container_types = set()

  if config.has_option('class', 'shared-types'):
    types = ast.literal_eval(config.get('class', 'shared-types'))
    for type in types:
      shared_container_types.add(type.lower())

  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
      main_type = type_name[7:]
      if main_type.endswith('_array'):
        main_type = main_type[:-6]
      shared_container_types.add(main_type)

  if 0 == len(shared_container_types):
    return

  for item in shared_container_types:
    # Add namespace if necessary:
    type = get_complete_type(item.capitalize(), library)
    statement = 'typedef boost::shared_ptr<{}>'.format(type)
    statement = '{:<64}Shared_{};'.format(statement, item)
    print_line(out, statement)
  print_empty_line(out)

# Print typedef definitions of vectors of shared container.
def print_array_typedefs(out, fields):
  shared_array_container_types = set()
  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
      main_type = type_name[7:]
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
def print_constructor_declaration(out, name, library):
  print_block(out, constructor_desc)
  if 'SGAL' == library:
    print_line(out, '{}(Boolean proto = false);'.format(name))
  else:
    print_line(out, '{}(SGAL::Boolean proto = false);'.format(name))
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

# Create description.
create_desc = '''/*! Create a new container of this type (virtual copy constructor).
 * \\return a new container of this type.
 */
'''

#! Print create() declaration.
def print_create_declaration(out, library):
  print_block(out, create_desc)
  if 'SGAL' == library:
    print_line(out, "virtual Container* create();")
  else:
    print_line(out, "virtual SGAL::Container* create();")
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
def print_get_prototype_declaration(out, library):
  print_block(out, get_prototype_desc);
  if 'SGAL' == library:
    print_line(out, 'virtual Container_proto* get_prototype();')
  else:
    print_line(out, 'virtual SGAL::Container_proto* get_prototype();')

#! Print prototype handling declarations.
def print_prototype_handling_declarations(out, args):
  library = args[0]
  print_init_prototype_declaration(out)
  print_empty_line(out);
  print_delete_prototype_declaration(out)
  print_empty_line(out);
  print_get_prototype_declaration(out, library)

# set_attribute() description
get_prototype_desc = '''/*! Set the attributes of the transform.
 * \\param[in] elem contains lists of attribute name and value pairs.
 */
'''

#! Print attribute handling declarations.
def print_attribute_handling_declaration(out, args):
  library = args[0]
  if 'SGAL' == library:
    print_line(out, "virtual void set_attributes(Element* elem);")
  else:
    print_line(out, "virtual void set_attributes(SGAL::Element* elem);")

#! Print field handling declarations
def print_fields_handling_declarations(out, args):
  fields = args[0]
  library = args[1]
  for field in fields[:]:
    type = field['type']
    name = field['name']
    if 'SGAL' == library:
      print_line(out, '{}* {}_handle(const Field_info*) {{ return &m_{}; }}'.format(type, name, name));
    else:
      print_line(out, '{}* {}_handle(const SGAL::Field_info*) {{ return &m_{}; }}'.format(type, name, name));

#! Print setter and declaration
def print_field_setter_declaration(out, field, library):
  type = field['type']
  name = field['name']
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
  pass_method = s_field_passing_method[general_type]
  desc = field['desc']
  print_line(out, '/*! Set the {}. */'.format(desc))
  if pass_method == 'value':
    print_line(out, 'void set_{}({} {});'.format(name, type, name));
  elif pass_method == 'reference':
    print_line(out, 'void set_{}(const {}& {});'.format(name, type, name));
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

def print_field_adder_declaration(out, field):
  name = field['name']
  type = field['type']
  desc = field['desc']
  type_name, type_namespace = get_type_attributes(type, library)
  print_line(out, '/*! Add a {} to the array of {}. */'.format(desc[:-1], desc))
  print_line(out, 'void add_{}({} {});'.format(name[:-1], type_name[:-6], name[:-1]));

#! Print setter declaration
def print_field_getter_declaration(out, field):
  type = field['type']
  name = field['name']
  clean_func = field['clean-function']
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
  pass_method = s_field_passing_method[general_type]
  compound = s_field_return_qualification[general_type]
  desc = field['desc']
  print_line(out, '/*! Obtain the {}. */'.format(desc))
  if pass_method == 'value':
    if not compound:
      if clean_func:
        print_line(out, '{} get_{}();'.format(type, name))
      else:
        print_line(out, '{} get_{}() const;'.format(type, name))
    else:
      if not clean_func:
        print_line(out, 'const {} get_{}() const;'.format(type, name))
      print_line(out, '{} get_{}();'.format(type, name))
  elif pass_method == 'reference':
    if not clean_func:
      print_line(out, 'const {}& get_{}() const;'.format(type, name))
    print_line(out, '{}& get_{}();'.format(type, name))
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

#! Print setter definition
def print_field_setter_definition(config, out, inlining, class_name, field):
  type = field['type']
  name = field['name']
  observed = distutils.util.strtobool(field['observed'])
  inline = 'inline ' if inlining else ""
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
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
  exec_func = get_execution_function(config, field)
  if observed or exec_func:
    print_line(out, 'auto* field_info = get_field_info({});'.format(name.upper()))
    if observed:
      print_line(out, 'field_info->detach(this);')
  print_line(out, 'm_{} = {};'.format(name, name))
  if exec_func:
    print_line(out, '{}(field_info);'.format(exec_func))
  elif observed:
    raise Exception('Field {} does not have an execution function, but is observed!'.format(name))
  print_line(out, "}", dec=True)

#! Print setter definition
def print_hpp_field_adder_definition(out, class_name, field):
  type = field['type']
  name = field['name']
  desc = field['desc']
  type_name, type_namespace = get_type_attributes(type, library)
  print_line(out, '//! \\brief adds a {} to the array of {}..'.format(desc[:-1], desc))
  print_line(out, 'inline void {}::add_{}({} {})'.format(class_name, name[:-1], type_name[:-6], name[:-1]))
  print_line(out , '{{ m_{}.push_back({}); }}'.format(name, name[:-1]))

#! Print getter definition
def print_field_getter_definition(out, inlining, class_name, field):
  type = field['type']
  name = field['name']
  inline = 'inline ' if inlining else ""
  clean_func = field['clean-function']
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
  pass_method = s_field_passing_method[general_type]
  compound = s_field_return_qualification[general_type]
  desc = field['desc']
  ext_type = type
  if type_name.startswith('Shared_'):
    ext_type = class_name + '::' + type
  if pass_method == 'value':
    if not compound:
      print_line(out, '//! \\brief obtains the {}.'.format(desc))
      if clean_func:
        print_line(out, '{}{} {}::get_{}()'.format(inline, ext_type, class_name, name))
        print_line(out, "{", inc=True)
        print_line(out, 'if (m_dirty_{}) clean_{}();'.format(clean_func, clean_func));
        print_line(out, 'return m_{};'.format(name))
        print_line(out, "}", dec=True)
      else:
        print_line(out, '{}{} {}::get_{}() const'.format(inline, ext_type, class_name, name))
        print_line(out, '{{ return m_{}; }}'.format(name))
    #
    else:
      if not clean_func:
        print_line(out, '//! \\brief obtains the {}.'.format(desc))
        print_line(out, '{}const {} {}::get_{}() const'.format(inline, ext_type, class_name, name))
        print_line(out, '{{ return m_{}; }}'.format(name))
        print_empty_line(out)
      print_line(out, '//! \\brief obtains the {}.'.format(desc))
      print_line(out, '{}{} {}::get_{}()'.format(inline, ext_type, class_name, name))
      if clean_func:
        print_line(out, "{", inc=True)
        print_line(out, 'if (m_dirty_{}) clean_{}();'.format(clean_func, clean_func));
        print_line(out, 'return m_{};'.format(name))
        print_line(out, "}", dec=True)
      else:
        print_line(out, '{{ return m_{}; }}'.format(name))
  elif pass_method == 'reference':
    if not clean_func:
      print_line(out, '//! \\brief obtains the {}.'.format(desc))
      print_line(out, '{}const {}& {}::get_{}() const'.format(inline, ext_type, class_name, name))
      print_line(out, '{{ return m_{}; }}'.format(name))
    print_empty_line(out)
    print_line(out, '//! \\brief obtains the {}.'.format(desc))
    print_line(out, '{}{}& {}::get_{}()'.format(inline, ext_type, class_name, name))
    if clean_func:
      print_line(out, "{", inc=True)
      print_line(out, 'if (m_dirty_{}) clean_{}();'.format(clean_func, clean_func));
      print_line(out, 'return m_{};'.format(name))
      print_line(out, "}", dec=True)
    else:
      print_line(out, '{{ return m_{}; }}'.format(name))
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

#! Handle field setter & getter declarations
def print_field_manipulators_declarations(out, field, last=False):
  print_field_setter_declaration(out, field, library)
  print_empty_line(out)
  type = field['type']
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
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
    print_line(out, "//! The %s." % desc)
    print_line(out, "%s m_%s;" % (type, name))
    print_empty_line(out)

def print_protected_data_members_from_set(out, members, static=False):
  for datum in members:
    desc = config.get(datum, 'desc')
    type = config.get(datum, 'type')
    name = config.get(datum, 'name')
    print_line(out, '//! {}'.format(desc))
    if static:
      print_line(out, 'static {} {};'.format(type, name))
    else:
      print_line(out, '{} {};'.format(type, name))
    print_empty_line(out)

def print_protected_data_members(config, out):
  if not config.has_option('class', 'protected-data'):
    return
  members = ast.literal_eval(config.get('class', 'protected-data'))
  print_protected_data_members_from_set(out, members)

def print_static_protected_data_members_declarations(config, out):
  if not config.has_option('class', 'static-protected-data'):
    return
  members = ast.literal_eval(config.get('class', 'static-protected-data'))
  print_protected_data_members_from_set(out, members, True)

def print_get_tag_declaration(out, library):
  print_line(out, '/*! Obtain the tag (type) of the container. */')
  if 'SGAL' == library:
    print_line(out, 'virtual const String& get_tag() const;')
  else:
    print_line(out, 'virtual const SGAL::String& get_tag() const;')
  print_empty_line(out)

#! Print the field static members
def print_static_member_declarations(out, fields):
  print_line(out, "// Default values:")
  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
      continue
    name = field['name']
    desc = field['desc']
    print_line(out, "static const %s s_def_%s;" % (type, name))

def print_tag_data_member(out, library):
  print_line(out, "//! The tag that identifies this container type.")
  if 'SGAL' == library:
    print_line(out, "static const String s_tag;")
  else:
    print_line(out, "static const SGAL::String s_tag;")
  print_empty_line(out)

def print_prototype_data_member(out, library):
  print_line(out, "//! The container prototype.")
  if 'SGAL' == library:
    print_line(out, "static Container_proto* s_prototype;");
  else:
    print_line(out, "static SGAL::Container_proto* s_prototype;");
  print_empty_line(out)

def print_prototype_definition(out, class_name):
  print_line(out, "//! \\brief constructs the prototype.")
  print_line(out, "inline %s* %s::prototype() { return new %s(true); }" %
             (class_name, class_name, class_name))
  print_empty_line(out)

def print_create_definition(out, class_name, library):
  print_line(out, "//! \\brief creates a new container of this type (virtual copy constructor).")
  if 'SGAL' == library:
    print_line(out, 'inline Container* {}::create() {{ return new {}(); }}'.format(class_name, class_name))
  else:
    print_line(out, 'inline SGAL::Container* {}::create() {{ return new {}(); }}'.format(class_name, class_name))
  print_empty_line(out)

#! Print the setter, getter, adder, and remover function definition.
# As a convension, if the field has geometry, the definition of the
# corresponding setter, adder, remover are placed in the cpp file.
def print_hpp_field_manipulators_definitions(config, out, class_name, field):
  geometry = field['geometry']
  exec_func = get_execution_function(config, field)
  clean_func = field['clean-function']
  if not exec_func:
    print_field_setter_definition(config, out, True, class_name, field)
    print_empty_line(out)
  type = field['type']
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
  field_element_type = s_field_element_type[general_type]
  if not geometry and field_element_type == 'multi-container':
    print_hpp_field_adder_definition(out, class_name, field)
    print_empty_line(out)
  if not clean_func:
    print_field_getter_definition(out, True, class_name, field)
    print_empty_line(out)

#! Print declarations of field setters, getters, adders, and removers:
def print_hpp_fields_manipulators_definitions(config, out, class_name, fields):
  for field in fields[:]:
    print_hpp_field_manipulators_definitions(config, out, class_name, field)

#! Print get_tag() definition.
def print_get_tag_definition(out, class_name, library):
  print_line(out, "//! \\brief obtains the tag (type) of the container.")
  if 'SGAL' == library:
    print_line(out, 'inline const String& {}::get_tag() const {{ return s_tag; }}'.format(class_name))
  else:
    print_line(out, 'inline const SGAL::String& {}::get_tag() const {{ return s_tag; }}'.format(class_name))

#! Print the include directives in the hpp
def print_hpp_include_directives(config, out, library, derived_class):
  incs = collections.OrderedDict()
  incs['os'] = OrderedSet()
  incs['stl'] = OrderedSet()
  incs['boost'] = OrderedSet()
  incs['CGAL'] = OrderedSet()
  incs['SGAL'] = OrderedSet()

  # cpp includes:
  hat = has_array_type(fields)
  incs['stl'].add('string')
  if (hat):
    incs['stl'].add('vector')

  # boost includes:
  if has_shared_type(fields) or config.has_option('class', 'shared-types'):
    incs['boost'].add('boost/shared_ptr.hpp')

  # SGAL_includes:
  incs['SGAL'].add('SGAL/basic.hpp')
  incs['SGAL'].add('SGAL/Types.hpp')
  if hat:
    incs['SGAL'].add('SGAL/Array_types.hpp')

  # Library incs:
  if 'SGAL' != library:
    if not library in incs:
      incs[library] = OrderedSet()
    incs[library].add('{}/basic.hpp'.format(library))

  # Derived class
  try:
    derived_class_namespace, derived_class_name = derived_class.split("::")
    incs[derived_class_namespace].add('{}/{}.hpp'.format(derived_class_namespace, derived_class_name))
  except ValueError:
    incs[library].add('{}/{}.hpp'.format(library, derived_class))

  # Add headers from config;
  if config.has_option('class', 'includes'):
    includes = ast.literal_eval(config.get('class', 'includes'))
    for inc in includes:
      lib = 'stl'
      tmp, file = os.path.split(inc)
      while tmp:
        lib = tmp
        tmp, file = os.path.split(lib)
      if not lib in incs:
        incs[lib] = OrderedSet()
      incs[lib].add(inc)

  # Print
  for key, value in incs.iteritems():
    if 0 != len(value):
      for item in value:
        if key != library:
          print_line(out, '#include <{}>'.format(item))
        else:
          print_line(out, '#include \"{}\"'.format(item))
      print_empty_line(out)

# Print additional private function declarations:
def print_private_function_declarations(config, out):
  functions = {}
  if not config.has_option('class', 'private-functions'):
    return;
  functions = ast.literal_eval(config.get('class', 'private-functions'))
  print_function_declarations(config, out, functions)

# Print additional protected function declarations:
def print_protected_function_declarations(config, out):
  functions = {}
  if not config.has_option('class', 'protected-functions'):
    return;
  functions = ast.literal_eval(config.get('class', 'protected-functions'))
  print_function_declarations(config, out, functions)

# Print additional public function declarations:
def print_public_function_declarations(config, out):
  functions = {}
  if not config.has_option('class', 'public-functions'):
    return;
  functions = ast.literal_eval(config.get('class', 'public-functions'))
  print_function_declarations(config, out, functions)

# Print function decalarions:
def print_function_declarations(config, out, functions):
  for fnc in functions:
    desc = config.get(fnc, 'desc')
    signature = config.get(fnc, 'signature')
    print_line(out, '/*! {}'.format(desc))
    print_line(out, ' */')
    print_line(out, '{};'.format(signature))
    print_empty_line(out)

# Prototype description.
field_changed_desc = '''/*! Process change of field.
 * \param field_info The information record of the field that changed.
 */
'''
#! Print the declaration of the field_changed() member function:
def print_field_changed_declaration(out, library):
  print_block(out, field_changed_desc)
  if 'SGAL' == library:
    print_line(out, "virtual void field_changed(const Field_info* field_info);")
  else:
    print_line(out, "virtual void field_changed(const SGAL::Field_info* field_info);")
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
  print_hpp_include_directives(config, out, library, derived_class)
  print_forward_declarations(config, out, fields)
  print_line(out, '{}_BEGIN_NAMESPACE'.format(library.upper()))
  print_empty_line(out)

  print_line(out, 'class SGAL_{}_DECL {} : public {} {{'.format(library, class_name, derived_class))
  print_line(out, "public:")
  increase_indent()

  print_friends(config, out)
  print_enumerations(config, out, derived_class, fields)	# enumerations
  print_typedefs(config, out, fields)				# typedefs

  # Print misc. member function declarations:
  print_constructor_declaration(out, class_name, library)
  print_destructor_declaration(out, class_name)
  print_prototype_declaration(out, class_name)
  print_create_declaration(out, library)

  # Print prototype handling declarations:
  print_group(out, "Protoype handlers",
              print_prototype_handling_declarations, library)

  # Print attribute handling declarations:
  print_group(out, "Attribute handlers",
              print_attribute_handling_declaration, library)

  # Print field handling declarations:
  print_group(out, "Field handlers",
              print_fields_handling_declarations, fields, library)

  # Print declarations of field setters and getters:
  print_group(out, "getters & setters",
              print_fields_manipulators_declarations, fields)

  #! Print the declaration of the field_changed() member function:
  if is_geometry_node(config, fields):
    print_field_changed_declaration(out, lbrary)

  # Print additional public functions:
  print_public_function_declarations(config, out)

  decrease_indent()
  print_line(out, "protected:")
  increase_indent()

  # Print protected member function declarations
  print_get_tag_declaration(out, library)
  print_protected_function_declarations(config, out)

  # Print protceted data members
  print_field_data_members(out, fields)
  print_protected_data_members(config, out)
  print_static_protected_data_members_declarations(config, out)

  decrease_indent()
  print_line(out, "private:")
  increase_indent()

  print_tag_data_member(out, library)
  print_prototype_data_member(out, library)
  print_static_member_declarations(out, fields)

  # Print additional private functions:
  print_private_function_declarations(config, out)

  decrease_indent()
  print_line(out, "};")
  print_empty_line(out)

  print_prototype_definition(out, class_name)
  print_create_definition(out, class_name, library)

  # Print declarations of fields manipulators:
  print_hpp_fields_manipulators_definitions(config, out, class_name, fields)

  print_get_tag_definition(out, class_name, library)

  print_empty_line(out)
  print_line(out, '{}_END_NAMESPACE'.format(library.upper()))
  print_empty_line(out)
  print_line(out, "#endif")

#! Generate the .hpp file
#! \param config the configuration dictionary of the node.
#! \param fields the fields of the node.
#! \param output_path the output path.
def generate_hpp_from_path(library, config, fields, output_path):
  class_name = config.get('DEFAULT', 'name')
  derived_class = config.get('class', 'base')

  hpp_filename = os.path.join(output_path, class_name + ".hpp")
  with open(hpp_filename, 'w') as out:
    generate_hpp(library, config, fields, out)

def print_cpp_include_directives(out, fields):
  incs = collections.OrderedDict()
  incs['os'] = OrderedSet()
  incs['stl'] = OrderedSet()
  incs['boost'] = OrderedSet()
  incs['CGAL'] = OrderedSet()
  incs['SGAL'] = OrderedSet()

  # Add include SGAL headers:
  incs['SGAL'].add('SGAL/Element.hpp')
  incs['SGAL'].add('SGAL/Container_proto.hpp')
  incs['SGAL'].add('SGAL/Field_infos.hpp')
  incs['SGAL'].add('SGAL/Field.hpp')

  # Add class header
  if not library in incs:
    incs[library] = OrderedSet()
  incs[library].add('{}/{}.hpp'.format(library, class_name))

  field_names_shared_container = set()
  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name == 'String':
      incs['boost'].add('boost/algorithm/string.hpp')
    elif type_name == 'Boolean':
      incs['SGAL'].add('SGAL/to_boolean.hpp')
    else:
      single_type = get_single_type(type_name)
      general_type = get_general_type(type_name)
      lexical_cast = s_field_lexical_cast[general_type]
      if lexical_cast[0]:
        incs['boost'].add('boost/lexical_cast.hpp')
        if lexical_cast[1]:
          single_type = get_single_type(type_name)
          incs['SGAL'].add('SGAL/io_{}.hpp'.format(single_type.lower()))
          if type.endswith('_array'):
            incs['stl'].add('sstream')
            incs['stl'].add('iterator')
            incs['stl'].add('algorithm')
            incs['SGAL'].add('SGAL/multi_istream_iterator.hpp')

      if single_type.startswith('Shared_'):
        field_class_type = single_type[7:].capitalize()
        namespace = get_type_namespace(field_class_type, library)
        if not namespace in incs:
          incs[namespace] = OrderedSet()
        incs[namespace].add('{}/{}.hpp'.format(namespace, field_class_type))

  # Print
  for key, value in incs.iteritems():
    if 0 != len(value):
      for item in value:
        if key != library:
          print_line(out, '#include <{}>'.format(item))
        else:
          print_line(out, '#include \"{}\"'.format(item))
      print_empty_line(out)

#! Print default values of fields. These values are static data members.
def print_static_member_definitions(out, class_name):
  print_line(out, "// Default values:")
  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if type_name.startswith('Shared_'):
      continue
    name = field['name']
    default_value = field['default-value']
    if default_value:
      print_line(out, 'const {} {}::s_def_{}({});'.format(type, class_name, name, default_value))
    else:
      print_line(out, 'const {} {}::s_def_{};'.format(type, class_name, name))
  print_empty_line(out)

#! Print definitions of static data members.
def print_static_protected_data_members_definitions(config, out, class_name):
  if not config.has_option('class', 'static-protected-data'):
    return
  members = ast.literal_eval(config.get('class', 'static-protected-data'))
  for datum in members:
    desc = config.get(datum, 'desc')
    type = config.get(datum, 'type')
    name = config.get(datum, 'name')
    value = config.get(datum, 'value')
    print_line(out, '//! {}'.format(desc))
    print_line(out, '{} {}::{}({});'.format(type, class_name, name, value))
    print_empty_line(out)

#! Print constructor definition.
def print_constructor_definition(config, out, class_name, derived_class, fields, library):
  print_line(out, '//! \\brief construct from prototype.')
  initializable_fields = []
  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    if not type_name.startswith('Shared_'):
      initializable_fields.append(field)

  if 0 == len(initializable_fields):
    if 'SGAL' == library:
      print_line(out, '{}::{}(Boolean proto) : {}(proto) {{}}'.format(class_name, class_name, derived_class))
    else:
      print_line(out, '{}::{}(SGAL::Boolean proto) : {}(proto) {{}}'.format(class_name, class_name, derived_class))
  else:
    if 'SGAL' == library:
      print_line(out, '{}::{}(Boolean proto) :'.format(class_name, class_name), inc=True)
    else:
      print_line(out, '{}::{}(SGAL::Boolean proto) :'.format(class_name, class_name), inc=True)

    print_line(out, '{}(proto),'.format(derived_class))
    protected_data = {}
    if config.has_option('class', 'protected-data'):
      protected_data = ast.literal_eval(config.get('class', 'protected-data'))
      for datum in protected_data:
        if config.has_option(datum, 'value'):
          name = config.get(datum, 'name')
          value = config.get(datum, 'value')
          print_line(out, '{}({}),'.format(name, value))

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
  array_fields = []
  observed_fields = []
  for field in fields[:]:
    name = field['name']
    type = field['type']
    observed = distutils.util.strtobool(field['observed'])
    if type.endswith('_array'):
      array_fields.append(name)
    if observed:
      observed_fields.append(name)
  if (0 == len(array_fields)) and (0 == len(observed_fields)):
    print_line(out, "%s::~%s() {}" % (class_name, class_name))
    print_empty_line(out)
    return

  print_line(out, "%s::~%s()" % (class_name, class_name))
  print_line(out, "{", inc=True)
  for name in array_fields[:]:
    print_line(out, 'm_{}.clear();'.format(name))
  for name in observed_fields:
    print_line(out, 'get_field_info({})->detach(this);'.format(name.upper()))
  print_line(out, "}", dec=True)
  print_empty_line(out)

def get_execution_function(config, field):
  exec_function = field['execution-function']
  if (exec_function):
    return exec_function
  if config.has_option('fields', 'execution-function'):
    return config.get('fields', 'execution-function')
  return ''

def print_init_prototype_definition(config, out, class_name, derived_class, fields, library):
  print_line(out, "//! \\brief initilalizes the container prototype.")
  print_line(out, "void %s::init_prototype()" % class_name)
  print_line(out, "{", inc=True)

  print_line(out, "if (s_prototype) return;")
  if 'SGAL' == library:
    print_line(out, 's_prototype = new Container_proto({}::get_prototype());'.format(derived_class))
  else:
    print_line(out, 's_prototype = new SGAL::Container_proto({}::get_prototype());'.format(derived_class))
  print_empty_line(out)
  print_line(out, "// Add the field-info records to the prototype:")
  # print_line(out, "auto exec_func = static_cast<SGAL::Execution_function>(&%s::structure_changed);" % class_name)
  default_exec_function = ''
  if config.has_option('fields', 'execution-function'):
    default_exec_function = config.get('fields', 'execution-function')
  if default_exec_function:
    ef_name, ef_scope_name = get_type_attributes(default_exec_function, class_name)
    if 'SGAL' == library:
      print_line(out, 'auto exec_func = static_cast<Execution_function>(&{}::{});'.format(ef_scope_name, ef_name))
    else:
      print_line(out, 'auto exec_func = static_cast<SGAL::Execution_function>(&{}::{});'.format(ef_scope_name, ef_name))

  for field in fields[:]:
    name = field['name']
    type = field['type']
    rule = field['rule']
    default_value = field['default-value']
    exec_function = field['execution-function']
    tmp = name.replace('_', ' ').title().replace(' ', '')
    field_name = tmp[:1].lower() + tmp[1:]
    type_name, type_namespace = get_type_attributes(type, library)
    general_type = get_general_type(type_name)
    field_handle_type = get_field_handle_type(general_type)
    print_empty_line(out)
    print_line(out, "// %s" % field_name)
    if (exec_function):
      ef_name, ef_scope_name = get_type_attributes(exec_function, class_name)
      if 'SGAL' == library:
        print_line(out, 'auto {}_exec_func = static_cast<Execution_function>(&{}::{});'.format(name, ef_scope_name, ef_name))
      else:
        print_line(out, 'auto {}_exec_func = static_cast<SGAL::Execution_function>(&{}::{});'.format(name, ef_scope_name, ef_name))
    print_line(out, 'auto {}_func = reinterpret_cast<{}>(&{}::{}_handle);'.format(name, field_handle_type, class_name, name))
    field_enum = name.upper()
    field_type = get_field_type(general_type)
    if 'SGAL' == library:
      statement = 's_prototype->add_field_info(new {}({}, \"{}\",  Field_rule::RULE_{}, {}_func'.format(field_type, field_enum, field_name, rule, name)
    else:
      statement = 's_prototype->add_field_info(new {}({}, \"{}\",  SGAL::Field_rule::RULE_{}, {}_func'.format(field_type, field_enum, field_name, rule, name)
    if (default_value):
      statement = statement + ', s_def_{}'.format(name)
    if (exec_function):
      statement = statement + ', {}_exec_func'.format(name)
    elif (default_exec_function):
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

def print_get_prototype_definition(out, library):
  print_line(out, "//! \\brief obtains the node prototype.")
  if 'SGAL' == library:
    print_line(out, 'Container_proto* {}::get_prototype()'.format(class_name))
  else:
    print_line(out, 'SGAL::Container_proto* {}::get_prototype()'.format(class_name))
  print_line(out, "{", inc=True)
  print_line(out, 'if (s_prototype == nullptr) {}::init_prototype();'.format(class_name));
  print_line(out, "return s_prototype;")
  print_line(out, "}", dec=True)
  print_empty_line(out)

#! Print the definitions of the prototype handling functions.
def print_prototype_handling_definitions(config, out, class_name, derived_class, fields, library):
  print_init_prototype_definition(config, out, class_name, derived_class, fields, library)
  print_empty_line(out)
  print_delete_prototype_definition(out)
  print_empty_line(out)
  print_get_prototype_definition(out, library)

def print_set_field(config, out, field, iterator, op):
  name = field['name']
  type = field['type']
  tmp = name.replace('_', ' ').title().replace(' ', '')
  field_name = tmp[:1].lower() + tmp[1:]
  print_line(out, 'if (name == \"{}\") {{'.format(field_name), inc=True)
  op(config, out, field)
  print_line(out, 'elem->mark_delete({});'.format(iterator))
  print_line(out, "continue;")
  print_line(out, "}", dec=True)

def print_set_field_from_string(config, out, field):
  name = field['name']
  type = field['type']
  type_name, type_namespace = get_type_attributes(type, library)
  if type_name == 'String':
    print_line(out, 'set_{}(boost::algorithm::trim_copy(value));'.format(name))
    return

  if type_name == 'Boolean':
    if 'SGAL' == library:
      print_line(out, 'set_{}(to_boolean(value));'.format(name))
    else:
      print_line(out, 'set_{}(SGAL::to_boolean(value));'.format(name))
    return

  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
  lexical_cast = s_field_lexical_cast[general_type]
  if not lexical_cast[0]:
    raise Exception('I do not know how to set field {} of type {} from a string!'.format(name, type))
  if not type.endswith('_array'):
    # The source type, i.e., 'const String&' must be explicitly specified for
    # certain types, e.g., Boolean; otherwise, in cases where the cast for the
    # specific type is provided by the SGAL namespace, the lexical-cast for the
    # type defined in the 'boost' namespace is used (instead of the one defined
    # in the SGAL namespace.
    print_line(out, 'set_{}(boost::lexical_cast<{}>(value));'.format(name, type))
    return

  print_line(out, 'std::stringstream ss(value);')
  single_type = get_single_type(type)
  if lexical_cast[1]:
    size = s_field_size[get_single_type(type_name)]
    statement = '''std::transform(multi_istream_iterator<{}>(ss),
                     multi_istream_iterator<{}>(),
                     std::back_inserter(m_{}),
                     &boost::lexical_cast<{}>);'''.format(size, size, name, single_type)
  else:
    statement = '''std::copy(std::istream_iterator<{}>(ss),
                std::istream_iterator<{}>(),
                std::back_inserter(m_{}));'''.format(single_type, single_type, name)
  print_call(out, statement)

#! Print the code that handles a multi-string attribute.
def print_set_field_from_multi_string(config, out, field):
  name = field['name']
  print_line(out, 'm_{}.resize(value.size());'.format(name))
  print_line(out, 'auto tit = m_{}.begin();'.format(name))
  print_line(out, 'for (auto sit : value) *tit++ = std::move(*sit);')
  # The above two line could be replaced by the single statement below using
  # C++17. It requires 'auto' of formal variable of lambda functions.
  # std::transform(value.begin(), value.end(), m_{}.begin(),
  #                [](auto s) -> std::string { return std::move(*s); });
  exec_func = get_execution_function(config, field)
  if exec_func:
    print_line(out, '{}(get_field_info({}));'.format(exec_func, name.upper()))

#! Print the code that handles a container attribute.
def print_set_field_from_container(config, out, field):
  name = field['name']
  type = field['type']
  type_name, type_namespace = get_type_attributes(type, library)
  field_class_type = get_complete_type(type_name[7:].capitalize(), library)
  print_line(out, 'set_{}(boost::dynamic_pointer_cast<{}>(cont));'.format(name, field_class_type))

def print_set_field_from_multi_container(config, out, field):
  name = field['name']
  type = field['type']
  type_name, type_namespace = get_type_attributes(type, library)
  print_line(out, "for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {", inc=True)
  field_class_type = get_complete_type(type_name[7:][:-6].capitalize(), library)
  print_line(out, 'add_{}(boost::dynamic_pointer_cast<{}>(*ci));'.format(name[:-1], field_class_type))
  print_line(out, "}", dec=True)

def print_set_attributes_definition(config, out, class_name, derived_class, fields):
  print_line(out, "//! \\brief sets the attributes of this node.")
  if 'SGAL' == library:
    print_line(out, 'void {}::set_attributes(Element* elem)'.format(class_name))
  else:
    print_line(out, 'void {}::set_attributes(SGAL::Element* elem)'.format(class_name))
  print_line(out, "{", inc=True)
  print_line(out, '{}::set_attributes(elem);'.format(derived_class))
  print_empty_line(out)

  string_fields = []
  multi_string_fields = []
  container_fields = []
  multi_container_fields = []
  for field in fields[:]:
    type = field['type']
    type_name, type_namespace = get_type_attributes(type, library)
    general_type = get_general_type(type_name)
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
      print_set_field(config, out, field, 'ai', print_set_field_from_string)
    print_line(out, "}", dec=True)
    print_empty_line(out)

  # Multi-string fields:
  if 0 < len(multi_string_fields):
    print_line(out, "for (auto msai = elem->multi_str_attrs_begin(); msai != elem->multi_str_attrs_end(); ++msai) {", inc=True)
    print_line(out, "const auto& name = elem->get_name(msai);")
    print_line(out, "auto& value = elem->get_value(msai);")
    for field in multi_string_fields[:]:
      print_set_field(config, out, field, 'msai', print_set_field_from_multi_string)
    print_line(out, "}", dec=True)
    print_empty_line(out)

  # Container fields:
  if 0 < len(container_fields):
    print_line(out, "for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {", inc=True)
    print_line(out, "const auto& name = elem->get_name(cai);")
    print_line(out, "auto cont = elem->get_value(cai);")

    for field in container_fields[:]:
      print_set_field(config, out, field, 'cai', print_set_field_from_container)
    print_line(out, "}", dec=True)

  #! Multi-container fields
  if 0 < len(multi_container_fields):
    print_line(out, "for (auto mcai = elem->multi_cont_attrs_begin(); mcai != elem->multi_cont_attrs_end(); ++mcai) {", inc=True)
    print_line(out, "const auto& name = elem->get_name(mcai);")
    print_line(out, "auto& cont_list = elem->get_value(mcai);")
    for field in multi_container_fields[:]:
      print_set_field(config, out, field, 'mcai', print_set_field_from_multi_container)
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
def print_cpp_field_adder_definition(out, class_name, field, library):
  type = field['type']
  name = field['name']
  desc = field['desc']
  type_name, type_namespace = get_type_attributes(type, library)
  print_line(out, '//! \\brief adds a {} to the array of {}..'.format(desc[:-1], desc))
  print_line(out, 'void {}::add_{}({} {})'.format(class_name, name[:-1], type_name[:-6], name[:-1]))
  print_line(out, "{", inc=True)
  print_line(out , 'm_{}.push_back({});'.format(name, name[:-1]))
  print_register_observer(out, field, single=True)
  print_line(out, "}", dec=True)

#! Print the setter, getter, adder, and remover function definition.
# As a convension, if the field has an execution function, the
# definition of the corresponding setter, adder, remover are placed in the
# cpp file.
def print_cpp_field_manipulators_definitions(config, out, class_name, field, library):
  geometry = field['geometry']
  exec_func = get_execution_function(config, field)
  clean_func = field['clean-function']
  if exec_func:
    print_field_setter_definition(config, out, False, class_name, field)
    print_empty_line(out)
  type = field['type']
  type_name, type_namespace = get_type_attributes(type, library)
  general_type = get_general_type(type_name)
  field_element_type = s_field_element_type[general_type]
  if geometry and field_element_type == 'multi-container':
    print_cpp_field_adder_definition(out, class_name, field, library)
    print_empty_line(out)
  if clean_func:
    print_field_getter_definition(out, False, class_name, field)
    print_empty_line(out)

#! Print declarations of field setters and getters:
def print_cpp_fields_manipulators_definitions(config, out, class_name, fields, library):
  for field in fields[:]:
    print_cpp_field_manipulators_definitions(config, out, class_name, field, library)

 #! Print the definition of the field_changed() member function.
def print_field_changed_definition(out, class_name, library):
  print_line(out, "//! \\brief processes change of field.")
  if 'SGAL' == library:
    print_line(out, 'void {}::field_changed(const Field_info* field_info)'.format(class_name))
  else:
    print_line(out, 'void {}::field_changed(const SGAL::Field_info* field_info)'.format(class_name))
  print_line(out, "{", inc=True)
  print_line(out, "switch (field_info->get_id()) {", inc=True)
  print_line(out, "case BOUNDING_SPHERE: m_dirty_bounding_sphere = true; break;")
  print_line(out, "default: break;")
  print_line(out, "}", dec=True)
  if 'SGAL' == library:
    print_line(out, "Node::field_changed(field_info);")
  else:
    print_line(out, "SGAL::Node::field_changed(field_info);")
  print_line(out, "}", dec=True)
  print_empty_line(out)

#! Generate the .cpp file
#! \param config
#! \param fields
#! \param out
def generate_cpp(library, config, fields, out):
  print_line(out, copyright_text)
  print_line(out, "// Generated by " + os.path.basename(__file__))
  print_empty_line(out)
  print_cpp_include_directives(out, fields)
  print_line(out, '{}_BEGIN_NAMESPACE'.format(library.upper()))
  print_empty_line(out)

  # Print tag definition:
  # 1. Split class name to single words (separated by '_'),
  # 2. convert first character of each word to uppper case, and
  # 3. re-concatenate.
  node_name = class_name.replace('_', ' ').title().replace(' ', '')
  if 'SGAL' == library:
    print_line(out, 'const String {}::s_tag = \"{}\";'.format(class_name, node_name))
  else:
    print_line(out, 'const SGAL::String {}::s_tag = \"{}\";'.format(class_name, node_name))

  # Print prototype defintion:
  if 'SGAL' == library:
    print_line(out, "Container_proto* %s::s_prototype(nullptr);" % class_name)
  else:
    print_line(out, "SGAL::Container_proto* %s::s_prototype(nullptr);" % class_name)
  print_empty_line(out)

  print_static_member_definitions(out, class_name)
  print_static_protected_data_members_definitions(config, out, class_name)
  print_constructor_definition(config, out, class_name, derived_class, fields, library)
  print_destructor_definition(out, class_name, fields)
  print_prototype_handling_definitions(config, out, class_name, derived_class, fields, library)
  print_set_attributes_definition(config, out, class_name, derived_class, fields)

  # Print getters, setters, adders, and removers:
  cpp_fields = []
  for field in fields[:]:
    exec_func = get_execution_function(config, field)
    if exec_func:
      cpp_fields.append(field)

  if 0 < len(cpp_fields):
    print_cpp_fields_manipulators_definitions(config, out, class_name, cpp_fields, library)
    # print_cpp_field_draw_definitions()
    # print_cpp_field_cull_definitions()
    # print_cpp_field_isect_definitions()

  if is_geometry_node(config, fields):
    print_field_changed_definition(out, class_name, library)

  print_line(out, '{}_END_NAMESPACE'.format(library.upper()))

#! Generate the .cpp file
#! \param config
#! \param fields
#! \param output_path
def generate_cpp_from_path(library, config, fields, output_path):
  class_name = config.get('DEFAULT', 'name')
  derived_class = config.get('class', 'base')

  hpp_filename = os.path.join(output_path, class_name + ".cpp")
  with open(hpp_filename, 'w') as out:
    generate_cpp(library, config, fields, out)

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
    derived_class = config.get('class', 'base')
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
        exec_function = vals[4]
        clean_function = vals[5]
        observed = vals[6]
        desc = vals[7].strip('\"')
        # Assume that a type that starts with 'Shared' is either a shared
        # container or a shared container array.
        type_name, type_namespace = get_type_attributes(type, library)
        if not type_name.startswith('Shared_'):
          if not type_name in s_field_type:
            raise Exception('Type {} of field \"{}::{}\" is unknown!'.format(type, class_name, name))
        field = {
          'name': name,
          'type': type,
          'rule': rule,
          'default-value': default_value,
          'execution-function': exec_function,
          'clean-function': clean_function,
          'observed': observed,
          'desc': desc,
          'geometry': False
        }
        fields.append(field)

      # map(lambda x: print(x), fields)
      generate_hpp_from_path(library, config, fields, args.output_inc_path)
      generate_cpp_from_path(library, config, fields, args.output_lib_path)

  exit()
