#!/usr/bin/python

from __future__ import print_function
import os.path
import argparse
import json

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

#! Increase the indentation level.
def increase_indent():
  global indent
  indent += delta

#! Decrease the indentation level.
def decrease_indent():
  global indent
  indent -= delta

#! Print a single indented line.
def print_line(out, line, eol=True, ind='0'):
  if (ind == '+'): increase_indent();
  print ("%s%s" % (indent * ' ', line), file=out, end='\n' if eol else '')
  if (ind == '-'): decrease_indent();

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
    fnc(out);
  else:
    fnc(out, args);
  print_line(out, "//@}")
  print_empty_line(out);

#! Print enumerations.
def print_field_enumeration(out, derived_class_name, fields):
  print_line(out, "enum {")
  print_line(out, "FIRST = %s::LAST - 1," % derived_class_name, ind='+')
  for field in fields[:]:
    print_line(out, field['name'].upper() + ',')
  print_line(out, "LAST", ind='-')
  print_line(out, "};")
  print_empty_line(out);

# Constructor description.
constructor_desc = '''/*! Construct from prototype.
 * \\param proto indicates whether a prototype should be constructed.
 */
'''

#! Print the constructor declaration.
def print_constructor_declaration(out, name):
  print_block(out, constructor_desc);
  print_line(out, "%s(Boolean proto = false);" % name);
  print_empty_line(out);

#! Print the destructor declaration.
def print_destructor_declaration(out, name):
  print_line(out, "/*! Destruct. */");
  print_line(out, "virtual ~%s();" % name);
  print_empty_line(out);

# Prototype description.
prototype_desc = '''/*! Construct the prototype.
 * \\return the prototype.
 */
'''

#! Print prototype() declaration.
def print_prototype_declaration(out, name):
  print_block(out, prototype_desc);
  print_line(out, "static %s* prototype();" % name);
  print_empty_line(out);

# Clone description.
clone_desc = '''/*! Clone.
 * \\return the cloned container.
 */
'''

#! Print clone() declaration.
def print_clone_declaration(out):
  print_block(out, clone_desc);
  print_line(out, "virtual Container* clone();");
  print_empty_line(out);

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
def print_prototype_handling_declaration(out):
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

#! Handle field setter & getter declarations
def print_field_setter_getter_declarations(out, field, last=False):
  type = field['type']
  name = field['name']
  pass_method = field['pass_method']
  compound = field['compound']
  desc = field['desc']
  # Setter
  print_line(out, "/*! Set the %s. */" % desc)
  if pass_method == 'value':
    print_line(out, "void set_%s(%s %s);" % (name, type, name));
  elif pass_method == 'reference':
    print_line(out, "void set_%s(const %s& %s);" % (name, type, name));
  else:
    raise Exception("Pass method %s is invalid!" % pass_method)

  print_empty_line(out)
  # Getter
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
  if not last: print_empty_line(out);

#! Print getters and setters.
def print_getters_setters_declaration(out, args):
  fields = args[0]
  for field in fields[:-1]:
    print_field_setter_getter_declarations(out, field)
  field = fields[-1]
  print_field_setter_getter_declarations(out, field, last=True)

#! Generate the .hpp file
#! \param name
#! \param derived_class_name
#! \param fields
def generate_hpp(name, derived_class_name, fields):
  hpp_filename = os.path.join(name + ".hpp")
  with open(hpp_filename, 'w') as out:
    print_line(out, copyright_text)
    print_line(out, "// Generated by " + os.path.basename(__file__))
    print_empty_line(out)
    print_line(out, "#ifndef FPG_%s_HPP" % name.upper())
    print_line(out, "#define FPG_%s_HPP" % name.upper())
    print_empty_line(out)
    print_line(out, "#include \"SGAL/basic.hpp\"")
    print_line(out, "#include \"SGAL/Types.hpp\"")
    print_empty_line(out)
    print_line(out, "#include \"FPG/basic.hpp\"")
    print_empty_line(out)
    print_line(out, "SGAL_BEGIN_NAMESPACE")
    print_empty_line(out)

    print_line(out, "class SGAL_FPG_DECL %s : public %s {" % (name, derived_class_name))
    print_line(out, "public:")
    increase_indent();

    # Print field enumeration:
    print_field_enumeration(out, derived_class_name, fields)

    # Print misc. member function declarations:
    print_constructor_declaration(out, name)
    print_destructor_declaration(out, name)
    print_prototype_declaration(out, name)
    print_clone_declaration(out)

    # Print prototype handling declarations:
    print_group(out, "Protoype handlers",
                print_prototype_handling_declaration)

    # Print attribute handling declarations:
    print_group(out, "Attribute handlers",
                print_attribute_handlin_declaration)

    # Print field handling declarations:
    print_group(out, "Field handlers",
                print_field_handlin_declaration, fields)

    # Print declarations of field setters and getters:
    print_group(out, "/// \\name getters & setters",
                print_getters_setters_declaration, fields)

    decrease_indent();
    print_line(out, "};")

    print_empty_line(out)
    print_line(out, "SGAL_END_NAMESPACE")
    print_empty_line(out)
    print_line(out, "#endif")

# Main function
if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Generate code of nodes.')
  parser.add_argument('filename', metavar="filename", nargs='?',
                      help='the specification file name')
  parser.add_argument('--input-path', type=readable_dir, nargs='*',
                      dest="input_paths", default='./')
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
      vals = line.split(',')
      vals = map(lambda s: s.strip(), vals)
      name = vals[0]
      type = vals[1]
      pass_method = vals[2]
      compound = True if vals[3] == 1 else False
      default_value = vals[4]
      desc = vals[5].strip('\"')
      field = {
        'name': name,
        'type': type,
        'pass_method': pass_method,
        'compound': compound,
        'default': default_value,
        'desc': desc
      }
      fields.append(field)

    # map(lambda x: print(x), fields)
    generate_hpp(class_name, derived_class_name, fields)

  exit()
