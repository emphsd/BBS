cmake_minimum_required(VERSION 3.15)

project(Mine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(VCPKG_BUILD_TYPE release)
# -DCMAKE_TOOLCHAIN_FILE=C:/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake

find_package(Eigen3 REQUIRED)
set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv2")
set(OpenCV_INCLUDE_DIRS "${VCPKG_INSTALLED_DIR}/x64-windows/include/opencv2")
set(TIFF_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/packages/tiff_x64-windows-static/include")
set(TIFF_LIBRARY "${VCPKG_INSTALLED_DIR}/packages/tiff_x64-windows-static/lib/tiff.lib")
set(TIFF_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/tiff")

# Import OpenCV library
find_package(OpenCV CONFIG REQUIRED)
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

find_package(TIFF)
# find_package(protobuf CONFIG REQUIRED)


link_directories( ${OpenCV_LIB_DIR} )
link_libraries(${OpenCV_LIBS})

add_executable(bbs src/main.cpp src/bbs.cpp)
target_link_libraries(bbs PRIVATE ${OpenCV_LIBS})
target_link_libraries(bbs PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)
# target_link_libraries(bbs PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)


# add_executable(bbs src/main.cpp
# )