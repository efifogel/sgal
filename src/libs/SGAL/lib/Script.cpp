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
std::vector<Container_proto*> Script::s_prototypes;

REGISTER_TO_FACTORY(Script, "Script");

/*! Constructor */
Script::Script(Boolean proto) :
  Script_base(proto)
{
  if (proto) return;
  size_t id = s_prototypes.size();
  m_id = id;
  s_prototypes.resize(id+1);
  s_prototypes[id] = nullptr;
}

/*! Destructor */
Script::~Script() { }

/*! \brief initializes the container prototype. */
void Script::init_prototype()
{
  if (s_prototypes[m_id]) return;
  s_prototypes[m_id] = new Container_proto(Script_base::get_prototype());
}

/*! \brief deletes the container prototype. */
void Script::delete_prototype()
{
  if (!s_prototypes[m_id]) return;
  delete s_prototypes[m_id];
  s_prototypes[m_id] = nullptr;
}

/*! \brief obtains the container prototype. */
Container_proto* Script::get_prototype()
{
  if (!s_prototypes[m_id]) Script::init_prototype();
  return s_prototypes[m_id];
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

   case SF_IMAGE:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_SHARED_CONTAINER:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_BOOL:
    {
     Boolean_array initial_value;
     variant_field = initial_value;
     add_fi<MF_BOOL>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case MF_FLOAT:
    {
     Float_array initial_value;
     variant_field = initial_value;
     add_fi<MF_FLOAT>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case MF_TIME:
    {
     Scene_time_array initial_value;
     variant_field = initial_value;
     add_fi<MF_TIME>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case MF_INT32:
    {
     Int_array initial_value;
     variant_field = initial_value;
     add_fi<MF_INT32>(id, name, rule, initial_value, exec_func, prototype);
    }
     break;

   case MF_VEC2F:
    {
     Vector2f_array initial_value;
     variant_field = initial_value;
     add_fi<MF_VEC2F>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case MF_VEC3F:
   case MF_COLOR:
    {
     Vector3f_array initial_value;
     variant_field = initial_value;
     add_fi<MF_VEC3F>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case MF_ROTATION:
    {
     Rotation_array initial_value;
     variant_field = initial_value;
     add_fi<MF_ROTATION>(id, name, rule, initial_value, exec_func, prototype);
    }
    break;

   case MF_STR:
    {
     String_array initial_value;
     variant_field = initial_value;
     add_fi<MF_STR>(id, name, rule, initial_value, exec_func, prototype);
    }
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

void setter(v8::Local<v8::String> property,
            v8::Local<v8::Value> value,
            const v8::PropertyCallbackInfo<v8::Value>& info)
{
  v8::String::Utf8Value utf8_property(property);
  std::cout << "Intersepted name: " << *utf8_property << std::endl;
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
  // global_tmpl->SetNamedPropertyHandler(0, setter);

  Container_proto* proto = get_prototype();
  Container_proto::Id_const_iterator it = proto->ids_begin(proto);

  v8::Handle<v8::Context> context =
    v8::Context::New(isolate, NULL, global_tmpl);
  v8::Context::Scope context_scope(context);    // enter the contex
  v8::Handle<v8::Object> global = context->Global();

  // Add an object for every field and every output field.
  for (it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if ((field_info->get_rule() != RULE_FIELD) &&
        (field_info->get_rule() != RULE_OUT))
      continue;

    v8::Handle<v8::String> field_name =
      v8::String::NewFromUtf8(isolate, field_info->get_name().c_str(),
                              v8::String::kInternalizedString);

    // v8::Handle<v8::ObjectTemplate> field_tmpl =
    //   v8::ObjectTemplate::New(isolate);
    // field_tmpl->SetInternalFieldCount(1);
    // v8::Handle<v8::Object> field_obj = field_tmpl->NewInstance();
    // field_obj->SetInternalField(0, v8::Boolean::New(isolate, false));

    // std::cout << "In name: " << field_info->get_name() << std::endl;

    // In the following we extract the value through a temporary value holder.
    // Another option is to first extract values of static field and then
    // extract values of dynamic fields, where the latter can be done by
    // directly using field_handle(field_info)
    switch (field_info->get_type_id()) {
     case SF_BOOL:
      {
       Uint id = field_info->get_id();
       Boolean* tmp =
         (id == DIRECT_OUTPUT) ? direct_output_handle(field_info) :
         (id == MUST_EVALUATE) ? must_evaluate_handle(field_info) :
         field_handle<Boolean>(field_info);
       global->Set(field_name, v8::Boolean::New(isolate, *tmp));
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_FLOAT:
      {
       Float* tmp = field_handle<Float>(field_info);
       global->Set(field_name, v8::Number::New(isolate, *tmp));
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_TIME:
      {
       Scene_time* tmp = field_handle<Scene_time>(field_info);
       global->Set(field_name, v8::Number::New(isolate, *tmp));
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_INT32:
      {
       Int* tmp = field_handle<Int>(field_info);
       global->Set(field_name, v8::Int32::New(isolate, *tmp));
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_STR:
      {
       Uint id = field_info->get_id();
       std::string* tmp = (id == URL) ? url_handle(field_info) :
         field_handle<std::string>(field_info);
       global->Set(field_name, v8::String::NewFromUtf8(isolate, tmp->c_str()));
      }
      break;

     case SF_VEC2F:
      {
       Vector2f* tmp = field_handle<Vector2f>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, 2);
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       array->Set(0, v8::Number::New(isolate, (*tmp)[0]));
       array->Set(1, v8::Number::New(isolate, (*tmp)[1]));
       global->Set(field_name, array);
      }
      break;

     case SF_VEC3F:
     case SF_COLOR:
      {
       Vector3f* tmp = field_handle<Vector3f>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, 3);
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       array->Set(0, v8::Number::New(isolate, (*tmp)[0]));
       array->Set(1, v8::Number::New(isolate, (*tmp)[1]));
       array->Set(2, v8::Number::New(isolate, (*tmp)[2]));
       global->Set(field_name, array);
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_ROTATION:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case SF_IMAGE:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case SF_SHARED_CONTAINER:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_BOOL:
      {
       Boolean_array* tmp = field_handle<Boolean_array>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       for (size_t i = 0; i < tmp->size(); ++i)
         array->Set(i, v8::Boolean::New(isolate, (*tmp)[i]));
       global->Set(field_name, array);
      }
      break;

     case MF_FLOAT:
      {
       Float_array* tmp = field_handle<Float_array>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       for (size_t i = 0; i < tmp->size(); ++i)
         array->Set(i, v8::Number::New(isolate, (*tmp)[i]));
       global->Set(field_name, array);
      }
      break;

     case MF_TIME:
      {
       Scene_time_array* tmp = field_handle<Scene_time_array>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       for (size_t i = 0; i < tmp->size(); ++i)
         array->Set(i, v8::Number::New(isolate, (*tmp)[i]));
       global->Set(field_name, array);
      }
      break;

     case MF_INT32:
      {
       Int_array* tmp = field_handle<Int_array>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       for (size_t i = 0; i < tmp->size(); ++i)
         array->Set(i, v8::Int32::New(isolate, (*tmp)[i]));
       global->Set(field_name, array);
      }
      break;

     case MF_VEC2F:
      {
       Vector2f_array* tmp = field_handle<Vector2f_array>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       for (size_t i = 0; i < tmp->size(); ++i) {
         v8::Handle<v8::Array> vec2f = v8::Array::New(isolate, 2);
         vec2f->Set(0, v8::Number::New(isolate, ((*tmp)[i])[0]));
         vec2f->Set(1, v8::Number::New(isolate, ((*tmp)[i])[1]));
         array->Set(i, vec2f);
       }
       global->Set(field_name, array);
      }
      break;

     case MF_VEC3F:
     case MF_COLOR:
      {
       Vector3f_array* tmp = field_handle<Vector3f_array>(field_info);
       v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
       if (array.IsEmpty()) {
         std::cerr << "failed to allocate v8 Array!" << std::endl;
         break;
       }
       for (size_t i = 0; i < tmp->size(); ++i) {
         v8::Handle<v8::Array> vec3f = v8::Array::New(isolate, 3);
         vec3f->Set(0, v8::Number::New(isolate, ((*tmp)[i])[0]));
         vec3f->Set(1, v8::Number::New(isolate, ((*tmp)[i])[1]));
         vec3f->Set(2, v8::Number::New(isolate, ((*tmp)[i])[2]));
         array->Set(i, vec3f);
       }
       global->Set(field_name, array);
      }
      break;

     case MF_ROTATION:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_STR:
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
  // std::cout << field_info->get_name() << std::endl;
  switch (field_info->get_type_id()) {
   case SF_BOOL:
    // std::cout << "  value: " << *(field_handle<Boolean>(field_info))
    //           << std::endl;
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
     // std::cout << "  value: " << *tmp << std::endl;
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

   case SF_IMAGE:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_BOOL:
    {
     const Boolean_array* tmp = field_handle<Boolean_array>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     for (size_t i = 0; i < tmp->size(); ++i)
       array->Set(i, v8::Boolean::New(isolate, (*tmp)[i]));
     args[0] = array;
    }
    break;

   case MF_FLOAT:
    {
     const Float_array* tmp = field_handle<Float_array>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     for (size_t i = 0; i < tmp->size(); ++i)
       array->Set(i, v8::Number::New(isolate, (*tmp)[i]));
     args[0] = array;
    }
    break;

   case MF_TIME:
    {
     const Scene_time_array* tmp = field_handle<Scene_time_array>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     for (size_t i = 0; i < tmp->size(); ++i)
       array->Set(i, v8::Number::New(isolate, (*tmp)[i]));
     args[0] = array;
    }
    break;

   case MF_INT32:
    {
     const Int_array* tmp = field_handle<Int_array>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     for (size_t i = 0; i < tmp->size(); ++i)
       array->Set(i, v8::Int32::New(isolate, (*tmp)[i]));
     args[0] = array;
    }
    break;

   case MF_STR:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case MF_VEC2F:
    {
     const Vector2f_array* tmp = field_handle<Vector2f_array>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     for (size_t i = 0; i < tmp->size(); ++i) {
       v8::Handle<v8::Array> vec2f = v8::Array::New(isolate, 2);
       vec2f->Set(0, v8::Int32::New(isolate, ((*tmp)[i])[0]));
       vec2f->Set(1, v8::Int32::New(isolate, ((*tmp)[i])[1]));
       array->Set(i, vec2f);
     }
     args[0] = array;
    }
    break;

   case MF_VEC3F:
   case MF_COLOR:
    {
     const Vector3f_array* tmp = field_handle<Vector3f_array>(field_info);
     v8::Handle<v8::Array> array = v8::Array::New(isolate, tmp->size());
     if (array.IsEmpty()) {
       std::cerr << "failed to allocate v8 Array!" << std::endl;
       break;
     }
     for (size_t i = 0; i < tmp->size(); ++i) {
       v8::Handle<v8::Array> vec3f = v8::Array::New(isolate, 3);
       vec3f->Set(0, v8::Int32::New(isolate, ((*tmp)[i])[0]));
       vec3f->Set(1, v8::Int32::New(isolate, ((*tmp)[i])[1]));
       vec3f->Set(2, v8::Int32::New(isolate, ((*tmp)[i])[2]));
       array->Set(i, vec3f);
     }
     args[0] = array;
    }
    break;

   case MF_ROTATION:
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
    if ((field_info->get_rule() != RULE_OUT) &&
        (field_info->get_rule() != RULE_FIELD))
      continue;

    v8::Handle<v8::String> field_name =
      v8::String::NewFromUtf8(isolate, field_info->get_name().c_str(),
                              v8::String::kInternalizedString);
    v8::Handle<v8::Value> value = global->Get(field_name);

    // Ignore any scalar field that has not been assigned by the script.
    if (value->IsUndefined()) continue;

    // std::cout << "Out name: " << field_info->get_name() << std::endl;

    switch (field_info->get_type_id()) {
     case SF_BOOL:
      {
       Uint id = field_info->get_id();
       Boolean* tmp =
         (id == DIRECT_OUTPUT) ? direct_output_handle(field_info) :
         (id == MUST_EVALUATE) ? must_evaluate_handle(field_info) :
         field_handle<Boolean>(field_info);
       *tmp = value->BooleanValue();
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_FLOAT:
      {
       Float* tmp = field_handle<Float>(field_info);
       *tmp = static_cast<Float>(value->NumberValue());
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_TIME:
      {
       Scene_time* tmp = field_handle<Scene_time>(field_info);
       *tmp = static_cast<Scene_time>(value->NumberValue());
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_INT32:
      {
       Int* tmp = field_handle<Int>(field_info);
       *tmp = value->Int32Value();
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_STR:
      {
       Uint id = field_info->get_id();
       std::string* tmp = (id == URL) ? url_handle(field_info) :
         field_handle<std::string>(field_info);
       v8::Local<v8::String> str = v8::Handle<v8::String>::Cast(value);
       v8::String::Utf8Value utf8(str);
       tmp->assign(*utf8);
      }
      break;

     case SF_VEC2F:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Vector2f* tmp = field_handle<Vector2f>(field_info);
       tmp->set(static_cast<Float>(array->Get(0)->NumberValue()),
                static_cast<Float>(array->Get(1)->NumberValue()));
       SGAL_assertion(!std::isnan((*tmp)[0]));
       SGAL_assertion(!std::isnan((*tmp)[1]));
      }
      break;

     case SF_VEC3F:
     case SF_COLOR:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Vector3f* tmp = field_handle<Vector3f>(field_info);
       tmp->set(static_cast<Float>(array->Get(0)->NumberValue()),
                static_cast<Float>(array->Get(1)->NumberValue()),
                static_cast<Float>(array->Get(2)->NumberValue()));
       SGAL_assertion(!std::isnan((*tmp)[0]));
       SGAL_assertion(!std::isnan((*tmp)[1]));
       SGAL_assertion(!std::isnan((*tmp)[2]));
       // std::cout << "  value: " << *tmp << std::endl;
      }
      break;

     case SF_ROTATION:
     case SF_IMAGE:
     case SF_SHARED_CONTAINER:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     case MF_BOOL:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Boolean_array* tmp = field_handle<Boolean_array>(field_info);
       tmp->resize(array->Length());
       for (size_t i = 0; i < array->Length(); ++i)
         (*tmp)[i] = array->Get(i)->BooleanValue();
       // std::cout << "  ";
       // std::copy(tmp->begin(), tmp->end(),
       //           std::ostream_iterator<int>(std::cout, " "));
       // std::cout << std::endl;
      }
      break;

     case MF_FLOAT:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Float_array* tmp = field_handle<Float_array>(field_info);
       tmp->resize(array->Length());
       for (size_t i = 0; i < array->Length(); ++i)
         (*tmp)[i] = array->Get(i)->NumberValue();
      }
      break;

     case MF_TIME:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Scene_time_array* tmp = field_handle<Scene_time_array>(field_info);
       tmp->resize(array->Length());
       for (size_t i = 0; i < array->Length(); ++i)
         (*tmp)[i] = array->Get(i)->NumberValue();
      }
      break;

     case MF_INT32:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Int_array* tmp = field_handle<Int_array>(field_info);
       tmp->resize(array->Length());
       for (size_t i = 0; i < array->Length(); ++i)
         (*tmp)[i] = array->Get(i)->Int32Value();
      }
      break;

     case MF_VEC2F:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Vector2f_array* tmp = field_handle<Vector2f_array>(field_info);
       tmp->resize(array->Length());
       for (size_t i = 0; i < array->Length(); ++i) {
         v8::Local<v8::Array> vec2f = v8::Handle<v8::Array>::Cast(array->Get(i));
         ((*tmp)[i])[0] = vec2f->Get(0)->NumberValue();
         ((*tmp)[i])[1] = vec2f->Get(1)->NumberValue();
       }
      }
      break;

     case MF_VEC3F:
     case MF_COLOR:
      {
       v8::HandleScope scope(isolate);
       v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
       Vector3f_array* tmp = field_handle<Vector3f_array>(field_info);
       tmp->resize(array->Length());
       for (size_t i = 0; i < array->Length(); ++i) {
         v8::Local<v8::Array> vec3f = v8::Handle<v8::Array>::Cast(array->Get(i));
         ((*tmp)[i])[0] = vec3f->Get(0)->NumberValue();
         ((*tmp)[i])[1] = vec3f->Get(1)->NumberValue();
         ((*tmp)[i])[2] = vec3f->Get(2)->NumberValue();
       }
      }
      break;

     case MF_STR:
     case MF_ROTATION:
     case MF_SHARED_CONTAINER:
      std::cerr << "Not supported yet!" << std::endl;
      break;

     default:
      std::cerr << "Unsupported type!" << std::endl;
      return;
    }
    //! \todo must not cascade if the field hasn't been assigned.
    Field* field = get_field(field_info);
    if (field) field->cascade();
  }
}

SGAL_END_NAMESPACE
