cmake_minimum_required(VERSION 2.8.12)
project(res C CXX)

if(NOT DEFINED CMAKE_MACOSX_RPATH)
  # There is some weirdness around this variable, the default value is different depending on
  # the cmake version, see policy CMP0042.
  # A more explicit way to treat this would be `cmake_policy(SET CMP0042 NEW)` but that would
  # fail on CMake 2.8.12 (the variable exists but the policy doesn't)
  set(CMAKE_MACOSX_RPATH ON)
endif()

include(GNUInstallDirs)
enable_testing()

#-----------------------------------------------------------------

set( RES_VERSION_MAJOR 0 )
set( RES_VERSION_MINOR 0 )
set( RES_VERSION_MICRO "not-available" )

# If the micro version is not integer, that should be interpreted as a
# development version leading towards version MAJOR.MINOR.0

execute_process(COMMAND date "+%Y-%m-%d %H:%M:%S" OUTPUT_VARIABLE RES_BUILD_TIME )
string(STRIP ${RES_BUILD_TIME} RES_BUILD_TIME)

find_package(Git)

if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} "--git-dir=${CMAKE_SOURCE_DIR}/.git" status
    RESULT_VARIABLE RESULT
    OUTPUT_QUIET
    ERROR_QUIET
    )

  if(NOT "${RESULT}" STREQUAL 0)
    set(GIT_FOUND OFF)
  endif()
endif()

if(GIT_FOUND)
   execute_process(
     COMMAND ${GIT_EXECUTABLE} "--git-dir=${CMAKE_SOURCE_DIR}/.git" rev-parse HEAD
     OUTPUT_VARIABLE GIT_COMMIT
     OUTPUT_STRIP_TRAILING_WHITESPACE
   )

   execute_process(
     COMMAND ${GIT_EXECUTABLE} "--git-dir=${CMAKE_SOURCE_DIR}/.git" rev-parse --short HEAD
     OUTPUT_VARIABLE GIT_COMMIT_SHORT
     OUTPUT_STRIP_TRAILING_WHITESPACE
   )

   execute_process(
     COMMAND ${GIT_EXECUTABLE} "--git-dir=${CMAKE_SOURCE_DIR}/.git" describe --tags
     OUTPUT_VARIABLE GIT_TAG
     OUTPUT_STRIP_TRAILING_WHITESPACE
   )

   string( REGEX REPLACE "^([^.]+)\\.([^.]+)\\.(.+)$" "\\1" RES_VERSION_MAJOR "${GIT_TAG}")
   string( REGEX REPLACE "^([^.]+)\\.([^.]+)\\.(.+)$" "\\2" RES_VERSION_MINOR "${GIT_TAG}")
   string( REGEX REPLACE "^([^.]+)\\.([^.]+)\\.(.+)$" "\\3" RES_VERSION_MICRO "${GIT_TAG}")
else()
    set( GIT_COMMIT "unknown (git not found!)")
    set( GIT_COMMIT_SHORT "unknown (git not found!)")
    message( WARNING "Git not found. Build will not contain correct version info." )
endif()

message( STATUS "libres version: ${RES_VERSION_MAJOR}.${RES_VERSION_MINOR}.${RES_VERSION_MICRO}" )

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CXX11Features)

#-----------------------------------------------------------------

option( BUILD_TESTS         "Should the tests be built"             OFF)
option( ENABLE_PYTHON       "Enable the python wrappers"            ON )
option( USE_RPATH           "Should we embed path to libraries"     ON )
option( INSTALL_ERT_LEGACY  "Install legacy ert code"               OFF)
option( ERT_LSF_SUBMIT_TEST "Build and run tests of LSF submit"     OFF)

set( SHARE_DIR "share/ert")
# If the SITE_CONFIG_FILE is not set as -DSITE_CONFIG_FILE switch when invoking
# cmake we set it here to location of the to-be-installed site-config file. That
# implies that when testing the binary will have embedded a path to non-existent
# or stale version of the site configuration file. Irrespective of the value of
# SITE_CONFIG_FILE the environment variable ERT_SITE_CONFIG will be set to point
# to the in-source version of the site-config file for testing.
#
# The variable SITE_CONFIG_FILE can be set by the user, if that is set it will
# take presedence. If the user has not explicitly set the SITE_CONFIG_FILE variable
# we will use the default path based on the install prefix.
set( SITE_CONFIG_FILE "" CACHE FILEPATH "Path to global ERT Configuration file")
set( __SITE_CONFIG_FILE "${CMAKE_INSTALL_PREFIX}/${SHARE_DIR}/site-config")
if (SITE_CONFIG_FILE)
  set( __SITE_CONFIG_FILE ${SITE_CONFIG_FILE})
endif()
message(STATUS "The path ${__SITE_CONFIG_FILE} will be compiled into the libres library as the location of the site configuration file")

if (USE_RPATH)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

#-----------------------------------------------------------------

find_package( ecl REQUIRED )

set(ERT_LSF_LIB_PATH     "" CACHE FILEPATH "Path to search for the LSF libraries")
set(ERT_LSF_INCLUDE_PATH "" CACHE FILEPATH "Path to search for the LSF header files")
find_path(LSF_HEADER_PATH lsf/lsf.h PATHS ${ERT_LSF_INCLUDE_PATH})
find_library(LSF_LIBRARY NAMES lsf PATHS ${ERT_LSF_LIB_PATH})

#-----------------------------------------------------------------

set(EQUINOR_TESTDATA_ROOT "" CACHE PATH  "Root to Equinor internal testdata")
if (EXISTS ${EQUINOR_TESTDATA_ROOT})
    set( LINK "${CMAKE_CURRENT_SOURCE_DIR}/test-data/Equinor" )
    if (EXISTS ${LINK})
        execute_process(COMMAND ${CMAKE_COMMAND} -E remove "${LINK}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${EQUINOR_TESTDATA_ROOT}" "${LINK}")
    message(STATUS "Linking testdata: ${LINK} -> ${EQUINOR_TESTDATA_ROOT}")
endif()

file(COPY bin/job_dispatch.py DESTINATION ${PROJECT_BINARY_DIR}/bin )
set( ERT_ROOT "${PROJECT_BINARY_DIR}" )
#-----------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif()

if (MSVC)
    add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4996" )
else ()
    set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wno-unknown-pragmas ${CMAKE_C_FLAGS}")
endif()

if (NOT BUILD_SHARED_LIBS)
    message(WARNING "Building python - forcing shared libs.")
    set(BUILD_SHARED_LIBS ON)
endif ()

set(INSTALL_GROUP ""
        CACHE STRING "Group to install as - blank to install as current group")
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
#-----------------------------------------------------------------

find_package(Threads)
if (CMAKE_USE_PTHREADS_INIT)
    set(HAVE_PTHREAD TRUE)
endif ()

# feature tests
include(CheckFunctionExists)
check_function_exists( regexec ERT_HAVE_REGEXP )

#-----------------------------------------------------------------
# install_example() is a small utility function which is used to install an
# example file. If a file with the same name already exists in the installation
# directory the example file will not be reinstalled on top.
#
# The decision whether to install the example file or not is evaluated at
# configure - time; i.e. there is a race open between configure time and
# installation. Should ideally use the INSTALL(CODE ...) functionality to defer
# the decision to installation time.

function(install_example src_file destination)

  set (_full_destination "${CMAKE_INSTALL_PREFIX}/${destination}")
  if (EXISTS "${_full_destination}/${src_file}")
    message(STATUS "File: ${_full_destination}/${src_file} already exists - will not be updated")
  else()
    message(STATUS "An example ${src_file} will be installed as ${_full_destination}")
    install(FILES ${src_file} DESTINATION "${_full_destination}")
  endif()

endfunction()


add_subdirectory(lib)
add_subdirectory(python)

foreach(share_dir workflows gui forward-models)
  install(DIRECTORY "share/ert/${share_dir}" DESTINATION ${SHARE_DIR})
endforeach()
install_example("share/ert/site-config" ${SHARE_DIR})
install(PROGRAMS bin/job_dispatch.py DESTINATION bin )

foreach (script ecl100 ecl300 flow rms)
  install(PROGRAMS "share/ert/forward-models/res/script/${script}" DESTINATION "${SHARE_DIR}/forward-models/res/script")
endforeach()

foreach (script careful_copy_file copy_directory copy_file delete_directory delete_file make_directory move_file symlink)
  install(PROGRAMS "share/ert/forward-models/shell/script/${script}" DESTINATION "${SHARE_DIR}/forward-models/shell/script")
endforeach()

foreach (script template_render)
    install(PROGRAMS "share/ert/forward-models/templating/script/${script}" DESTINATION "${SHARE_DIR}/forward-models/templating/script")
endforeach()

install(EXPORT res-config
        DESTINATION share/cmake/res
        NAMESPACE res::)
export(TARGETS res
        NAMESPACE res::
        FILE resConfig.cmake)
export(PACKAGE res)
