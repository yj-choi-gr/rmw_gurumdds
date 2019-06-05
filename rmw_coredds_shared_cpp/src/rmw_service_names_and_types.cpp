#include <map>
#include <set>
#include <string>

#include "rcutils/allocator.h"
#include "rcutils/logging_macros.h"
#include "rcutils/strdup.h"
#include "rcutils/types/string_array.h"

#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
#include "rmw/error_handling.h"

#include "rmw_coredds_shared_cpp/demangle.hpp"
#include "rmw_coredds_shared_cpp/names_and_types_helpers.hpp"
#include "rmw_coredds_shared_cpp/dds_include.hpp"
#include "rmw_coredds_shared_cpp/rmw_common.hpp"
#include "rmw_coredds_shared_cpp/types.hpp"

rmw_ret_t
shared__rmw_get_service_names_and_types(
  const char * implementation_identifier,
  const rmw_node_t * node,
  rcutils_allocator_t * allocator,
  rmw_names_and_types_t * service_names_and_types)
{
  if (allocator == nullptr) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (node == nullptr) {
    RMW_SET_ERROR_MSG("node handle is nulll");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (node->implementation_identifier != implementation_identifier) {
    RMW_SET_ERROR_MSG("node handle is not from this rmw implementation");
    return RMW_RET_ERROR;
  }

  rmw_ret_t ret = rmw_names_and_types_check_zero(service_names_and_types);
  if (ret != RMW_RET_OK) {
    return ret;
  }

  CoreddsNodeInfo * node_info = static_cast<CoreddsNodeInfo *>(node->data);
  if (node_info == nullptr) {
    RMW_SET_ERROR_MSG("node info handle is null");
    return RMW_RET_ERROR;
  }
  if (node_info->pub_listener == nullptr) {
    RMW_SET_ERROR_MSG("publisher listener handle is null");
    return RMW_RET_ERROR;
  }
  if (node_info->sub_listener == nullptr) {
    RMW_SET_ERROR_MSG("subscriber listener handle is null");
    return RMW_RET_ERROR;
  }

  std::map<std::string, std::set<std::string>> services;
  node_info->pub_listener->fill_service_names_and_types(services);
  node_info->sub_listener->fill_service_names_and_types(services);

  if (services.size() > 0) {
    ret = copy_services_to_names_and_types(services, allocator, service_names_and_types);
    if (ret != RMW_RET_OK) {
      return ret;
    }
  }

  return RMW_RET_OK;
}
