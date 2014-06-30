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
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Script::s_tag = "Script";
std::vector<Container_proto*> Script::s_prototypes;

REGISTER_TO_FACTORY(Script, "Script");

/*! Constructor */
Script::Script(Boolean proto) :
  Script_base(proto),
  m_isolate(nullptr)
{
  if (proto) return;
  size_t id = s_prototypes.size();
  m_id = id;
  s_prototypes.resize(id+1);
  s_prototypes[id] = nullptr;
}

/*! Destructor */
Script::~Script() { m_context.Reset(); }

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

//! \brief intercepts getting scalars.
void Script::getter(v8::Local<v8::String> property,
                    const v8::PropertyCallbackInfo<v8::Value>& info)
{
#if !defined(NDEBUG) || defined(SGAL_TRACE)
  if (SGAL::TRACE(Trace::SCRIPT)) {
    v8::String::Utf8Value utf8_property(property);
    std::cout << "getter() " << *utf8_property << std::endl;
  }
#endif

  // Obtain the Script node:
  v8::Handle<v8::Object> obj = info.Holder();
  SGAL_assertion(obj->InternalFieldCount() == 1);
  v8::Handle<v8::External> internal_field =
    v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
  SGAL_assertion(internal_field->Value() != nullptr);
  Script* script_node = static_cast<Script*>(internal_field->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  script: " + script_node->get_name() + "\n");

  // Obtain the field information record:
  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);
  SGAL_assertion(ext->Value() != nullptr);
  Field_info* field_info = static_cast<Field_info*>(ext->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  field: " + field_info->get_name() + "\n");

  Uint id = field_info->get_type_id();
  switch (id) {
   case SF_BOOL:
    {
     Boolean* tmp =
       (id == DIRECT_OUTPUT) ? script_node->direct_output_handle(field_info) :
       (id == MUST_EVALUATE) ? script_node->must_evaluate_handle(field_info) :
       script_node->field_handle<Boolean>(field_info);
     info.GetReturnValue().Set(*tmp);
    }
    break;

   case SF_FLOAT:
    {
     Float* tmp = script_node->field_handle<Float>(field_info);
     info.GetReturnValue().Set(*tmp);
    }
    break;

   case SF_TIME:
    {
     Scene_time* tmp = script_node->field_handle<Scene_time>(field_info);
     info.GetReturnValue().Set(*tmp);
    }
    break;

   case SF_INT32:
    {
     Int* tmp = script_node->field_handle<Int>(field_info);
     info.GetReturnValue().Set(*tmp);
    }
    break;

   case SF_STR:
    {
     std::string* tmp = (id == URL) ? script_node->url_handle(field_info) :
       script_node->field_handle<std::string>(field_info);
     info.GetReturnValue().Set(tmp->c_str());
    }
    break;

   case SF_VEC2F:
   case SF_VEC3F:
   case SF_COLOR:
   case SF_ROTATION:
   case SF_IMAGE:
   case SF_SHARED_CONTAINER:
   case MF_BOOL:
   case MF_FLOAT:
   case MF_TIME:
   case MF_INT32:
   case MF_VEC2F:
   case MF_VEC3F:
   case MF_COLOR:
   case MF_STR:
   case MF_ROTATION:
   case MF_SHARED_CONTAINER:
   default:
    SGAL_error();
    return;
  }
}

//! \brief intercepts setting scalars.
void Script::setter(v8::Local<v8::String> property,
                    v8::Local<v8::Value> value,
                    const v8::PropertyCallbackInfo<void>& info)
{
#if !defined(NDEBUG) || defined(SGAL_TRACE)
  if (SGAL::TRACE(Trace::SCRIPT)) {
    v8::String::Utf8Value utf8_property(property);
    std::cout << "setter() " << *utf8_property << std::endl;
  }
#endif

  // Obtain the Script node:
  v8::Handle<v8::Object> obj = info.Holder();
  SGAL_assertion(obj->InternalFieldCount() == 1);
  v8::Handle<v8::External> internal_field =
    v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
  SGAL_assertion(internal_field->Value() != nullptr);
  Script* script_node = static_cast<Script*>(internal_field->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  script: " + script_node->get_name() + "\n");

  // Obtain the field information record:
  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);
  SGAL_assertion(ext->Value() != nullptr);
  Field_info* field_info = static_cast<Field_info*>(ext->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  field: " + field_info->get_name() + "\n");

  // Apply the setting
  Uint id = field_info->get_type_id();
  switch (id) {
   case SF_BOOL:
    {
     Boolean* tmp =
       (id == DIRECT_OUTPUT) ? script_node->direct_output_handle(field_info) :
       (id == MUST_EVALUATE) ? script_node->must_evaluate_handle(field_info) :
       script_node->field_handle<Boolean>(field_info);
     *tmp = value->BooleanValue();
     // std::cout << "  value: " << *tmp << std::endl;
    }
    break;

   case SF_FLOAT:
    {
     Float* tmp = script_node->field_handle<Float>(field_info);
     *tmp = static_cast<Float>(value->NumberValue());
     // std::cout << "  value: " << *tmp << std::endl;
    }
    break;

   case SF_TIME:
    {
     Scene_time* tmp = script_node->field_handle<Scene_time>(field_info);
     *tmp = static_cast<Scene_time>(value->NumberValue());
     // std::cout << "  value: " << *tmp << std::endl;
    }
    break;

   case SF_INT32:
    {
     Int* tmp = script_node->field_handle<Int>(field_info);
     *tmp = value->Int32Value();
     // std::cout << "  value: " << *tmp << std::endl;
    }
    break;

   case SF_STR:
    {
     std::string* tmp = (id == URL) ? script_node->url_handle(field_info) :
       script_node->field_handle<std::string>(field_info);
     v8::Local<v8::String> str = v8::Handle<v8::String>::Cast(value);
     v8::String::Utf8Value utf8(str);
     tmp->assign(*utf8);
    }
    break;

   case SF_VEC2F:
   case SF_VEC3F:
   case SF_COLOR:
   case SF_ROTATION:
   case SF_IMAGE:
   case SF_SHARED_CONTAINER:
   case MF_BOOL:
   case MF_FLOAT:
   case MF_TIME:
   case MF_INT32:
   case MF_VEC2F:
   case MF_VEC3F:
   case MF_COLOR:
   case MF_STR:
   case MF_ROTATION:
   case MF_SHARED_CONTAINER:
   default:
    SGAL_error();
    return;
  }

  if (script_node->is_direct_output()) {
    Field* field = script_node->get_field(field_info);
    if (field) field->cascade();
  }
  else script_node->insert_assigned(field_info);
}

//! \brief intercepts getting arrays.
void Script::array_getter(v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value>& info)
{
#if !defined(NDEBUG) || defined(SGAL_TRACE)
  if (SGAL::TRACE(Trace::SCRIPT)) {
    v8::String::Utf8Value utf8_property(property);
    std::cout << "array_getter() " << *utf8_property << std::endl;
  }
#endif

  auto isolate = info.GetIsolate();
  v8::HandleScope handle_scope(isolate);

  // Obtain the Script node:
  v8::Handle<v8::Object> obj = info.Holder();
  SGAL_assertion(obj->InternalFieldCount() == 1);
  v8::Handle<v8::External> internal_field =
    v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
  SGAL_assertion(internal_field->Value() != nullptr);
  Script* script_node = static_cast<Script*>(internal_field->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  script: " + script_node->get_name() + "\n");

  // Prepare for indexing
  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);
  v8::Handle<v8::ObjectTemplate> tmpl = v8::ObjectTemplate::New(isolate);
  tmpl->SetInternalFieldCount(1);
  tmpl->SetIndexedPropertyHandler(indexed_getter, indexed_setter, 0, 0, 0, ext);
  auto inst = tmpl->NewInstance();
  inst->SetInternalField(0, v8::External::New(isolate, script_node));
  info.GetReturnValue().Set(inst);
}

//! \brief intercepts setting arrays.
void Script::array_setter(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void>& info)
{
#if !defined(NDEBUG) || defined(SGAL_TRACE)
  if (SGAL::TRACE(Trace::SCRIPT)) {
    v8::String::Utf8Value utf8_property(property);
    std::cout << "array_setter() " << *utf8_property << std::endl;
  }
#endif

  auto isolate = info.GetIsolate();

  // Obtain the Script node:
  v8::Handle<v8::Object> obj = info.Holder();
  SGAL_assertion(obj->InternalFieldCount() == 1);
  v8::Handle<v8::External> internal_field =
    v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
  SGAL_assertion(internal_field->Value() != nullptr);
  Script* script_node = static_cast<Script*>(internal_field->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  script: " + script_node->get_name() + "\n");

  // Obtain the field handle and apply the setting:
  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);
  SGAL_assertion(ext->Value() != nullptr);
  Field_info* field_info = static_cast<Field_info*>(ext->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  field: " + field_info->get_name() + "\n");

  switch (field_info->get_type_id()) {
   case SF_BOOL:
   case SF_FLOAT:
   case SF_TIME:
   case SF_INT32:
   case SF_STR:
    SGAL_error();
    break;

   case SF_VEC2F:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     Vector2f* tmp = script_node->field_handle<Vector2f>(field_info);
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
     Vector3f* tmp = script_node->field_handle<Vector3f>(field_info);
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
     Boolean_array* tmp = script_node->field_handle<Boolean_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i)
       (*tmp)[i] = array->Get(i)->BooleanValue();
     // std::cout << "  ";
     // std::copy(tmp->begin(), tmp->end(),
     //           std::ostream_iterator<bool>(std::cout, " "));
     // std::cout << std::endl;
    }
    break;

   case MF_FLOAT:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     Float_array* tmp = script_node->field_handle<Float_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i)
       (*tmp)[i] = static_cast<Float>(array->Get(i)->NumberValue());
    }
    break;

   case MF_TIME:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     Scene_time_array* tmp =
       script_node->field_handle<Scene_time_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i)
       (*tmp)[i] = static_cast<Float>(array->Get(i)->NumberValue());
    }
    break;

   case MF_INT32:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     Int_array* tmp = script_node->field_handle<Int_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i)
       (*tmp)[i] = array->Get(i)->Int32Value();
    }
    break;

   case MF_VEC2F:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     Vector2f_array* tmp = script_node->field_handle<Vector2f_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i) {
       v8::Local<v8::Array> vec2f =
         v8::Handle<v8::Array>::Cast(array->Get(i));
       ((*tmp)[i])[0] = static_cast<Float>(vec2f->Get(0)->NumberValue());
       ((*tmp)[i])[1] = static_cast<Float>(vec2f->Get(1)->NumberValue());
     }
     // std::cout << "  ";
     // std::copy(tmp->begin(), tmp->end(),
     //           std::ostream_iterator<Vector2f>(std::cout, " "));
     // std::cout << std::endl;
    }
    break;

   case MF_VEC3F:
   case MF_COLOR:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     Vector3f_array* tmp = script_node->field_handle<Vector3f_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i) {
       v8::Local<v8::Array> vec3f =
         v8::Handle<v8::Array>::Cast(array->Get(i));
       ((*tmp)[i])[0] = static_cast<Float>(vec3f->Get(0)->NumberValue());
       ((*tmp)[i])[1] = static_cast<Float>(vec3f->Get(1)->NumberValue());
       ((*tmp)[i])[2] = static_cast<Float>(vec3f->Get(2)->NumberValue());
     }
     // std::cout << "  ";
     // std::copy(tmp->begin(), tmp->end(),
     //           std::ostream_iterator<Vector3f>(std::cout, " "));
     // std::cout << std::endl;
    }
    break;

   case MF_STR:
    {
     v8::HandleScope scope(isolate);
     v8::Local<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
     String_array* tmp = script_node->field_handle<String_array>(field_info);
     tmp->resize(array->Length());
     for (size_t i = 0; i < array->Length(); ++i) {
       v8::Local<v8::String> str = v8::Handle<v8::String>::Cast(array->Get(i));
       v8::String::Utf8Value utf8(str);
       (*tmp)[i].assign(*utf8);
     }
    }
    break;

   case MF_ROTATION:
   case MF_SHARED_CONTAINER:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   default:
    std::cerr << "Unsupported type!" << std::endl;
    return;
  }

  if (script_node->is_direct_output()) {
    Field* field = script_node->get_field(field_info);
    if (field) field->cascade();
  }
  else script_node->insert_assigned(field_info);
}

//! \brief intercepts getting an array element by index.
void Script::indexed_getter(uint32_t index,
                            const v8::PropertyCallbackInfo<v8::Value>& info)
{
  SGAL_TRACE_CODE(Trace::SCRIPT,
                  std::cout << "indexed_getter() " << index << std::endl;);
}

//! \brief intercepts setting an array element by index.
void Script::indexed_setter(uint32_t index,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<v8::Value>& info)
{
  SGAL_TRACE_CODE(Trace::SCRIPT,
                  std::cout << "indexed_setter() " << index << std::endl;);

  auto isolate = info.GetIsolate();

  // Obtain the Script node:
  v8::Handle<v8::Object> obj = info.Holder();
  SGAL_assertion(obj->InternalFieldCount() == 1);
  v8::Handle<v8::External> internal_field =
    v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
  SGAL_assertion(internal_field->Value() != nullptr);
  Script* script_node = static_cast<Script*>(internal_field->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  script: " + script_node->get_name() + "\n");

  // Obtain the field information record:
  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);
  SGAL_assertion(ext->Value() != nullptr);
  Field_info* field_info = static_cast<Field_info*>(ext->Value());
  SGAL_TRACE_MSG(Trace::SCRIPT, "  field: " + field_info->get_name() + "\n");

  // Assign
  switch (field_info->get_type_id()) {
   case SF_BOOL:
   case SF_FLOAT:
   case SF_TIME:
   case SF_INT32:
   case SF_STR:
    SGAL_error();
    break;

   case SF_VEC2F:
    {
     v8::HandleScope scope(isolate);
     SGAL_assertion(index < 2);
     Vector2f* tmp = script_node->field_handle<Vector2f>(field_info);
     (*tmp)[index] = value->NumberValue();
    }
    break;

   case SF_VEC3F:
   case SF_COLOR:
    {
     v8::HandleScope scope(isolate);
     SGAL_assertion(index < 3);
     Vector3f* tmp = script_node->field_handle<Vector3f>(field_info);
     (*tmp)[index] = value->NumberValue();
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
     Boolean_array* tmp = script_node->field_handle<Boolean_array>(field_info);
     tmp->resize(index+1);
     (*tmp)[index] = value->BooleanValue();
    }
    break;

   case MF_FLOAT:
    {
     v8::HandleScope scope(isolate);
     Float_array* tmp = script_node->field_handle<Float_array>(field_info);
     tmp->resize(index+1);
     (*tmp)[index] = value->NumberValue();
    }
    break;

   case MF_TIME:
    {
     v8::HandleScope scope(isolate);
     Scene_time_array* tmp =
       script_node->field_handle<Scene_time_array>(field_info);
     tmp->resize(index+1);
     (*tmp)[index] = value->NumberValue();
    }
    break;

   case MF_INT32:
    {
     v8::HandleScope scope(isolate);
     Int_array* tmp = script_node->field_handle<Int_array>(field_info);
     tmp->resize(index+1);
     (*tmp)[index] = value->Int32Value();;
    }
    break;

   case MF_VEC2F:
    {
     v8::HandleScope scope(isolate);
     Vector2f_array* tmp = script_node->field_handle<Vector2f_array>(field_info);
     tmp->resize(index+1);
     v8::Local<v8::Array> vec2f = v8::Handle<v8::Array>::Cast(value);
     ((*tmp)[index])[0] = static_cast<Float>(vec2f->Get(0)->NumberValue());
     ((*tmp)[index])[1] = static_cast<Float>(vec2f->Get(1)->NumberValue());
    }
    break;

   case MF_VEC3F:
   case MF_COLOR:
    {
     v8::HandleScope scope(isolate);
     Vector3f_array* tmp = script_node->field_handle<Vector3f_array>(field_info);
     tmp->resize(index+1);
     v8::Local<v8::Array> vec3f = v8::Handle<v8::Array>::Cast(value);
     ((*tmp)[index])[0] = static_cast<Float>(vec3f->Get(0)->NumberValue());
     ((*tmp)[index])[1] = static_cast<Float>(vec3f->Get(1)->NumberValue());
     ((*tmp)[index])[2] = static_cast<Float>(vec3f->Get(2)->NumberValue());
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

  if (script_node->is_direct_output()) {
    Field* field = script_node->get_field(field_info);
    if (field) field->cascade();
  }
  else script_node->insert_assigned(field_info);
}

//! \brief records the scene graph.
void Script::add_to_scene(Scene_graph* scene_graph)
{
  // Get the Isolate instance of the V8 engine from the scene graph.
  m_isolate = scene_graph->get_isolate();
  v8::Isolate::Scope isolate_scope(m_isolate);
  v8::HandleScope handle_scope(m_isolate);      // stack-allocated handle scope

  // Create a new ObjectTemplate
  v8::Handle<v8::ObjectTemplate> global_tmpl =
    v8::ObjectTemplate::New(m_isolate);
  global_tmpl->SetInternalFieldCount(1);

  v8::Local<v8::Context> context =
    v8::Context::New(m_isolate, NULL, global_tmpl);
  m_context.Reset(m_isolate, context);

  v8::Context::Scope context_scope(context);    // enter the contex
  v8::Handle<v8::Object> global = context->Global();
  v8::Local<v8::Object> prototype =
    v8::Local<v8::Object>::Cast(global->GetPrototype());
  prototype->SetInternalField(0, v8::External::New(m_isolate, this));
  add_callbacks(global);    // add callbacks for every field and output field.
  bound_script();           // bound the script to the context
}

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
       value.empty() ? false : compare_to_true(value);
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

//! \brief converts a single Boolean field value to a v8 engine Boolean.
v8::Handle<v8::Value> Script::get_single_boolean(const Field_info* field_info)
{
  Uint id = field_info->get_id();
  Boolean* tmp =
    (id == DIRECT_OUTPUT) ? direct_output_handle(field_info) :
    (id == MUST_EVALUATE) ? must_evaluate_handle(field_info) :
    field_handle<Boolean>(field_info);
  return v8::Boolean::New(m_isolate, *tmp);
}

//! \brief converts a single string field value to a v8 engine string.
v8::Handle<v8::Value> Script::get_single_string(const Field_info* field_info)
{
  Uint id = field_info->get_id();
  std::string* tmp = (id == URL) ? url_handle(field_info) :
    field_handle<std::string>(field_info);
  return v8::String::NewFromUtf8(m_isolate, tmp->c_str());
}

/*! \brief converts a single Vector2f field value to a v8 engine array of 2
 * floats.
 */
v8::Handle<v8::Value> Script::get_single_vector2f(const Field_info* field_info)
{
  Vector2f* tmp = field_handle<Vector2f>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, 2);
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  array->Set(0, v8::Number::New(m_isolate, (*tmp)[0]));
  array->Set(1, v8::Number::New(m_isolate, (*tmp)[1]));
  return array;
}

/*! \brief converts a single Vector3f field value to a v8 engine array of 3
 * floats.
 */
v8::Handle<v8::Value> Script::get_single_vector3f(const Field_info* field_info)
{
  Vector3f* tmp = field_handle<Vector3f>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, 3);
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  array->Set(0, v8::Number::New(m_isolate, (*tmp)[0]));
  array->Set(1, v8::Number::New(m_isolate, (*tmp)[1]));
  array->Set(2, v8::Number::New(m_isolate, (*tmp)[2]));
  return array;
}

/*! \brief converts a multi Boolean field value to a v8 engine array of
 * Booleans.
 */
v8::Handle<v8::Value> Script::get_multi_boolean(const Field_info* field_info)
{
  Boolean_array* tmp = field_handle<Boolean_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i)
    array->Set(i, v8::Boolean::New(m_isolate, (*tmp)[i]));
  return array;
}

//! \brief converts a multi float field value to a v8 engine array of floats.
v8::Handle<v8::Value> Script::get_multi_float(const Field_info* field_info)
{
  Float_array* tmp = field_handle<Float_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i)
    array->Set(i, v8::Number::New(m_isolate, (*tmp)[i]));
  return array;
}

//! \brief converts a multi time field value to a v8 engine array of floats.
v8::Handle<v8::Value> Script::get_multi_time(const Field_info* field_info)
{
  Scene_time_array* tmp = field_handle<Scene_time_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i)
    array->Set(i, v8::Number::New(m_isolate, (*tmp)[i]));
  return array;
}

//! \brief converts a multi int32 field value to a v8 engine array of int32.
v8::Handle<v8::Value> Script::get_multi_int32(const Field_info* field_info)
{
  Int_array* tmp = field_handle<Int_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i)
    array->Set(i, v8::Int32::New(m_isolate, (*tmp)[i]));
  return array;
}

//! \brief converts a multi string field value to a v8 engine array of strings.
v8::Handle<v8::Value> Script::get_multi_string(const Field_info* field_info)
{
  String_array* tmp = field_handle<String_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i)
    array->Set(i, v8::String::NewFromUtf8(m_isolate, (*tmp)[i].c_str()));
  return array;
}

/*! \brief converts a multi Vector2f field value to a v8 engine array of
 * arrays of 2 floats.
 */
v8::Handle<v8::Value> Script::get_multi_vector2f(const Field_info* field_info)
{
  Vector2f_array* tmp = field_handle<Vector2f_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i) {
    v8::Handle<v8::Array> vec2f = v8::Array::New(m_isolate, 2);
    vec2f->Set(0, v8::Number::New(m_isolate, ((*tmp)[i])[0]));
    vec2f->Set(1, v8::Number::New(m_isolate, ((*tmp)[i])[1]));
    array->Set(i, vec2f);
  }
  return array;
}

/*! \brief converts a multi Vector3f field value to a v8 engine array of
 * arrays of 3 floats.
 */
v8::Handle<v8::Value> Script::get_multi_vector3f(const Field_info* field_info)
{
  Vector3f_array* tmp = field_handle<Vector3f_array>(field_info);
  v8::Handle<v8::Array> array = v8::Array::New(m_isolate, tmp->size());
  if (array.IsEmpty()) {
    std::cerr << "failed to allocate v8 Array!" << std::endl;
    return v8::Null(m_isolate);
  }
  for (size_t i = 0; i < tmp->size(); ++i) {
    v8::Handle<v8::Array> vec3f = v8::Array::New(m_isolate, 3);
    vec3f->Set(0, v8::Number::New(m_isolate, ((*tmp)[i])[0]));
    vec3f->Set(1, v8::Number::New(m_isolate, ((*tmp)[i])[1]));
    vec3f->Set(2, v8::Number::New(m_isolate, ((*tmp)[i])[2]));
    array->Set(i, vec3f);
  }
  return array;
}

/*! \brief adds setter and getter callbacks to the engine for every field and
 * every output field.
 */
void Script::add_callbacks(v8::Local<v8::Object> global)
{
  Container_proto* proto = get_prototype();
  Container_proto::Id_const_iterator it = proto->ids_begin(proto);
  for (it = proto->ids_begin(proto); it != proto->ids_end(proto); ++it) {
    const Field_info* field_info = (*it).second;
    if ((field_info->get_rule() != RULE_FIELD) &&
        (field_info->get_rule() != RULE_OUT))
      continue;

    v8::Handle<v8::String> field_name =
      v8::String::NewFromUtf8(m_isolate, field_info->get_name().c_str(),
                              v8::String::kInternalizedString);
    // std::cout << "In name: " << field_info->get_name() << std::endl;
    v8::Handle<v8::External> ext =
      v8::External::New(m_isolate, const_cast<Field_info*>(field_info));
    if (field_info->is_scalar())
      global->SetAccessor(field_name, getter, setter, ext);
    else
      global->SetAccessor(field_name, array_getter, array_setter, ext);
  }
}

/*! \brief cascades all fields that has been altered during the execution of the
 * script.
 */
void Script::cascade_assigned()
{
  if (m_direct_output) return;
  for (auto it = m_assigned.begin(); it != m_assigned.end(); ++it) {
    Field* field = get_field(*it);
    if (field) field->cascade();
  }
}

//! \brief bounds the script.
void Script::bound_script()
{
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
    v8::String::NewFromUtf8(m_isolate, source_str.c_str());

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
}

//! \brief obtains the first argument for the call.
v8::Handle<v8::Value> Script::get_argument(const Field_info* field_info)
{
  // std::cout << field_info->get_name() << std::endl;
  switch (field_info->get_type_id()) {
   case SF_BOOL: return get_single_boolean(field_info);
   case SF_FLOAT: return get_single_float(field_info);
   case SF_TIME: return get_single_time(field_info);
   case SF_INT32: return get_single_int32(field_info);
   case SF_STR: return get_single_string(field_info);
   case SF_VEC2F: return get_single_vector2f(field_info);

   case SF_VEC3F:
   case SF_COLOR: return get_single_vector3f(field_info);

   case SF_ROTATION:
   case SF_SHARED_CONTAINER:
   case SF_IMAGE:
    std::cerr << "Not supported yet!" << std::endl;
    return v8::Null(m_isolate);

   case MF_BOOL: return get_multi_boolean(field_info); break;
   case MF_FLOAT: return get_multi_float(field_info); break;
   case MF_TIME: return get_multi_time(field_info); break;
   case MF_INT32: return get_multi_int32(field_info); break;
   case MF_STR: return get_multi_string(field_info); break;
   case MF_VEC2F: return get_multi_vector2f(field_info); break;

   case MF_VEC3F:
   case MF_COLOR: return get_multi_vector3f(field_info); break;

   case MF_ROTATION:
   case MF_SHARED_CONTAINER:
    SGAL_error_msg("Not supported yet!");
    return v8::Null(m_isolate);

   default:
    SGAL_error_msg("Unsupported type!");
    return v8::Null(m_isolate);
  }
  SGAL_error_msg("Should never get here!");
  return v8::Null(m_isolate);
}

//! \brief executes the suitable script function according to the event.
void Script::execute(const Field_info* field_info)
{
  v8::Isolate::Scope isolate_scope(m_isolate);
  v8::HandleScope handle_scope(m_isolate);      // stack-allocated handle scope
  v8::Local<v8::Context> context =
      v8::Local<v8::Context>::New(m_isolate, m_context);
  v8::Context::Scope context_scope(context);    // enter the contex
  v8::Handle<v8::Object> global = context->Global();
  v8::Local<v8::Object> prototype =
    v8::Local<v8::Object>::Cast(global->GetPrototype());
  prototype->SetInternalField(0, v8::External::New(m_isolate, this));

  const std::string& str = field_info->get_name();
  v8::Handle<v8::String> name = v8::String::NewFromUtf8(m_isolate, str.c_str());
  v8::Handle<v8::Value> value = global->Get(name);

  // If there is no such function, bail out.
  if (!value->IsFunction()) {
    std::cerr << "A function named " << str.c_str() << " does not exist!"
              << std::endl;
    return;
  }

  v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  v8::Handle<v8::Value> args[2];
  args[0] = get_argument(field_info);
  args[1] = v8::Number::New(m_isolate, m_time);

  // Clear the set of assigned fields. When an object is assigned,
  // the id of the associate field is inserted into the set. A filed, the
  // id of which is not found in the set is not cascaded.
  clear_assigned();
  v8::TryCatch try_catch;
  v8::Handle<v8::Value> func_result = func->Call(global, 2, args);
  if (func_result.IsEmpty()) {
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return;
  }

  cascade_assigned();
}

SGAL_END_NAMESPACE
