cmake_minimum_required(VERSION 3.5)
project(booktracker VERSION 3.3.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_VERSION_STRING "${CMAKE_PROJECT_VERSION}")

set(JSON_BuildTests OFF CACHE INTERNAL "") # nlohmann_json only

configure_file(version_config.h.in ${CMAKE_BINARY_DIR}/generated/version_config.h)
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # Make sure it can be included...

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_subdirectory(nlohmann_json)

find_package(Qt5 5.15 COMPONENTS Widgets REQUIRED)

add_executable(booktracker WIN32)

target_sources(booktracker PRIVATE
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  book.cpp
  book.h
  bookdialog.cpp
  bookdialog.h
  bookdialog.ui
  models.cpp
  models.h
  reviewdialog.cpp
  reviewdialog.h
  reviewdialog.ui
  settingsdialog.cpp
  settingsdialog.h
  settingsdialog.ui
  uilogic.h
  uilogic.cpp
  confirmdialog.h
  confirmdialog.cpp
  confirmdialog.ui
)

# add_custom_target( bundle COMMAND "${CMAKE_CPACK_COMMAND}" "-C" "$<CONFIGURATION>" "--config" "${CMAKE_BINARY_DIR}/BundleConfig.cmake" COMMENT "CPack CMake target")

target_link_libraries(booktracker Qt5::Widgets nlohmann_json::nlohmann_json)
install(TARGETS booktracker COMPONENT base)

include(cmake/DeployQt.cmake)
initBundle()
