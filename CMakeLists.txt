cmake_minimum_required(VERSION 3.16...3.21)
project(Betavoltaic_Opt)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(betavoltaic betavoltaic.cc ${sources} ${headers})
target_link_libraries(betavoltaic ${Geant4_LIBRARIES})

# Copy the macro file to the build directory
file(COPY run.mac DESTINATION ${CMAKE_BINARY_DIR})