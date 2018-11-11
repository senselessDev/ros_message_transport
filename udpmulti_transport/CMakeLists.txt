cmake_minimum_required(VERSION 2.8.3)
project(udpmulti_transport)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs pluginlib message_generation message_transport_common)

#find_package(Boost REQUIRED COMPONENTS thread)



# Generate messages in the 'msg' folder
add_message_files(
  FILES
  UDPMultHeader.msg
  UDPMultTopic.msg
)

# Generate services in the 'srv' folder
add_service_files(
  FILES
  UDPMultClearAll.srv
  UDPMultGetTopicList.srv
  UDPMultRegisterTopic.srv
)

include_directories(
include
${catkin_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} src/udpmulti_publisher.cpp) 
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${catkin_LIBRARIES})

add_library(test-manifest test/manifest.cpp)
target_link_libraries(test-manifest ${PROJECT_NAME} ${catkin_LIBRARIES})
 
add_executable(udpmulti_manager src/udpmulti_manager.cpp)
target_link_libraries(udpmulti_manager ${PROJECT_NAME} ${catkin_LIBRARIES})

## Generate added messages and services with any dependencies listed here
generate_messages(
    #TODO DEPENDENCIES geometry_msgs std_msgs
)

# catkin_package parameters: http://ros.org/doc/groovy/api/catkin/html/dev_guide/generated_cmake_api.html#catkin-package
# TODO: fill in what other packages will need to use this package
catkin_package(
    DEPENDS std_msgs message_transport_common
    CATKIN_DEPENDS # TODO
    INCLUDE_DIRS include
    LIBRARIES # TODO
)