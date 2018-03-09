#pragma once

#include <optional>
#include <string>

#include <uv.h>
#include <v8.h>

namespace {
template <class T>
struct Factory;

template <>
struct Factory<v8::Object> {
    static inline v8::Local<v8::Object> New(v8::Isolate* isolate) {
        return v8::Object::New(isolate);
    }
};

template <>
struct Factory<v8::External> {
    static inline v8::Local<v8::External> New(v8::Isolate* isolate,
                                              void*        value) {
        return v8::External::New(isolate, value);
    }
};

template <>
struct Factory<v8::Promise::Resolver> {
    static inline v8::Local<v8::Promise::Resolver> New(v8::Local<v8::Context>& context) {
        return v8::Promise::Resolver::New(context).ToLocalChecked();
    }
};

template <>
struct Factory<v8::Array> {
    static inline v8::Local<v8::Array> New(v8::Isolate* isolate, int length = 0) {
        return v8::Array::New(isolate, length);
    }
};

template <>
struct Factory<v8::Function> {
    static inline v8::Local<v8::Function> New(v8::Local<v8::Context>& context,
                                              v8::FunctionCallback    callback,
                                              v8::Local<v8::Value>    data   = v8::Local<v8::Value>(),
                                              int                     length = 0) {
        return v8::Function::New(context, callback, data, length).ToLocalChecked();
    }
};

template <>
struct Factory<v8::FunctionTemplate> {
    static inline v8::Local<v8::FunctionTemplate> New(v8::Isolate*             isolate,
                                                      v8::FunctionCallback     callback  = nullptr,
                                                      v8::Local<v8::Value>     data      = v8::Local<v8::Value>(),
                                                      v8::Local<v8::Signature> signature = v8::Local<v8::Signature>(),
                                                      int                      length    = 0) {
        return v8::FunctionTemplate::New(isolate, callback, data, signature, length);
    }
};
}; // namespace

namespace v8 {
template <class T, class A0>
inline Local<T> New(A0 a0) {
    return Factory<T>::New(a0);
}

template <class T, class A0, class A1>
inline Local<T> New(A0 a0, A1 a1) {
    return Factory<T>::New(a0, a1);
}

template <class T, class A0, class A1, class A2>
inline Local<T> New(A0 a0, A1 a1, A2 a2) {
    return Factory<T>::New(a0, a1, a2);
}

template <class T, class A0, class A1, class A2, class A3>
inline Local<T> New(A0 a0, A1 a1, A2 a2, A3 a3) {
    return Factory<T>::New(a0, a1, a2, a3);
}

struct PropertyAttributeEx {
    static const PropertyAttribute None               = PropertyAttribute::None;
    static const PropertyAttribute ReadOnly           = PropertyAttribute::ReadOnly;
    static const PropertyAttribute DontEnum           = PropertyAttribute::DontEnum;
    static const PropertyAttribute DontDelete         = PropertyAttribute::DontDelete;
    static const PropertyAttribute ReadOnlyDontEnum   = static_cast<PropertyAttribute>(ReadOnly | DontEnum);
    static const PropertyAttribute ReadOnlyDontDelete = static_cast<PropertyAttribute>(ReadOnly | DontDelete);
    static const PropertyAttribute DontEnumDontDelete = static_cast<PropertyAttribute>(DontEnum | DontDelete);
    static const PropertyAttribute All                = static_cast<PropertyAttribute>(ReadOnly | DontEnum | DontDelete);
};

static v8::Local<v8::Boolean> New(Isolate* isolate, bool value) {
    return v8::Boolean::New(isolate, value);
}

static v8::Local<v8::Integer> New(Isolate* isolate, int32_t value) {
    return v8::Integer::New(isolate, value);
}

static v8::Local<v8::Value> New(Isolate* isolate, std::optional<int32_t> value) {
    if (!value)
        return v8::Undefined(isolate);

    return v8::New(isolate, *value);
}

static v8::Local<v8::Integer> New(Isolate* isolate, uint32_t value) {
    return v8::Integer::NewFromUnsigned(isolate, value);
}

static v8::Local<v8::Number> New(Isolate* isolate, double value) {
    return v8::Number::New(isolate, value);
}

static v8::Local<v8::String> New(Isolate*           isolate,
                                 const std::string& value,
                                 v8::NewStringType  type = v8::NewStringType::kNormal) {
    return v8::String::NewFromUtf8(isolate, value.c_str(), type, static_cast<int>(value.size())).ToLocalChecked();
}

static v8::Local<v8::Value> New(Isolate*    isolate,
                                const char* value) {
    if (value == nullptr)
        return v8::Undefined(isolate);

    return v8::String::NewFromUtf8(isolate, value, v8::NewStringType::kNormal, -1).ToLocalChecked();
}

static v8::Local<v8::Value> New(Isolate*          isolate,
                                const char*       value,
                                v8::NewStringType type,
                                int               size = -1) {
    if (value == nullptr)
        return v8::Undefined(isolate);

    return v8::String::NewFromUtf8(isolate, value, type, size).ToLocalChecked();
}

static v8::Local<v8::String> New(Isolate*          isolate,
                                 const char*       value,
                                 int               size,
                                 v8::NewStringType type = v8::NewStringType::kNormal) {
    return v8::String::NewFromUtf8(isolate, value, type, size).ToLocalChecked();
}

static v8::Local<v8::Value> New(Isolate* isolate, int64_t value) {
    if (value > INT32_MAX)
        return v8::New(isolate, std::to_string(value));
    else
        return v8::New(isolate, static_cast<int32_t>(value));
}

static v8::Local<v8::External> New(Isolate* isolate, void* value) {
    return v8::External::New(isolate, value);
}
} // namespace v8
