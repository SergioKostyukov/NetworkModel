cmake_minimum_required(VERSION 3.21)
project(Computer_networks)

set(CMAKE_CXX_STANDARD 20)

add_executable(Computer_networks main.cpp CommunicationNode.cpp CommunicationNode.h Channel.h Workstation.cpp Workstation.h NetworkModel.cpp NetworkModel.h)
