cmake_minimum_required(VERSION 2.8)

project(Monografia)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(VTK_DIR "/PATH/TO/VTK/BUILD/DIRECTORY")
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

file(GLOB my_source_files "*.h" "*.cpp")
add_executable(Visualizador ${my_source_files} )     #MyDll

target_link_libraries(Visualizador ${ITK_LIBRARIES} ${VTK_LIBRARIES})