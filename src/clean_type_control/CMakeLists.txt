cmake_minimum_required(VERSION 3.0.2)
project(clean_type_control)

set(PIGPIO_LIBRARY_LF2 /home/ubuntu/pigpio-master/libpigpiod_if2.so )
set(PIGPIO_LIBRARY_LF /home/ubuntu/pigpio-master/libpigpiod_if.so )
set(PIGPIO_LIBRARY /home/ubuntu/pigpio-master/libpigpio.so )
set(pigpio_INCLUDE_DIRS /usr/local/include/)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

# find_package(pigpiod_if REQUIRED)  # Specify pigpio library

catkin_package(
#  INCLUDE_DIRS include /usr/local/lib/
#  LIBRARIES clean_type_control
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
CATKIN_DEPENDS roscpp std_msgs
# DEPENDS pigpio
# DEPENDS ${PIGPIO_LIBRARY}  # Specify pigpio library as a dependency
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${pigpio_INCLUDE_DIRS}
)


add_executable(clean_type_control_node /home/ubuntu/test_robot/src/clean_type_control/src/clean_type_control_node.cpp)
target_link_libraries(clean_type_control_node
  ${catkin_LIBRARIES}
  ${PIGPIO_LIBRARY} # Add pigpio library
  ${PIGPIO_LIBRARY_LF2} # Add pigpio library
  ${PIGPIO_LIBRARY_LF} # Add pigpio library

)

