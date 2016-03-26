cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("My Project")

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(libfreenect REQUIRED)

include_directories("/usr/include/libusb-1.0/")

add_executable(test src/test.cpp)
target_link_libraries(test  ${OpenCV_LIBS}
                               ${CMAKE_THREAD_LIBS_INIT}
                               ${FREENECT_LIBRARIES})

# add_executable(regtest src/regtest.cpp
#                       src/features.cpp)
# target_link_libraries(regtest  ${OpenCV_LIBS}
#                               ${CMAKE_THREAD_LIBS_INIT}
#                               ${FREENECT_LIBRARIES})

# add_executable(main src/main.cpp
#                    src/features.cpp)
# target_link_libraries(main  ${OpenCV_LIBS}
#                            ${CMAKE_THREAD_LIBS_INIT}
#                            ${FREENECT_LIBRARIES})
