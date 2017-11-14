#include "node_kind.hpp"

#include <svn_types.h>

#define InternalizedString(value) \
    v8::New<v8::String>(isolate, value, v8::NewStringType::kInternalized, sizeof(value) - 1)

#define SET_ENUM(target, prefix, name)                                                                                          \
    {                                                                                                                           \
        auto key = InternalizedString(#name);                                                                                   \
        target->DefineOwnProperty(context,                                                                                      \
                                  key,                                                                                          \
                                  v8::New<v8::Integer>(isolate, prefix##name),                                                  \
                                  v8::PropertyAttributeEx::ReadOnlyDontDelete);                                                 \
        target->DefineOwnProperty(context,                                                                                      \
                                  v8::New<v8::String>(isolate, std::to_string(prefix##name), v8::NewStringType::kInternalized), \
                                  key,                                                                                          \
                                  v8::PropertyAttributeEx::ReadOnlyDontDelete);                                                 \
    }

#define SetReadOnly(object, name, value)                  \
    (object)->DefineOwnProperty(context,                  \
                                InternalizedString(name), \
                                value,                    \
                                v8::PropertyAttributeEx::ReadOnlyDontDelete)

#define SET_NODE_KIND(name) SET_ENUM(object, svn_node_, name)

namespace node {
namespace node_kind {
void init(v8::Local<v8::Object>   exports,
          v8::Isolate*            isolate,
          v8::Local<v8::Context>& context) {
    auto object = v8::New<v8::Object>(isolate);

    SET_NODE_KIND(none);
    SET_NODE_KIND(file);
    SET_NODE_KIND(dir);
    SET_NODE_KIND(unknown);

    SetReadOnly(exports, "NodeKind", object);
}
} // namespace node_kind
} // namespace node
