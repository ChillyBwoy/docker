cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(embed)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(embed ${PROJECT_SOURCE_DIR}/src/embed.cpp)
target_link_libraries(embed
  /opt/v8-embed.x86_64/obj/libv8_base.a
  /opt/v8-embed.x86_64/obj/libv8_libbase.a
  /opt/v8-embed.x86_64/obj/libv8_nosnapshot.a
  /opt/v8-embed.x86_64/obj/libv8_snapshot.a
  /opt/v8-embed.x86_64/obj/libv8_libplatform.a
  /opt/v8-embed.x86_64/obj/libv8_libsampler.a
  rt
  dl
  Threads::Threads)
