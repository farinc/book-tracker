cmake_minimum_required(VERSION 3.5)
project(book-tracker VERSION 3.2.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_VERSION_STRING "${CMAKE_PROJECT_VERSION}")

configure_file( version_config.h.in ${CMAKE_BINARY_DIR}/generated/version_config.h )
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

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  book.cpp
  book.h
  bookdialog.cpp
  bookdialog.h
  bookdialog.ui
  costitemsmodel.cpp
  costitemsmodel.h
  models.cpp
  models.h
  savedialog.cpp
  savedialog.h
  savedialog.ui
  settingsdialog.cpp
  settingsdialog.h
  settingsdialog.ui
)

target_link_libraries(${CMAKE_PROJECT_NAME} Qt::Core Qt::Widgets)
