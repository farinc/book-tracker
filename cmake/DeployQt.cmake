find_package(Qt5Core REQUIRED)

# Retrieve the absolute path to qmake and then use that path to find
# the windeployqt and macdeployqt binaries
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
if(WIN32 AND NOT WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "windeployqt not found")
endif()

find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
if(APPLE AND NOT MACDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "macdeployqt not found")
endif()

find_program(LINUXDEPLOYQT_EXECUTABLE linuxdeployqt)
if(UNIX AND NOT LINUXDEPLOYQT_EXECUTABLE)
    message(ERROR "linuxdeployqt not found")
    message("Be sure to pass in the correct path to executable with -DLINUXDEPLOYQT_EXECUTABLE")
endif()

# Add commands that copy the required Qt files to the same directory as the
# target after being built as well as including them in final installation
function(windeployqt target)

    # Run windeployqt immediately after build
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                --verbose 0
                --no-compiler-runtime
                --no-angle
                --no-opengl-sw
                --dir \"$<TARGET_FILE_DIR:${target}>/libs\"
                \"$<TARGET_FILE:${target}>\"
        COMMENT "Deploying Qt..."
    )

    # windeployqt doesn't work correctly with the system runtime libraries,
    # so we fall back to one of CMake's own modules for copying them over

    # Doing this with MSVC 2015 requires CMake 3.6+
    if((MSVC_VERSION VERSION_EQUAL 1900 OR MSVC_VERSION VERSION_GREATER 1900)
            AND CMAKE_VERSION VERSION_LESS "3.6")
        message(WARNING "Deploying with MSVC 2015+ requires CMake 3.6+")
    endif()

    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
    foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
        get_filename_component(filename "${lib}" NAME)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                copy_if_different "${lib}" \"$<TARGET_FILE_DIR:${target}>/bundle\"
            COMMENT "Copying ${filename}..."
        )
    endforeach()
endfunction()

# Add commands that copy the required Qt files to the application bundle
# represented by the target.
function(macdeployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            \"$<TARGET_FILE_DIR:${target}>/bundle\"
            -always-overwrite
        COMMENT "Deploying Qt..."
    )
endfunction()

function(linuxdeployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${LINUXDEPLOYQT_EXECUTABLE}"
            \"$<TARGET_FILE_DIR:${target}>/bundle\"
            -always-overwrite
        COMMENT "Deploying Qt..."
    )
endfunction()

macro(initBundle)

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

    include(CPack REQUIRED)
    include(CPackComponent)

    add_custom_command(
        TARGET booktracker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bundle
    )

    # At this point, we grab the dependencies of the program. This is handled by different "deployers"
    if(WIN32)
        set(CPACK_GENERATOR NSIS)
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        set(CPACK_NSIS_DISPLAY_NAME "Book Tracker")
        windeployqt(booktracker)
    elseif(APPLE)
        macdeployqt(booktracter)
    elseif(UNIX)
        set(CPACK_GENERATOR External)
        linuxdeployqt(booktracker)
    endif()

    # Sets a task to place the shared libraries in the build directory "bundle" folder and place it in the install binary directory (where the binary is).
    # Furthermore, the COMPONENT requirement makes it such that this only happens when the "base" conponent is activitated (which occurs during install)
    install(
        DIRECTORY ${CMAKE_BINARY_DIR}/bundle/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT base
    )

    cpack_add_component(
        base
        DISPLAY_NAME "Base Program"
        DESCRIPTION "Install me"
        REQUIRED
    )

endmacro()

mark_as_advanced(WINDEPLOYQT_EXECUTABLE MACDEPLOYQT_EXECUTABLE LINUXDEPLOYQT_EXECUTABLE)
