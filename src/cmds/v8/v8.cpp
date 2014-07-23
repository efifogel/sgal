#include <iostream>
#include <vector>
#include <v8.h>

void indexed_enumerator(const v8::PropertyCallbackInfo<v8::Array>& info)
{
  std::cout << "indexed_enumerator()"
            << std::endl;
}

void indexed_getter(uint32_t index,
                    const v8::PropertyCallbackInfo<v8::Value>& info)
{
  std::cout << "indexed_getter()"
            << ", index: " << index
            << std::endl;
}

void indexed_setter(uint32_t index,
                    v8::Local<v8::Value> value,
                    const v8::PropertyCallbackInfo<v8::Value>& info)
{
  std::cout << "indexed_setter()"
            << ", index: " << index
            << std::endl;
}

void getter(v8::Local<v8::String> name,
            const v8::PropertyCallbackInfo<v8::Value>& info)
{
  // info.GetReturnValue().Set(info.Holder());

  v8::String::Utf8Value utf8_name(name);
  std::string str(*utf8_name);
  std::cout << "getter() " << str.c_str() << std::endl;
  return;

  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);

  if (str.compare("scalar") == 0) {
    int* scalar_ptr = (int*)(ext->Value());
    info.GetReturnValue().Set(*scalar_ptr);
    std::cout << ", value: " << *scalar_ptr << std::flush;
  }
  else if (str.compare("array") == 0) {
    std::vector<int>* array_ptr = (std::vector<int>*)(ext->Value());

    v8::Handle<v8::Array> array = v8::Array::New(info.GetIsolate(), 2);
    array->Set(0, v8::Int32::New(info.GetIsolate(), (*array_ptr)[0]));
    array->Set(1, v8::Int32::New(info.GetIsolate(), (*array_ptr)[1]));

    info.GetReturnValue().Set(array);
    std::cout << ", value: " << (*array_ptr)[0] << "," << (*array_ptr)[1]
              << std::flush;
  }
  std::cout << std::endl;
}

void setter(v8::Local<v8::String> name, v8::Local<v8::Value> value,
            const v8::PropertyCallbackInfo<void>& info)
{
  v8::String::Utf8Value utf8_name(name);
  std::string str(*utf8_name);
  std::cout << "setter() " << str.c_str() << std::endl;
  return;

  v8::Local<v8::Value> data = info.Data();
  v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(data);

  if (str.compare("scalar") == 0) {
    int* scalar_ptr = (int*)(ext->Value());
    *scalar_ptr = value->Int32Value();
    std::cout << ", value: " << *scalar_ptr << std::flush;
  }
  else if (str.compare("array") == 0) {
    std::vector<int>* array_ptr = (std::vector<int>*)(ext->Value());
    v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
    (*array_ptr)[0] = array->Get(0)->Int32Value();
    (*array_ptr)[1] = array->Get(1)->Int32Value();
    std::cout << ", value: " << (*array_ptr)[0] << "," << (*array_ptr)[1]
              << std::flush;
  }
  std::cout << std::endl;
}

void named_getter(v8::Local<v8::String> name,
                  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  v8::String::Utf8Value utf8_name(name);
  std::string str(*utf8_name);
  std::cout << "named_getter() " << str.c_str() << std::endl;
}

void named_setter(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                  const v8::PropertyCallbackInfo<v8::Value>& info)
{
  v8::String::Utf8Value utf8_name(name);
  std::string str(*utf8_name);
  std::cout << "named_setter() " << str.c_str() << std::endl;
}

// #define DYNAMIC

int main(int argc, char* argv[])
{
  if (argc <= 1) {
    std::cerr << "v8 <index>" << std::endl;
    return 0;
  }
  int index = atoi(argv[1]);

  int scalar;
  std::vector<int> array(2);

  v8::V8::InitializeICU();
  v8::Isolate* isolate = v8::Isolate::New();
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);

  v8::Handle<v8::String> scalar_name =
    v8::String::NewFromUtf8(isolate, "scalar");
  v8::Handle<v8::String> array_name =
    v8::String::NewFromUtf8(isolate, "array");

  v8::Handle<v8::ObjectTemplate> global_tmpl = v8::ObjectTemplate::New(isolate);

  v8::Handle<v8::External> scalar_data = v8::External::New(isolate, &scalar);
  v8::Handle<v8::External> array_data = v8::External::New(isolate, &array);

#if !defined(DYNAMIC)
  // Set getter & setter
  global_tmpl->SetAccessor(scalar_name, getter, setter, scalar_data);
  // global_tmpl->SetAccessor(array_name, getter, setter, array_data);

  global_tmpl->SetNamedPropertyHandler(named_getter, named_setter);
#endif

  v8::Handle<v8::Context> context =
    v8::Context::New(isolate, NULL, global_tmpl);
  v8::Context::Scope context_scope(context);    // enter the contex

#if defined(DYNAMIC)
  // Set getter & setter
  // context->Global()->SetAccessor(scalar_name, getter, setter, scalar_data);
  // context->Global()->SetAccessor(array_name, getter, setter, array_data);
#endif

  // Indexed setters
#if 1
  v8::Handle<v8::ObjectTemplate> tmpl = v8::ObjectTemplate::New(isolate);
  tmpl->SetIndexedPropertyHandler(indexed_getter, indexed_setter,
                                  0, 0, indexed_enumerator, array_data);
  context->Global()->Set(array_name, tmpl->NewInstance());
#else
  v8::Handle<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New(isolate);
  tmpl->SetClassName(array_name);
  v8::Handle<v8::ObjectTemplate> inst = tmpl->InstanceTemplate();
  // inst->SetInternalFieldCount(1);
  inst->SetIndexedPropertyHandler(indexed_getter, indexed_setter,
                                  0, 0, 0, array_data);
  v8::Handle<v8::Function> ctor = tmpl->GetFunction();
  v8::Handle<v8::Object> obj = ctor->NewInstance();
  // obj->SetInternalField(0, script_data);
  context->Global()->Set(array_name, obj);
#endif

  char str0[] =
    "function fnc(val) { scalar = 1; x = scalar + scalar; scalar = x + x; }";
  char str1[] =
    "function fnc(val) { var a = []; a[0] = 1; a[1] = 2; array = a; \n\
                         b = array; b[0] = 3; array = b; }";
  char str2[] =
    "function fnc(val) { array[0] = 1; array[1] = 2; }";
  char str3[] =
    "function fnc(val) { var a; }";
  char* strs[] = {str0, str1, str2, str3};
  v8::Handle<v8::String> source = v8::String::NewFromUtf8(isolate, strs[index]);

  v8::TryCatch try_catch;

  v8::Handle<v8::Script> script = v8::Script::Compile(source);
  if (script.IsEmpty()) {
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return 0;
  }

  v8::Handle<v8::Value> result = script->Run(); // run the script
  if (result.IsEmpty()) {
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return 0;
  }

  v8::Handle<v8::String> fnc_name = v8::String::NewFromUtf8(isolate, "fnc");
  v8::Handle<v8::Value> value = context->Global()->Get(fnc_name);
  if (!value->IsFunction()) {
    std::cerr << "A function named fnc does not exist!" << std::endl;
    return 0;
  }

  std::cout << "Start" << std::endl;
  v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  v8::Handle<v8::Value> args[1];
  args[0] = v8::Boolean::New(isolate, false);
  v8::Handle<v8::Value> func_result = func->Call(context->Global(), 1, args);
  if (func_result.IsEmpty()) {
    v8::String::Utf8Value error(try_catch.Exception());
    std::cerr << *error << std::endl;
    return 0;
  }

  return 0;
}
