#include "rmw/error_handling.h"
#include "rmw/serialized_message.h"
#include "rmw/rmw.h"

#include "./type_support_common.hpp"

extern "C"
{
rmw_ret_t
rmw_serialize(
  const void * ros_message,
  const rosidl_message_type_support_t * type_support,
  rmw_serialized_message_t * serialized_message)
{
  const rosidl_message_type_support_t * ts =
    get_message_typesupport_handle(type_support, RMW_COREDDS_CPP_TYPESUPPORT_C);
  if (ts == nullptr) {
    ts = get_message_typesupport_handle(type_support, RMW_COREDDS_CPP_TYPESUPPORT_CPP);
    if (ts == nullptr) {
      RMW_SET_ERROR_MSG("type support not from this implementation");
      return RMW_RET_ERROR;
    }
  }

  const message_type_support_callbacks_t * callbacks =
    static_cast<const message_type_support_callbacks_t *>(ts->data);
  if (callbacks == nullptr) {
    RMW_SET_ERROR_MSG("callbacks handle is null");
    return RMW_RET_ERROR;
  }

  void * dds_message = callbacks->alloc();
  if (dds_message == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate dds message");
    return RMW_RET_ERROR;
  }

  if (!callbacks->convert_ros_to_dds(ros_message, dds_message)) {
    RMW_SET_ERROR_MSG("failed to convert message");
    callbacks->free(dds_message);
    return RMW_RET_ERROR;
  }

  if (!callbacks->serialize(dds_message, serialized_message)) {
    RMW_SET_ERROR_MSG("failed to serialize message");
    callbacks->free(dds_message);
    return RMW_RET_ERROR;
  }

  callbacks->free(dds_message);

  return RMW_RET_OK;
}

rmw_ret_t
rmw_deserialize(
  const rmw_serialized_message_t * serialized_message,
  const rosidl_message_type_support_t * type_support,
  void * ros_message)
{
  const rosidl_message_type_support_t * ts =
    get_message_typesupport_handle(type_support, RMW_COREDDS_CPP_TYPESUPPORT_C);
  if (ts == nullptr) {
    ts = get_message_typesupport_handle(type_support, RMW_COREDDS_CPP_TYPESUPPORT_CPP);
    if (ts == nullptr) {
      RMW_SET_ERROR_MSG("type support not from this implementation");
      return RMW_RET_ERROR;
    }
  }

  const message_type_support_callbacks_t * callbacks =
    static_cast<const message_type_support_callbacks_t *>(ts->data);
  if (callbacks == nullptr) {
    RMW_SET_ERROR_MSG("callbacks handle is null");
    return RMW_RET_ERROR;
  }

  void * dds_message = callbacks->alloc();
  if (dds_message == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate dds message");
    return RMW_RET_ERROR;
  }

  if (!callbacks->deserialize(serialized_message, dds_message)) {
    RMW_SET_ERROR_MSG("failed to deserialize message");
    callbacks->free(dds_message);
    return RMW_RET_ERROR;
  }

  if (!callbacks->convert_dds_to_ros(dds_message, ros_message)) {
    RMW_SET_ERROR_MSG("failed to convert message");
    callbacks->free(dds_message);
    return RMW_RET_ERROR;
  }

  callbacks->free(dds_message);

  return RMW_RET_OK;
}
}  // extern "C"
