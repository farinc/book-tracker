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

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_subdirectory(nlohmann_json)

find_package(Qt5 5.15 COMPONENTS Widgets REQUIRED)

add_executable(booktracker WIN32
    resource.qrc)

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


target_link_libraries(booktracker PUBLIC Qt5::Widgets PRIVATE nlohmann_json::nlohmann_json)
install(TARGETS booktracker DESTINATION bin)


if(LINUX)
    include(GNUInstallDirs)
    install(FILES booktracker.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(FILES booktracker.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
endif()

set(CPACK_SOURCE_IGNORE_FILES .git/ .github/ .vscode/ .mypy_cache/ _CPack_Packages/ ${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/)

# Set package properties
set(CPACK_PACKAGE_NAME "book-tracker")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_STRING})
set(CPACK_PACKAGE_DESCRIPTION "Amanda's Book Tracker")
set(CPACK_PACKAGE_CONTACT "forrest.ram64@gmail.com")

# DEB
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5t64")
set(CPACK_DEBIAN_PACKAGE_SECTION "editors")

# RPM
set(CPACK_RPM_PACKAGE_REQUIRES "qt5-qtbase")

# NSIS
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
set(CMAKE_INSTALL_BINDIR bin)

# Include CPack module
include(CPack)
