// Source: @tootallnate https://gist.github.com/TooTallNate/3987725
// Maintain an up-to-date copy of this file when necessary.

#pragma once

#include <node_jsvmapi.h>

// UNUSED: however, inline functions don't generate code.
inline static void wrap_pointer_cb(void *data) {
  // fprintf(stderr, "wrap_pointer_cb\n");
}


#define CHECK_STATUS                   \
  if (status != napi_ok) {             \
    exit(1);                           \
  }


/*
 * Wraps "ptr" into a new SlowBuffer instance with size "length".
 */

inline static napi_value WrapPointer(void *ptr, size_t length) {
  napi_status status;
  napi_env env;
  status = napi_get_current_env(&env);
  CHECK_STATUS;
  napi_value buf;
  status = napi_create_external_buffer(env, length, static_cast<char *>(ptr), wrap_pointer_cb, &buf);
  CHECK_STATUS;
  return buf;
}

/*
 * Wraps "ptr" into a new SlowBuffer instance with length 0.
 */

inline static napi_value WrapPointer(void *ptr) {
  return WrapPointer(ptr, 0);
}

/*
 * Unwraps Buffer instance "buffer" to a C `char *` with the offset specified.
 */

inline static char *UnwrapPointer(napi_value buffer, int64_t offset) {
  napi_status status;
  napi_env env;
  status = napi_get_current_env(&env);
  CHECK_STATUS;
  bool has_instance;
  status = napi_is_buffer(env, buffer, &has_instance);
  CHECK_STATUS;
  CHECK_STATUS;
  if (has_instance) {
    char* data;
    size_t size;
    status = napi_get_buffer_info(env, buffer, &data, &size);
    CHECK_STATUS;
    return data;
  } else {
    return 0;
  }
}

/*
 * Unwraps Buffer instance "buffer" to a C `char *` (no offset applied).
 */

inline static char *UnwrapPointer(napi_value buffer) {
  napi_status status;
  napi_env env;
  status = napi_get_current_env(&env);
  CHECK_STATUS;
  bool has_instance;
  status = napi_is_buffer(env, buffer, &has_instance);
  CHECK_STATUS;
  if (has_instance) {
    char* data;
    size_t size;
    status = napi_get_buffer_info(env, buffer, &data, &size);
    CHECK_STATUS;
    return data;
  } else {
    return 0;
  }
}

/**
 * Templated version of UnwrapPointer that does a reinterpret_cast() on the
 * pointer before returning it.
 */

template <typename Type>
inline static Type UnwrapPointer(napi_value buffer) {
  return reinterpret_cast<Type>(UnwrapPointer(buffer));
}
