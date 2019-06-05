#include "rmw/allocators.h"
#include "rmw/error_handling.h"
#include "rmw/rmw.h"
#include "rmw/impl/cpp/macros.hpp"

#include "rmw_coredds_shared_cpp/rmw_common.hpp"
#include "rmw_coredds_shared_cpp/types.hpp"
#include "rmw_coredds_shared_cpp/dds_include.hpp"

rmw_wait_set_t *
shared__rmw_create_wait_set(
  const char * implementation_identifier,
  size_t max_conditions)
{
  (void)max_conditions;
  rmw_wait_set_t * wait_set = rmw_wait_set_allocate();

  CoreddsWaitSetInfo * wait_set_info = nullptr;

  if (!wait_set) {
    RMW_SET_ERROR_MSG("failed to allocate wait set");
    goto fail;
  }

  wait_set->implementation_identifier = implementation_identifier;
  wait_set->data = rmw_allocate(sizeof(CoreddsWaitSetInfo));
  wait_set_info = static_cast<CoreddsWaitSetInfo *>(wait_set->data);

  if (!wait_set_info) {
    RMW_SET_ERROR_MSG("failed to allocate wait set");
    goto fail;
  }

  wait_set_info->wait_set = dds_WaitSet_create();
  if (wait_set_info->wait_set == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate wait set");
    goto fail;
  }

  wait_set_info->active_conditions = dds_ConditionSeq_create(4);
  if (wait_set_info->active_conditions == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate active_conditions sequence");
    goto fail;
  }

  wait_set_info->attached_conditions = dds_ConditionSeq_create(4);
  if (wait_set_info->attached_conditions == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate attached_conditions sequence");
    goto fail;
  }

  return wait_set;

fail:
  if (wait_set_info != nullptr) {
    if (wait_set_info->active_conditions != nullptr) {
      dds_ConditionSeq_delete(wait_set_info->active_conditions);
    }

    if (wait_set_info->attached_conditions != nullptr) {
      dds_ConditionSeq_delete(wait_set_info->attached_conditions);
    }

    if (wait_set_info->wait_set != nullptr) {
      dds_WaitSet_delete(wait_set_info->wait_set);
    }

    wait_set_info = nullptr;
  }

  if (wait_set != nullptr) {
    if (wait_set->data != nullptr) {
      rmw_free(wait_set->data);
    }

    rmw_wait_set_free(wait_set);
  }

  return nullptr;
}

rmw_ret_t
shared__rmw_destroy_wait_set(const char * implementation_identifier, rmw_wait_set_t * wait_set)
{
  if (wait_set == nullptr) {
    RMW_SET_ERROR_MSG("wait set handle is null");
    return RMW_RET_ERROR;
  }

  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    wait_set handle, wait_set->implementation_identifier,
    implementation_identifier, return RMW_RET_ERROR);

  CoreddsWaitSetInfo * wait_set_info = static_cast<CoreddsWaitSetInfo *>(wait_set->data);

  if (wait_set_info->active_conditions != nullptr) {
    dds_ConditionSeq_delete(wait_set_info->active_conditions);
  }

  if (wait_set_info->attached_conditions != nullptr) {
    dds_ConditionSeq_delete(wait_set_info->attached_conditions);
  }

  if (wait_set_info->wait_set != nullptr) {
    dds_WaitSet_delete(wait_set_info->wait_set);
  }

  wait_set_info = nullptr;

  if (wait_set->data != nullptr) {
    rmw_free(wait_set->data);
  }

  if (wait_set != nullptr) {
    rmw_wait_set_free(wait_set);
  }

  return RMW_RET_OK;
}
