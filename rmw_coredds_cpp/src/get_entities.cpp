#include "rmw_coredds_shared_cpp/types.hpp"
#include "rmw_coredds_cpp/get_entities.hpp"
#include "rmw_coredds_cpp/identifier.hpp"
#include "rmw_coredds_cpp/types.hpp"

namespace rmw_coredds_cpp
{
dds_DomainParticipant *
get_participant(rmw_node_t * node)
{
  if (node == nullptr) {
    return nullptr;
  }

  if (node->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsNodeInfo * impl = static_cast<CoreddsNodeInfo *>(node->data);
  return impl->participant;
}

dds_Publisher *
get_publisher(rmw_publisher_t * publisher)
{
  if (publisher == nullptr) {
    return nullptr;
  }

  if (publisher->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsPublisherInfo * impl = static_cast<CoreddsPublisherInfo *>(publisher->data);
  return impl->publisher;
}

dds_DataWriter *
get_data_writer(rmw_publisher_t * publisher)
{
  if (publisher == nullptr) {
    return nullptr;
  }

  if (publisher->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsPublisherInfo * impl = static_cast<CoreddsPublisherInfo *>(publisher->data);
  return impl->topic_writer;
}

dds_Subscriber *
get_subscriber(rmw_subscription_t * subscription)
{
  if (subscription == nullptr) {
    return nullptr;
  }

  if (subscription->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsSubscriberInfo * impl = static_cast<CoreddsSubscriberInfo *>(subscription->data);
  return impl->subscriber;
}

dds_DataReader *
get_data_reader(rmw_subscription_t * subscription)
{
  if (subscription == nullptr) {
    return nullptr;
  }

  if (subscription->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsSubscriberInfo * impl = static_cast<CoreddsSubscriberInfo *>(subscription->data);
  return impl->topic_reader;
}

dds_DataWriter *
get_request_data_writer(rmw_client_t * client)
{
  if (client == nullptr) {
    return nullptr;
  }

  if (client->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsClientInfo * impl = static_cast<CoreddsClientInfo *>(client->data);
  return impl->request_writer;
}

dds_DataReader *
get_response_data_writer(rmw_client_t * client)
{
  if (client == nullptr) {
    return nullptr;
  }

  if (client->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsClientInfo * impl = static_cast<CoreddsClientInfo *>(client->data);
  return impl->response_reader;
}

dds_DataReader *
get_request_data_reader(rmw_service_t * service)
{
  if (service == nullptr) {
    return nullptr;
  }

  if (service->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsServiceInfo * impl = static_cast<CoreddsServiceInfo *>(service->data);
  return impl->request_reader;
}

dds_DataWriter *
get_response_data_writer(rmw_service_t * service)
{
  if (service == nullptr) {
    return nullptr;
  }

  if (service->implementation_identifier != gurum_coredds_identifier) {
    return nullptr;
  }

  CoreddsServiceInfo * impl = static_cast<CoreddsServiceInfo *>(service->data);
  return impl->response_writer;
}
}  // namespace rmw_coredds_cpp
