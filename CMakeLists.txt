cmake_minimum_required(VERSION 3.5)

include(FetchContent)

project(book-tracker LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_EXTERNAL_JSON "Use an external JSON library" OFF)
add_subdirectory(thirdparty)

# Note that the namespaced target will always be available regardless of the
# import method

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(book-tracker SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    book.cpp
    book.h
    bookdialog.cpp
    bookdialog.h
    bookdialog.ui
  )
else()
  add_executable(book-tracker
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    book.cpp
    book.h
    bookdialog.cpp
    bookdialog.h
    bookdialog.ui
  )
endif()

target_link_libraries(book-tracker PRIVATE Qt5::Widgets)
target_link_libraries(book-tracker PRIVATE nlohmann_json::nlohmann_json)
