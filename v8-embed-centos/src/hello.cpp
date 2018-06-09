// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>

#include "libplatform/libplatform.h"
#include "v8.h"

using std::string;
using std::map;

using v8::Context;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::NewStringType;
using v8::Script;
using v8::String;

void ParseOptions(int argc, char* argv[], map<string, string>* options, string* file) {
  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    size_t index = arg.find('=', 0);
    if (index == string::npos) {
      *file = arg;
    } else {
      string key = arg.substr(0, index);
      string value = arg.substr(index+1);
      (*options)[key] = value;
    }
  }
}

MaybeLocal<String> ReadFile(Isolate* isolate, const string& name) {
  FILE* file = fopen(name.c_str(), "rb");

  if (file == NULL) {
    return MaybeLocal<String>();
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  std::unique_ptr<char> chars(new char[size + 1]);
  chars.get()[size] = '\0';

  for (size_t i = 0; i < size;) {
    i += fread(&chars.get()[i], 1, size - i, file);

    if (ferror(file)) {
      fclose(file);
      return MaybeLocal<String>();
    }
  }

  fclose(file);

  MaybeLocal<String> result = String::NewFromUtf8(
    isolate,
    chars.get(),
    NewStringType::kNormal,
    static_cast<int>(size)
  );

  return result;
}


int main(int argc, char* argv[]) {
  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  Isolate* isolate = Isolate::New(create_params);

  {
    Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    // Create a new context.
    Local<Context> context = Context::New(isolate);

    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);

    {
      // std::string js_source = read_js("hello_world.js");
      // Create a string containing the JavaScript source code.
      string file;
      map<string, string> options;
      ParseOptions(argc, argv, &options, &file);

      if (file.empty()) {
        fprintf(stderr, "No script was specified.\n");
        return 1;
      }

      Local<String> source;
      if (!ReadFile(isolate, file).ToLocal(&source)) {
        fprintf(stderr, "Error reading file\n");
        return 1;
      }

      // Compile the source code.
      Local<Script> script = Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }
  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::ShutdownPlatform();
  delete create_params.array_buffer_allocator;
  return 0;
}
