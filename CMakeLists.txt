cmake_minimum_required(VERSION 3.26)
project(RM_ArmorDetect)

set(CMAKE_CXX_STANDARD 14)

# 旧协议指令屏蔽
if(POLICY CMP0146)
    cmake_policy(SET CMP0146 OLD)
endif()

#========== cuda ==========
set(CMAKE_CUDA_ARCHITECTURES 86)
set(CMAKE_CUDA_COMPILER "/usr/local/cuda-12.6/bin/nvcc")
find_package(CUDA REQUIRED)

include_directories(/usr/local/cuda-12.6/include)
link_directories(/usr/local/cuda-12.6/lib64)

include_directories(/home/valmorx/DeepLearningSource/cudnn-8.9.7/include)
link_directories(/home/valmorx/DeepLearningSource/cudnn-8.9.7/lib)


#========== OpenCV ==========
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(${PROJECT_BINARY_DIR}/../src SRC)
include_directories(${PROJECT_BINARY_DIR}/../include)
link_directories(${PROJECT_BINARY_DIR}/../lib)

add_library(Local_Lib_Static STATIC ${SRC})
add_library(Local_Lib_Shared SHARED ${SRC})

set_target_properties(Local_Lib_Shared PROPERTIES OUTPUT_NAME "Local_Lib")
set_target_properties(Local_Lib_Static PROPERTIES OUTPUT_NAME "Local_Lib")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_executable(RM_ArmorDetect
        main.cpp
        ${SRC}
)

target_link_libraries(RM_ArmorDetect ${OpenCV_LIBS})
