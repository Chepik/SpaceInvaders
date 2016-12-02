cmake_minimum_required(VERSION 2.8)

add_definitions(--std=c++11)

# Set up your project name.
set(PROJECT_NAME SpaceInvaders)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/bin")

set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(JSONCPP_WITH_TESTS OFF)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)
add_subdirectory(3party/jsoncpp)
include_directories(3party/jsoncpp/include)
aux_source_directory(3party/jsoncpp/src/lib_json JSONCPP_SRC)

# Set up the main source folder.
set(SOURCE_ROOT src)

file(GLOB_RECURSE INCS "src/*.hpp" "src/*.ui")

# Scan source folder to find all sources and put the result to SRC_LIST.
aux_source_directory(${SOURCE_ROOT} SRC_LIST)
qt5_wrap_cpp(QT_WRAPPED_SRC ${SRC_LIST})

# Create executable by SRC_LIST.
add_executable(${PROJECT_NAME} ${SRC_LIST} ${QT_WRAPPED_SRC} ${INCS} ${JSONCPP_SRC})

# Qt modules
qt5_use_modules(${PROJECT_NAME} Widgets OpenGL)

# Add subdirectory with Google Test Library.
add_subdirectory(3party/googletest)

# Enable unit testing.
enable_testing()

# Set up testing project name.
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

# Add include directories for testing project.
include_directories(3party/googletest/googletest/include ${SOURCE_ROOT})

# Set up testing project.
set(TESTS_SOURCE_ROOT tests)

aux_source_directory(${TESTS_SOURCE_ROOT} TEST_SRC_FILES)

set(TEST_SRC_FILES ${SRC_LIST} ${TEST_SRC_FILES})

list(REMOVE_ITEM TEST_SRC_FILES src/main.cpp)

qt5_wrap_cpp(QT_WRAPPED_TEST_SRC ${TEST_SRC_FILES})

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES} ${QT_WRAPPED_TEST_SRC} ${JSONCPP_SRC})

# Link gtest and gtest_main libraries.
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)
qt5_use_modules(${PROJECT_TEST_NAME} Widgets OpenGL)

# Finish tests setting up.
add_test(test ${PROJECT_TEST_NAME})

macro(copy_files GLOBPAT DESTINATION)
  file(GLOB COPY_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${GLOBPAT})
  foreach(FILENAME ${COPY_FILES})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
    set(DST "${DESTINATION}/${FILENAME}")
    add_custom_command(TARGET ${PROJECT_TEST_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
  endforeach(FILENAME)
endmacro(copy_files)

set(DATA_FILES_LIST data/alien.png data/star.png)
copy_files("${DATA_FILES_LIST}" ${CMAKE_CURRENT_BINARY_DIR})
