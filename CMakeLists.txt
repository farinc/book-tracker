cmake_minimum_required(VERSION 3.5)
project(booktracker VERSION 3.3.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_VERSION_STRING "${CMAKE_PROJECT_VERSION}")

option(GENERATE_BUNDLE "Build a bundle/installer for the current plateform" OFF)

set(JSON_BuildTests OFF CACHE INTERNAL "") # nlohmann_json only

configure_file(version_config.h.in ${CMAKE_BINARY_DIR}/generated/version_config.h)
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # Make sure it can be included...

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
  qdarkstyle/dark/dark_style.qrc
  qdarkstyle/light/light_style.qrc
)

# add_custom_target( bundle COMMAND "${CMAKE_CPACK_COMMAND}" "-C" "$<CONFIGURATION>" "--config" "${CMAKE_BINARY_DIR}/BundleConfig.cmake" COMMENT "CPack CMake target")

target_link_libraries(booktracker Qt5::Widgets nlohmann_json::nlohmann_json)
install(TARGETS booktracker COMPONENT base)

set(CPACK_PACKAGE_NAME "Book Tracker")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A program to manage Amanda's Books")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "BookTracker")
set(CPACK_PACKAGE_EXECUTABLES "booktracker;Book Tracker")
set(CPACK_IFW_PACKAGE_TITLE "Installer for Book Tracker")
set(CPACK_PACKAGE_FILE_NAME "installer")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installation Tool")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_COMPONENTS_ALL base)

# Keeps stuff clean...
add_custom_command(
    TARGET booktracker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bundle
)

if(GENERATE_BUNDLE)
    include(cmake/DeployQt.cmake)
    # At this point, we grab the dependencies of the program. This is handled by different "deployers"
    if(WIN32)
        list(APPEND CPACK_GENERATOR NSIS)
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
        set(CMAKE_INSTALL_BINDIR bin)
        windeployqt(booktracker)
    elseif(APPLE)
        macdeployqt(booktracter)
    elseif(UNIX)
        list(APPEND CPACK_GENERATOR External)
        include(GNUInstallDirs) #Sometimes, the correct paths dont get set for some reason...
        linuxdeployqt(booktracker)
    endif()

    # Sets a task to place the shared libraries in the build directory "bundle" folder and place it in the install binary directory (where the binary is).
    # Furthermore, the COMPONENT requirement makes it such that this only happens when the "base" conponent is activitated (which occurs during install)
    install(
        DIRECTORY ${CMAKE_BINARY_DIR}/bundle/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT base
    )

    include(CPack REQUIRED)
    include(CPackComponent REQUIRED)

    # The base component. This is where it is defined...
    cpack_add_component(
        base
        DISPLAY_NAME "Base Program"
        DESCRIPTION "Install me"
        REQUIRED
    )
endif()
