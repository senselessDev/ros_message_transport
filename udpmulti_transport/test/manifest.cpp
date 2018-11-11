#include <ros/ros.h>
#include <std_msgs/Header.h>
#include <pluginlib/class_list_macros.hpp>
#include "udpmulti_transport/udpmulti_publisher.h"
#include "udpmulti_transport/udpmulti_subscriber.h"

PLUGINLIB_EXPORT_CLASS(udpmulti_transport::UDPMultiPublisher<std_msgs::Header>, message_transport::PublisherPlugin<std_msgs::Header>)
PLUGINLIB_EXPORT_CLASS(udpmulti_transport::UDPMultiSubscriber<std_msgs::Header>, message_transport::SubscriberPlugin<std_msgs::Header>)