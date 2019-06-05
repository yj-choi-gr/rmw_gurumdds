#include <limits>
#include "rmw_coredds_shared_cpp/qos.hpp"

bool is_time_default(const rmw_time_t & time)
{
  return time.sec == 0 && time.nsec == 0;
}

dds_Duration_t
rmw_time_to_dds(const rmw_time_t & time)
{
  dds_Duration_t duration;
  duration.sec = static_cast<int32_t>(time.sec);
  duration.nanosec = static_cast<uint32_t>(time.nsec);
  return duration;
}

template<typename dds_EntityQos>
bool
set_entity_qos_from_profile_generic(
  const rmw_qos_profile_t * qos_profile,
  dds_EntityQos * entity_qos)
{
  switch (qos_profile->history) {
    case RMW_QOS_POLICY_HISTORY_KEEP_LAST:
      entity_qos->history.kind = dds_KEEP_LAST_HISTORY_QOS;
      break;
    case RMW_QOS_POLICY_HISTORY_KEEP_ALL:
      entity_qos->history.kind = dds_KEEP_ALL_HISTORY_QOS;
      break;
    case RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT:
      break;
    default:
      RMW_SET_ERROR_MSG("unknown qos history policy");
      return false;
  }

  switch (qos_profile->reliability) {
    case RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT:
      entity_qos->reliability.kind = dds_BEST_EFFORT_RELIABILITY_QOS;
      break;
    case RMW_QOS_POLICY_RELIABILITY_RELIABLE:
      entity_qos->reliability.kind = dds_RELIABLE_RELIABILITY_QOS;
      break;
    case RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT:
      break;
    default:
      RMW_SET_ERROR_MSG("unknown qos reliability policy");
      return false;
  }

  switch (qos_profile->durability) {
    case RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL:
      entity_qos->durability.kind = dds_TRANSIENT_LOCAL_DURABILITY_QOS;
      break;
    case RMW_QOS_POLICY_DURABILITY_VOLATILE:
      entity_qos->durability.kind = dds_VOLATILE_DURABILITY_QOS;
      break;
    case RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT:
      break;
    default:
      RMW_SET_ERROR_MSG("unknown qos durability policy");
      return false;
  }

  if (qos_profile->depth != RMW_QOS_POLICY_DEPTH_SYSTEM_DEFAULT) {
    entity_qos->history.depth = static_cast<int32_t>(qos_profile->depth);
  }

  // ensure the history depth is at least the requested queue size
  assert(entity_qos->history.depth >= 0);
  if (
    entity_qos->history.kind == dds_KEEP_LAST_HISTORY_QOS &&
    static_cast<size_t>(entity_qos->history.depth) < qos_profile->depth)
  {
    if (qos_profile->depth > (std::numeric_limits<int32_t>::max)()) {
      RMW_SET_ERROR_MSG(
        "failed to set history depth since the requested queue size exceeds the DDS type");
      return false;
    }
    entity_qos->history.depth = static_cast<int32_t>(qos_profile->depth);
  }

  return true;
}

bool
get_datawriter_qos(
  dds_Publisher * publisher,
  const rmw_qos_profile_t * qos_profile,
  dds_DataWriterQos * datawriter_qos)
{
  dds_ReturnCode_t ret = dds_Publisher_get_default_datawriter_qos(publisher, datawriter_qos);
  if (ret != dds_RETCODE_OK) {
    RMW_SET_ERROR_MSG("failed to get default datawriter qos");
    return false;
  }

  set_entity_qos_from_profile_generic(qos_profile, datawriter_qos);

  return true;
}

bool get_datareader_qos(
  dds_Subscriber * subscriber,
  const rmw_qos_profile_t * qos_profile,
  dds_DataReaderQos * datareader_qos)
{
  dds_ReturnCode_t ret = dds_Subscriber_get_default_datareader_qos(subscriber, datareader_qos);
  if (ret != dds_RETCODE_OK) {
    RMW_SET_ERROR_MSG("failed to get default datareader qos");
    return false;
  }

  set_entity_qos_from_profile_generic(qos_profile, datareader_qos);

  return true;
}
