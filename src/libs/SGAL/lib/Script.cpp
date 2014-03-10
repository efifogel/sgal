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

// #include "JSWObjectInt.h"
// #include "SG_JSObject.h"
// #include "SGVec_JSObject.h"

SGAL_BEGIN_NAMESPACE

const std::string Script::s_tag = "Script";
Container_proto* Script::s_prototype(nullptr);

REGISTER_TO_FACTORY(Script, "Script");

/*! Constructor */
Script::Script(Boolean proto) :
  Node(proto),
  m_protocol(PROTOCOL_INVALID)
  // m_JSWObject(nullptr),
  // m_engineInitialized(false),
  // m_SAI(nullptr),
  // m_SAINode(nullptr)
{}

/*! Destructor */
Script::~Script()
{
  // if (m_JSWObject) m_JSWObject->Destroy();
}

/*! \brief initializes the container prototype. */
void Script::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  String_handle_function url_func =
    static_cast<String_handle_function>(&Script::url_handle);
  s_prototype->add_field_info(new SF_string(URL, "url",
                                            RULE_EXPOSED_FIELD,
                                            url_func));

  Boolean_handle_function direct_output_func =
    static_cast<Boolean_handle_function>(&Script::direct_output_handle);
  s_prototype->add_field_info(new SF_bool(DIRECT_OUTPUT, "directOutput",
                                          RULE_EXPOSED_FIELD,
                                          direct_output_func));

  Boolean_handle_function must_evaluate_func =
    static_cast<Boolean_handle_function>(&Script::must_evaluate_handle);
  s_prototype->add_field_info(new SF_bool(MUST_EVALUATE, "mustEvaluate",
                                          RULE_EXPOSED_FIELD,
                                          must_evaluate_func));
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
  typedef Element::Str_attr_iter          Str_attr_iter;

  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "url") {
      std::string url = strip_double_quotes(value);
      set_url(url);
      url.clear();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "directOutput") {
      if (compare_to_true(value)) set_direct_output();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "mustEvaluate") {
      if (compare_to_true(value)) set_must_evaluate();
      elem->mark_delete(ai);
      continue;
    }
  }

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

  attribs = Node::get_attributes();

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

  Execution_function exec_func =
    static_cast<Execution_function>(&Script::execute);

  Variant_field variant_field;
  Uint id = LAST + m_fields.size();
  switch (type) {
   case SF_BOOL:
    {
     Boolean initial_value =
       value.empty() ? false : boost::lexical_cast<Boolean>(value);
     variant_field = initial_value;
     Boolean_handle_function field_func =
       static_cast<Boolean_handle_function>(&Script::field_handle<Boolean>);
     SF_bool* field_info =
       new SF_bool(id, name, rule, field_func, initial_value, exec_func);
     prototype->add_field_info(field_info);
    }
    break;

   case SF_FLOAT:
    {
     Float initial_value = value.empty() ? 0 : boost::lexical_cast<Float>(value);
     variant_field = initial_value;
     Float_handle_function field_func =
       static_cast<Float_handle_function>(&Script::field_handle<Float>);
     SF_float* field_info =
       new SF_float(id, name, rule, field_func, initial_value, exec_func);
     prototype->add_field_info(field_info);
    }
    break;

   case SF_INT32:
    {
     Int initial_value = value.empty() ? 0 : boost::lexical_cast<Int>(value);
     variant_field = initial_value;
     Int_handle_function field_func =
       static_cast<Int_handle_function>(&Script::field_handle<Int>);
     SF_int* field_info =
       new SF_int(id, name, rule, field_func, initial_value, exec_func);
     prototype->add_field_info(field_info);
    }
    break;

   case SF_VEC2F:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_VEC3F:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_COLOR:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_ROTATION:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_TIME:
    std::cerr << "Not supported yet!" << std::endl;
    break;

   case SF_STR:
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
    std::cerr << "Not supported yet!" << std::endl;
    break;

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

#if 0
/*! \brief adds a field definition to the script node.
 * \param name (in) the name of the field.
 * \param type (in) the type of the field.
 */
void Script::add_field_def(const String& name, const String& type,
                           Container* value, Node* field)
{
  m_child_list.push_back(field);

  Container_proto* prototype = get_prototype();

  // Add the object fields to the prototype
  s_prototype->add_field_info(m_field_infoIDCount, name, type, value, 0,
                              (Execution_func_type)&Script::execute);
}
#endif

// \brief executes the suitable script function according to the event.
void Script::execute(Field_info* field_info)
{
  const std::string& name = field_info->get_name();
  std::cout << "Script::execute() " << name << std::endl;

  // Get the Isolate instance of the V8 engine from the scene graph.
  v8::Isolate* isolate = m_scene_graph->get_isolate();
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);        // stack-allocated handle scope
  v8::Handle<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);    // enter the contex

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
  std::cout << "script: " << source_str << std::endl;

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

  v8::Handle<v8::Object> global = context->Global();
  v8::Handle<v8::Value> value =
    global->Get(v8::String::NewFromUtf8(isolate, name.c_str()));

  // If there is no such function, bail out.
  if (!value->IsFunction()) {
    std::cerr << "A function named " << name.c_str() << "does not exist!"
              << std::endl;
    return;
  }

  v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  v8::Handle<v8::Value> args[2];
  args[0] = v8::String::NewFromUtf8(isolate, "0");
  args[1] = v8::String::NewFromUtf8(isolate, "0");

  v8::Handle<v8::Value> func_result = func->Call(global, 2, args);
  if (func_result.IsEmpty()) {
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return;
  }

  // Convert the result to an UTF8 string and print it.
  // v8::String::Utf8Value utf8(result);
  // printf("%s\n", *utf8);

#if 0
  // if this is the first time the script is executed -
  // initialize the suitable script engine object
  if (!m_engineInitialized) {
    m_JSWObject->Init(get_name().c_str());
    bool compiled = m_JSWObject->CompileScript(m_url.c_str());
    if (!compiled)
      debug("JS Error - Object: %s, failed to compile script", m_name);
    m_engineInitialized = true;
  }

  SAI_fieldServices* field = m_SAINode->get_field(field_info->get_id());
  if (field == nullptr) {
    assert(false);
    return;
  }
  Int arg_type = field->get_type();

  m_SAI->BeginUpdate();

  if (arg_type == SFBOOL) {
    SAI_fieldSpecificServicesTemplate<Boolean, SFBOOL>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Boolean, SFBOOL>*>(field);
    if (spField == nullptr) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFFLOAT) {
    SAI_fieldSpecificServicesTemplate<Float,SFFLOAT>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Float, SFFLOAT>*>(field);
    if (spField == nullptr) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           (double)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFTIME) {
    SAI_fieldSpecificServicesTemplate<Scene_time, SFTIME>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Scene_time, SFTIME>*>(field);
    if (spField == nullptr) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           (double)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFINT32)
  {
    SAI_fieldSpecificServicesTemplate<Int, SFINT32>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<Int, SFINT32>*>(field);
    if (spField == nullptr) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           (int)spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFSTRING) {
    SAI_fieldSpecificServicesTemplate<String, SFSTRING>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<String, SFSTRING>*>(field);
    if (spField == nullptr) {
      assert(false);
      return;
    }
    m_JSWObject->CallFunction(field_info->get_name(),
           spField->get_value(),
           (double)m_executionCoordinator->get_sceneTime());
  }
  else if (arg_type == SFNODE)
  {
    SAI_fieldSpecificServicesTemplate<SAI_node_services*,SFNODE>* spField =
      dynamic_cast<SAI_fieldSpecificServicesTemplate<SAI_node_services*, SFNODE>*>(field);
    if (spField == nullptr) {
      assert(false);
      return;
    }
    SAI_node_services* nodeValue = spField->get_value();

    // If the SAI node has a client - it served a ESG_JSObject before -
    // use this client
    if (nodeValue->get_client()==nullptr)
      // if the SAI node has no client - allocate a new object
      m_JSWObject->CallFunction(field_info->get_name(),
                                new ESG_JSObject(m_SAI, nodeValue,
                                                 field_info->get_name()),
                                (double)m_executionCoordinator->get_sceneTime());
    else {
      // Cast the client to ESG_JSObject
      ESG_JSObject* obj = dynamic_cast<ESG_JSObject*>(nodeValue->get_client());
      if (obj == nullptr) {
        assert(false);
        return;
      }
      m_JSWObject->CallFunction(field_info->get_name(), obj,
                                (double)m_executionCoordinator->get_sceneTime());
    }
  }
  else if ((arg_type == SFVEC2F) || (arg_type == SFVEC3F) ||
           (arg_type == SFROTATION) || (arg_type == SFVEC4F))
  {
    // If the SAI field has a client - it served a ESGVec_JSObject before -
    // use this client
    if (field->get_client() == nullptr) {
      // if the SAI node has no client - allocate a new object and return it
      SAI_node_services* SAINode = m_SAI->get_node(this);
      m_JSWObject->CallFunction(field_info->get_name(),
                                new ESGVec_JSObject(m_SAI,SAINode,field),
                                (double)m_executionCoordinator->get_sceneTime());
    }
    else {
      // Cast the client to ESG_JSObject
      ESGVec_JSObject* obj =
        dynamic_cast<ESGVec_JSObject*>(field->get_client());
      if (obj == nullptr) {
        assert(false);
        return;
      }
      m_JSWObject->CallFunction(field_info->get_name(), obj,
                                (double)m_executionCoordinator->get_sceneTime());
    }
  }
  else assert(false);

  m_SAI->EndUpdate();
#endif
}

SGAL_END_NAMESPACE
