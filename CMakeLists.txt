cmake_minimum_required(VERSION 3.14)
project(learn_os_embedded_fw)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################################
#           Subdirectories         #
####################################
add_subdirectory(operating_systems)
add_subdirectory(linux_device_driver)