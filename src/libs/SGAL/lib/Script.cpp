// Copyright (c) 2014 Israel.
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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <regex>
#include <boost/lexical_cast.hpp>

#include <v8.h>

#include "SGAL/basic.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Script.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Field.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Script::s_tag = "Script";
Container_proto* Script::s_prototype(nullptr);

REGISTER_TO_FACTORY(Script, "Script");

/*! Constructor */
Script::Script(Boolean proto) :
  Script_base(proto)
{ }

/*! Destructor */
Script::~Script() { }

/*! \brief initializes the container prototype. */
void Script::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Script_base::get_prototype());
}

/*! \brief deletes the container prototype. */
void Script::delete_prototype()
{
  if (!s_prototype) return;
  delete s_prototype;
  s_prototype = nullptr;
}

/*! \brief obtains the container prototype. */
Container_proto* Script::get_prototype()
{
  if (!s_prototype) Script::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Script::set_attributes(Element* elem)
{
  Script_base::set_attributes(elem);

  Element::Field_attr_iter fi;
  for (fi = elem->field_attrs_begin(); fi != elem->field_attrs_end(); ++fi) {
    Field_rule rule = elem->get_rule(fi);
    Field_type type = elem->get_type(fi);
    const std::string& name = elem->get_name(fi);
    const std::string& value = elem->get_value(fi);
    add_field_info(rule, type, name, value);
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Script::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Script_base::get_attributes();

  attrib.first = "url";
  attrib.second = m_url;
  attribs.push_back(attrib);

  return attribs;
}
#endif

/*! \brief records the scene graph. */
void Script::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

/*! \brief adds a field info record to the script node. */
void Script::add_field_info(Field_rule rule, Field_type type,
                            const std::string& name, const std::string& value)
{
  Container_proto* prototype = get_prototype();

  Execution_function exec_func =  (rule == RULE_IN) ?
    static_cast<Execution_function>(&Script::execute) : nullptr;

  Variant_field variant_field;
  Uint id = LAST + m_fields.size();
  switch (type) {
   case SF_BOOL:
    {
     auto initial_value =
       value.empty() ? false : boost::lexical_cast<Boolean>(value);
     variant_field = initial_value;
     add_fi<SF_BOOL>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_FLOAT:
    {
     auto initial_value = value.empty() ? 0 : boost::lexical_cast<Float>(value);
     variant_field = initial_value;
     add_fi<SF_FLOAT>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_TIME:
    {
     auto initial_value =
       value.empty() ? 0 : boost::lexical_cast<Scene_time>(value);
     variant_field = initial_value;
     add_fi<SF_TIME>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_INT32:
    {
     auto initial_value = value.empty() ? 0 : boost::lexical_cast<Int>(value);
     variant_field = initial_value;
     add_fi<SF_INT32>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_VEC2F:
    {
     Vector2f initial_value(value);
     variant_field = initial_value;
     add_fi<SF_VEC2F>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_VEC3F:
    {
     Vector3f initial_value(value);
     variant_field = initial_value;
     add_fi<SF_VEC3F>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_COLOR:
    {
     Vector3f initial_value(value);
     variant_field = initial_value;
     add_fi<SF_COLOR>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_ROTATION:
    {
     Rotation initial_value(value);
     variant_field = initial_value;
     add_fi<SF_ROTATION>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_STR:
    {
     std::string initial_value(value);
     variant_field = initial_value;
     add_fi<SF_STR>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case SF_SHARED_CONTAINER:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_FLOAT:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_INT32:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_VEC2F:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_VEC3F:
   case MF_COLOR:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_ROTATION:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_TIME:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_STR:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_IMAGE:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_SHARED_CONTAINER:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   default:
    std::cerr << "Unrecognized type (" << type << ")!" << std::endl;
    break;
  }

  m_fields.push_back(variant_field);
}

// \brief executes the suitable script function according to the event.
void Script::execute(const Field_info* field_info)
{
  // Get the Isolate instance of the V8 engine from the scene graph.
  v8::Isolate* isolate = m_scene_graph->get_isolate();
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);        // stack-allocated handle scope

  // Create a new ObjectTemplate
  v8::Handle<v8::ObjectTemplate> global_tmpl =
    v8::ObjectTemplate::New(isolate);

  Container_proto* proto = get_prototype();
  Container_proto::Id_const_iterator it = proto->ids_begin(proto);

  v8::Handle<v8::Context> context =
    v8::Context::New(isolate, NULL, global_tmpl);
  v8::Context::Scope context_scope(context);    // enter the contex
  v8::Handle<v8::Object> global = context->Global();

  // Add an object for every non-scalar output field.
  for (it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if ((field_info->get_rule() != RULE_OUT) || field_info->is_scalar())
      continue;

    v8::Handle<v8::String> field_name =
      v8::String::NewFromUtf8(isolate, field_info->get_name().c_str(),
                              v8::String::kInternalizedString);
    v8::Handle<v8::ObjectTemplate> field_tmpl =
      v8::ObjectTemplate::New(isolate);
    v8::Handle<v8::Object> field_obj = field_tmpl->NewInstance();
    global->Set(field_name, field_obj);
  }

  // Add an object for every field.
  for (it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if (field_info->get_rule() != RULE_FIELD) continue;

    v8::Handle<v8::String> field_name =
      v8::String::NewFromUtf8(isolate, field_info->get_name().c_str(),
                              v8::String::kInternalizedString);
    Field* field = get_field(field_info);
    //! \todo need to introduce fields...
    if (!field) continue;

    // In the following we extract the value through a temporary value holder.
    // Another option is to first extract values of static field and then
    // extract values of dynamic fields, where the latter can be done by
    // directly using field_handle(field_info)
    switch (field_info->get_type_id()) {
     case SF_BOOL:
      {
       Boolean tmp;
       Value_holder<Boolean> vh_tmp(&tmp);
       Value_holder_base* vh_field = field->get_value_holder();
       vh_tmp.delegate(*vh_field);
       bool* value_handle = vh_tmp.get_value();
       global->Set(field_name, v8::Boolean::New(isolate, *value_handle));
      }
      break;

     // case SF_FLOAT:
     //  args[0] = v8::Number::New(isolate, *(field_handle<Float>(field_info)));
     //  break;

     // case SF_TIME:
     //  args[0] = v8::Number::New(isolate, *(field_handle<Scene_time>(field_info)));
     //  break;

     // case SF_INT32:
     //  args[0] = v8::Int32::New(isolate, *(field_handle<Int>(field_info)));
     //  break;

     // case SF_STR:
     //  {
     //   const std::string* tmp = field_handle<std::string>(field_info);
     //   args[0] = v8::String::NewFromUtf8(isolate, tmp->c_str());
     //  }
     //  break;

     // case SF_VEC2F:
     //  {
     //   const Vector2f* tmp = field_handle<Vector2f>(field_info);
     //   v8::Handle<v8::Array> array = v8::Array::New(isolate, 2);
     //   if (array.IsEmpty()) {
     //     std::cerr << "failed to allocate v8 Array!" << std::endl;
     //     break;
     //   }
     //   array->Set(0, v8::Number::New(isolate, (*tmp)[0]));
     //   array->Set(1, v8::Number::New(isolate, (*tmp)[1]));
     //   args[0] = array;
     //  }
     //  break;

     // case SF_VEC3F:
     // case SF_COLOR:
     //  {
     //   const Vector3f* tmp = field_handle<Vector3f>(field_info);
     //   v8::Handle<v8::Array> array = v8::Array::New(isolate, 3);
     //   if (array.IsEmpty()) {
     //     std::cerr << "failed to allocate v8 Array!" << std::endl;
     //     break;
     //   }
     //   array->Set(0, v8::Number::New(isolate, (*tmp)[0]));
     //   array->Set(1, v8::Number::New(isolate, (*tmp)[1]));
     //   array->Set(2, v8::Number::New(isolate, (*tmp)[2]));
     //   args[0] = array;
     //  }
     //  break;

     case SF_ROTATION:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case SF_SHARED_CONTAINER:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_FLOAT:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_INT32:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_VEC2F:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_VEC3F:
     case MF_COLOR:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_ROTATION:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_TIME:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_STR:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case SF_IMAGE:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_SHARED_CONTAINER:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     default:
      std::cerr << "Unsupported type!" << std::endl;
      return;
    }
  }

  const std::string& str = field_info->get_name();
  v8::Handle<v8::String> name = v8::String::NewFromUtf8(isolate, str.c_str());

  // Extract the script
  std::cout << "Extracting" << std::endl;
  size_t pos = m_url.find(':');
  if (std::string::npos == pos) {
    std::cerr << "Invalid script!" << std::endl;
    return;
  }
  if (m_url.compare(0, pos, "javascript") == 0)
    m_protocol = PROTOCOL_CUSTOM_ECMASCRIPT;
  else if (std::regex_match(m_url, std::regex("http://.*\\.js")))
    m_protocol = PROTOCOL_ECMASCRIPT;
  else if (std::regex_match(m_url, std::regex("http://.*\\.class")))
    m_protocol = PROTOCOL_JAVA_BYTECODE;
  else {
    std::cerr << "Invalid script!" << std::endl;
    return;
  }

  pos = m_url.find_first_not_of(" \t\r\n", pos + 1);
  std::string source_str = m_url.substr(pos);

  // std::cout << "source_str.c_str(): " << source_str.c_str() << std::endl;

  // Create a string containing the JavaScript source code.
  v8::Handle<v8::String> source =
    v8::String::NewFromUtf8(isolate, source_str.c_str());

  // set up an error handler to catch any exceptions the script might throw.
  v8::TryCatch try_catch;

  v8::Handle<v8::Script> script = v8::Script::Compile(source);
  if (script.IsEmpty()) {
    // The script failed to compile; bail out.
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return;
  }

  v8::Handle<v8::Value> result = script->Run(); // run the script
  if (result.IsEmpty()) {
    // The TryCatch above is still in effect and will have caught the error.
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return;
  }

  v8::Handle<v8::Value> value = global->Get(name);

  // If there is no such function, bail out.
  if (!value->IsFunction()) {
    std::cerr << "A function named " << str.c_str() << " does not exist!"
              << std::endl;
    return;
  }

  v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  v8::Handle<v8::Value> args[2];
  switch (field_info->get_type_id()) {
   case SF_BOOL:
    args[0] = v8::Boolean::New(isolate, *(field_handle<Boolean>(field_info)));
    break;

   case SF_FLOAT:
    args[0] = v8::Number::New(isolate, *(field_handle<Float>(field_info)));
    break;

   case SF_TIME:
    args[0] = v8::Number::New(isolate, *(field_handle<Scene_time>(field_info)));
    break;

   case SF_INT32:
    args[0] = v8::Int32::New(isolate, *(field_handle<Int>(field_info)));
    break;

   case SF_VEC2F:
    {
     const Vector2f* tmp = field_handle<Vector2f>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, 2);
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     array->Set(0, v8::Number::New(isolate, (*tmp)[0]));
     array->Set(1, v8::Number::New(isolate, (*tmp)[1]));
     args[0] = array;
    }
    break;

   case SF_VEC3F:
   case SF_COLOR:
    {
     const Vector3f* tmp = field_handle<Vector3f>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, 3);
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     array->Set(0, v8::Number::New(isolate, (*tmp)[0]));
     array->Set(1, v8::Number::New(isolate, (*tmp)[1]));
     array->Set(2, v8::Number::New(isolate, (*tmp)[2]));
     args[0] = array;
    }
    break;

   case SF_ROTATION:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_STR:
    {
     const std::string* tmp = field_handle<std::string>(field_info);
     args[0] = v8::String::NewFromUtf8(isolate, tmp->c_str());
    }
    break;

   case SF_SHARED_CONTAINER:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_FLOAT:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_INT32:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_VEC2F:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_VEC3F:
   case MF_COLOR:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_ROTATION:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_TIME:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_STR:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_IMAGE:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_SHARED_CONTAINER:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   default:
    std::cerr << "Unsupported type!" << std::endl;
    return;
  }
  args[1] = v8::Number::New(isolate, m_time);

  v8::Handle<v8::Value> func_result = func->Call(global, 2, args);
  if (func_result.IsEmpty()) {
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return;
  }

  for (it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if (field_info->get_rule() == RULE_OUT) {
      v8::Handle<v8::String> field_name =
        v8::String::NewFromUtf8(isolate, field_info->get_name().c_str(),
                                v8::String::kInternalizedString);
      v8::Handle<v8::Value> value = global->Get(field_name);

      // Ignore any scalar field that has not been assigned by the script.
      if (value->IsUndefined()) continue;

      Field* field = get_field(field_info);

      // Ignore any field that has not been routed out.
      if (!field) continue;

      switch (field_info->get_type_id()) {
       case SF_BOOL:
        {
         Boolean tmp = value->BooleanValue();
         Value_holder<Boolean> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_FLOAT:
        {
         Float tmp = static_cast<Float>(value->NumberValue());
         if (std::isnan(tmp)) continue;         // ignore the field if NaN
         Value_holder<Float> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_TIME:
        {
         Scene_time tmp = value->NumberValue();
         if (std::isnan(tmp)) continue;         // ignore the field if NaN
         Value_holder<Scene_time> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_INT32:
        {
         Int tmp = value->Int32Value();
         Value_holder<Int> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_STR:
        {
         v8::Local<v8::String> str = v8::Handle<v8::String>::Cast(value);
         v8::String::Utf8Value utf8(str);
         std::string tmp(*utf8);
         Value_holder<std::string> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_VEC2F:
        {
         v8::HandleScope scope(isolate);
         v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
         Vector2f tmp(static_cast<Float>(array->Get(0)->NumberValue()),
                      static_cast<Float>(array->Get(1)->NumberValue()));

        // Ignore the field if any one of the values is NaN
         if (std::isnan(tmp[0]) || std::isnan(tmp[1])) continue;

         Value_holder<Vector2f> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_VEC3F:
       case SF_COLOR:
        {
         v8::HandleScope scope(isolate);
         v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
         Vector3f tmp(static_cast<Float>(array->Get(0)->NumberValue()),
                      static_cast<Float>(array->Get(1)->NumberValue()),
                      static_cast<Float>(array->Get(2)->NumberValue()));

         // Ignore the field if any one of the values is NaN
         if (std::isnan(tmp[0]) || std::isnan(tmp[1]) || std::isnan(tmp[0]))
           continue;

         Value_holder<Vector3f> value_holder(&tmp);
         (field->get_value_holder())->delegate(value_holder);
        }
        break;

       case SF_ROTATION:
       case SF_SHARED_CONTAINER:
       case MF_FLOAT:
       case MF_INT32:
       case MF_VEC2F:
       case MF_VEC3F:
       case MF_ROTATION:
       case MF_TIME:
       case MF_STR:
       case SF_IMAGE:
       case MF_SHARED_CONTAINER:
        std::cerr << "Not supported yet!" << std::endl;
        break;

       default:
        std::cerr << "Unsupported type!" << std::endl;
        return;
      }
      field->cascade();
    }
  }
}

SGAL_END_NAMESPACE
