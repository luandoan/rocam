cmake_minimum_required(VERSION 2.8.3)
project(rocam)

find_package(catkin REQUIRED
    COMPONENTS
      roscpp
      message_generation
      sensor_msgs
      dynamic_reconfigure
      driver_base
      cv_bridge
      image_transport
      camera_info_manager
      tf)

find_package(OpenCV REQUIRED)

add_message_files(
      FILES
      velocity.msg
)

generate_messages(
      DEPENDENCIES
      std_msgs
)

catkin_package(
    CATKIN_DEPENDS
      roscpp
      message_runtime
      sensor_msgs
      dynamic_reconfigure
      driver_base
      cv_bridge
      image_transport
      camera_info_manager
      tf)



include_directories(
      include
      {catkin_INCLUDE_DIRS}
      ${OpenCV_INCLUDE_DIRS}
)

add_executable(rocam_video src/read_video_stream_node.cpp)
target_link_libraries(rocam_video ${catkin_LIBRARIES})

add_executable(rocam_control src/control_camera_node.cpp)
target_link_libraries(rocam_control ${catkin_LIBRARIES} curl)

add_executable(rocam_track src/track_cam_node.cpp)
target_link_libraries(rocam_track ${catkin_LIBRARIES})

add_executable(joy_rocam src/joy_rocam_node.cpp)
target_link_libraries(joy_rocam ${catkin_LIBRARIES})

#install(TARGETS camera camera_timer camera_polling camera_stereo homography
#  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#)

# foreach(dir bag config launch)
#    install(DIRECTORY ${dir}/
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
#endforeach(dir)
