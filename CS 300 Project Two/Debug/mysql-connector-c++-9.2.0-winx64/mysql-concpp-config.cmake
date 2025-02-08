# Copyright (c) 2023, 2024, Oracle and/or its affiliates.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 2.0, as
# published by the Free Software Foundation.
#
# This program is designed to work with certain software (including
# but not limited to OpenSSL) that is licensed under separate terms, as
# designated in a particular file or component or in included license
# documentation. The authors of MySQL hereby grant you an additional
# permission to link the program and your derivative works with the
# separately licensed software that they have either included with
# the program or referenced in the documentation.
#
# Without limiting anything contained in the foregoing, this file,
# which is part of Connector/C++, is also subject to the
# Universal FOSS Exception, version 1.0, a copy of which can be found at
# https://oss.oracle.com/licenses/universal-foss-exception.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License, version 2.0, for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

#
# Cmake configuration file for MySQL Connector/C++ package `mysql-concpp`
# =======================================================================
#
# See also: https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html
#
# The following import targets are created by the package. All these targets
# are in the `mysql::` namespace.
#
# - concpp-xdevapi, concpp    -- the XDevAPI variant of the library
# - concpp-xdevapi-static, concpp-static -- static variant of the above
# - concpp-jdbc               -- the JDBC variant of the library
# - concpp-jdbc-static        -- static variant of the above
#
# Targets `mysql::concpp` and `mysql::concpp-static` are aliases for
# `mysql::concpp-xdevapi` and `mysql::concpp-xdevapi-static`, respectively.
#
# Also, the following variables are set:
#
# - Xxx_FOUND
# - Xxx_JDBC_FOUND, Xxx_jdbc_FOUND -- (1a)
# - Xxx_DEBUG_FOUND, Xxx_debug_FOUND -- (1b)
# - Xxx_RELEASE_FOUND, Xxx_release_FOUND -- (1b)
# - Xxx_VERSION, Xxx_VERSION_CC
# - Xxx_ROOT_DIR -- (2)
# - Xxx_RUNTIME_LIBRARY_DIRS, Xxx_RUNTIME_LIBRARY_DIR -- (2,3)
# - Xxx_RUNTIME_LIBRARY_DIRS_DEBUG, Xxx_RUNTIME_LIBRARY_DIR_DEBUG -- (4a)
# - Xxx_RUNTIME_LIBRARY_DIRS_RELEASE, Xxx_RUNTIME_LIBRARY_DIR_RELEASE -- (4b)
# - Xxx_PLUGIN_DIR -- (5a,5b)
#
# In these variable names Xxx is either `MYSQL_CONCPP` or `mysql-concpp`,
# CC is version component: one of `MAJOR`, `MINOR` or `PATCH`.
#
# Note (1a): Set to true if the classic JDBC connector libraries were found and
# the -jdbc targets are defined. It must be the case if `REQUIRE jdbc` clause
# was used in the cmake `find_package()` command.
#
# Note (1b): These are set to true if debug/release libraries are available
# (see below).
#
# Note (2): Set only in case of a monolithic install (TGZ, ZIP, MSI).
#
# Note (3): Application that links to shared connector libraries must find
# these libraries at runtime. Depending on the platform this is achieved by
# configuring RPATH in the executable, editing environment variables
# or copying shared libraries to the location of the executable. In either
# case the _RUNTIME_LIBRARY_DIR(S) variable gives the location where shared
# connector libraries can be found.
#
# Note (4a): If debug variants of connector libraries were found these
# variables are set to their location.
#
# Note (4b): For consistency, these variables are set if release variants
# of connector libraries were found and in that case they are equal
# to _RUNTIME_LIBRARY_DIR(S) ones.
#
# Note (5a): The JDBC connector might require loading of authentication plugins
# at connection time (depending on authentication mechanism being used). These
# plugins are in the location given by _PLUGIN_DIR variable. Depending on
# the installation type it might be necessary to specify this location
# with connection configuration options for plugins to be correctly found
# at runtime (see: https://dev.mysql.com/doc/connector-cpp/8.2/en/connector-cpp-authentication.html)
#
# Note (5b): Authentication plugins are bundled with the connector and
# the _PLUGIN_DIR variable is set only when the JDBC connector links the MySQL
# client library statically (which is the typical case). It is also possible
# to build JDBC connector with dynamic linking to the MySQL client library.
# In that case plugins are not bundled with the connector and _PLUGIN_DIR
# variable is not set -- if needed the plugins that come with the MySQL client
# library should be used in that case.
#
# Note: The variables are put in the cache but if Xxx_FOUND is not set or
# is false then the module will be re-loaded and the other variables in
# the cache will be overwritten with newly detected values.
#
# Note: If mysql-concpp_FIND_VERBOSE is true when loading package diagnostic
# messages will be printed by this script.
#
#
# OpenSSL dependency
# ------------------
#
# Connector/C++ requires OpenSSL libraries. Depending on the platform and
# the installation type it is either expected that OpenSSL will be installed
# on the system or these libraries are bundled in the connector package.
# Connector library targets are configured to use OpenSSL from appropriate
# locations. This can be overridden by user -- if `mysql::openssl` target
# is defined prior to loading `mysql-concpp` package then this target is used
# to resolve dependency on the OpenSSL library.
#
# MySQL client library dependency
# -------------------------------
#
# If JDBC connector library is built with the MySQL client library linked
# dynamically then the `concpp-jdbc-static` target depends on `libmysqlclient`
# which should be available on the build host. If it is installed
# in non-standard location then config variable WITH_MYSQL should be set
# to point at the MySQL install location. The library search path will
# be augmented to find the client library installed at that location.
#
# Note that connector libraries published by MySQL have the client library
# statically linked in so that there is no external dependency on it.
#
# Debug libraries
# ---------------
#
# When linking with static Connector/C++ library on Windows and building
# in debug mode the library built in debug mode is needed. Such debug builds
# of Connector/C++ are distributed as a separate package to be installed
# on top of the regular one. If debug libraries are available this script
# will detect them and configure mysql:: import targets so that they use
# debug variants of the library for builds in debug mode. The presence
# of debug libraries is indicated by setting _DEBUG_FOUND variable to true.
# It is also possible to request "debug" component using `REQUIRE debug`
# clause of `find_package()` command. If this is done then `find_package()`
# will fail if debug libraries were not found.
#
# When using custom builds of Connector/C++ it is possible to have
# an installation with only debug libraries. On Windows, in such situation,
# the connector targets created here will work only for debug builds.
# On non-Windows platforms debug libraries can and will be used for building
# in any mode. Presence of release libraries is indicated by _RELEASE_FOUND
# variable. One can use `REQUIRE release` clause of `find_package()` command
# to ensure that release variants of the libraries are present.
#
# Note: Debug libraries are needed and used only on Windows. For Linux
# separate packages with debug symbols are available that can be used
# to debug connector libraries but only release builds of these libraries
# are distributed.
#
# Note: When only debug libraries are available the _RUNTIME_LIBRARY_DIR(S)
# variables point at the location of these debug libraries on non-Windows
# platforms (because they are used by other build types). However, on Windows
# the _RUNTIME_LIBRARY_DIR(S) variables still point to the location where
# release variants of the libraries should be installed (but are not present).
# The libraries can be located using _RUNTIME_LIBRARY_DIR(S)_DEBUG variables
# in such scenario.

# message(STATUS "mysql-concpp module config (${MYSQL_CONCPP_FOUND}, ${mysql-concpp_FOUND})")

if(mysql-concpp_FOUND)
  return()
endif()

function(message_info)
  if(NOT mysql-concpp_FIND_VERBOSE OR mysql-concpp_FIND_QUIETLY)
    return()
  endif()
  message(STATUS "mysql-concpp: " ${ARGV})
endfunction()

function(set_warning)
  set(warning_message ${ARGV} CACHE INTERNAL "warning message")
endfunction()


#
# JDBC_MYSQL_DEP tells whether JDBC connector library depends on the MySQL
# client library. This is not the case when the connector library links
# the client library statically. However, if WITH_MYSQL option is defined
# and not false then JDBC targets are always configured to depend on the client
# library. Additionally, if WITH_MYSQL is a path pointing at MySQL install
# location, the library path will be extended so that linker looks for the
# client library at that location.
#

set(JDBC_MYSQL_DEP OFF)
if(WITH_MYSQL OR NOT 1)
  set(JDBC_MYSQL_DEP ON)
endif()


macro(main)

  # We can have two types of installations -- in case of RPM/DEB install
  # connector files are installed to system-wide locations (system-wide
  # install), in other cases all connector files are installed into a single
  # root directory (monolithic install).

  if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/include/mysqlx/xdevapi.h")

    # Case of monolithic install

    set(monolithic 1)

    set(MYSQL_CONCPP_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
    message_info("Module installed at: ${MYSQL_CONCPP_ROOT_DIR}")

    set(INCLUDE_DIR "${MYSQL_CONCPP_ROOT_DIR}/include")
    set(LIBRARY_DIR "${MYSQL_CONCPP_ROOT_DIR}/lib64")

    set(fail_message "Could NOT find MySQL Connector/C++ libraries at ${MYSQL_CONCPP_ROOT_DIR}.")

  else()

    # System-wide install, DEB or RPM layout.

    set(monolithic 0)

    set(INCLUDE_DIR "/usr/include/mysql-cppconn")

    # We do not set LIBRARY_DIR because in this case we will search for the libraries in system-wide locations.

    set(fail_message
      "Could NOT find MySQL Connector/C++ libraries at system-wide locations."
    )

  endif()


  # Headers must always be found first.

  find_includes()

  if(fail_message_includes)
    set_not_found(
      "Could NOT find MySQL Connector/C++ headers (${fail_message_includes})."
    )
    # Note: Error out early.
    message(FATAL_ERROR ${mysql-concpp_NOT_FOUND_MESSAGE})
  endif()

  message_info("Include path: ${MYSQL_CONCPP_INCLUDE_DIR}")

  # This suffix is used to locate static and import libraries on Windows

  set(vs_suffix)
  if(WIN32)
    set(vs_suffix vs14)
  endif()

  # Find required dependencies. Currently this looks for OpenSSL and defines
  # `mysql::openssl` interface library for it if found.

  find_deps()

  # Find connector libraries and define interface targets for the ones that
  # were found. This will set/update LIBRARY_DIR to the location where
  # libraries were found and will also set RELEASE/DEBUG_FOUND flags
  # if the corresponding variant of the libraries was found.

  set(DEBUG_FOUND 0)
  set(RELEASE_FOUND 0)

  find_connector(XDevAPI)
  find_connector(JDBC)


  set(MYSQL_CONCPP_DEBUG_FOUND ${DEBUG_FOUND})
  set(MYSQL_CONCPP_RELEASE_FOUND ${RELEASE_FOUND})

  if(monolithic)

    # Set MYSQL_CONCPP_RUNTIME_LIBRARY_DIR_*. Variables based on LIBRARY_DIR
    # determined above.

    if(DEBUG_FOUND)

      set(MYSQL_CONCPP_RUNTIME_LIBRARY_DIR_DEBUG "${LIBRARY_DIR}/debug")

    elseif(WIN32)

      set_warning(
        "Debug variants of connector libraries were not found"
        "at the install location -- building in debug mode will not work"
      )

    endif()


    if(RELEASE_FOUND)

      set(MYSQL_CONCPP_RUNTIME_LIBRARY_DIR_RELEASE "${LIBRARY_DIR}")
      set(MYSQL_CONCPP_RUNTIME_LIBRARY_DIR "${LIBRARY_DIR}")

    elseif(NOT WIN32)

      # If release libraries were not found and we are on non-Win platform we
      # will use debug libraries also for release builds.

      message_info(
        "Using debug variants of connector libraries for release builds because"
        " release variants are not found at the install location"
      )

      set(
        MYSQL_CONCPP_RUNTIME_LIBRARY_DIR
        "${MYSQL_CONCPP_RUNTIME_LIBRARY_DIR_DEBUG}"
      )

    else()

      # On Windows one can not mix release and debug code. The targets created
      # above have only debug-mode paths defined. We set
      # MYSQL_CONCPP_RUNTIME_LIBRARY_DIR to the path where release libraries
      # would be expected even if they were not found there.

      set_warning(
        "Release variants of connector libraries were not found"
        "at the install location -- building in release mode will not work"
      )

      set(MYSQL_CONCPP_RUNTIME_LIBRARY_DIR "${LIBRARY_DIR}")

    endif()

    # Note: Set plugin dir location only if connector has client library
    # statically linked in (does not depend on external one).

    if(NOT JDBC_MYSQL_DEP)
      set(MYSQL_CONCPP_PLUGIN_DIR "${LIBRARY_DIR}/plugin")
    endif()

  else()

    # Note: In system-wide install case we do not set _RUNTIME_LIBRARY_DIR_*
    # variables as libraries are installed at system-wide locations.

    if(NOT JDBC_MYSQL_DEP)
      set(MYSQL_CONCPP_PLUGIN_DIR
        "${LIBRARY_DIR}/mysql/libmysqlcppconn10/plugin"
      )
    endif()

  endif()


  # Aliases for -xdevapi* targets.

  foreach(suffix "" "-static" "-debug" "-static-debug")

    if(TARGET mysql::concpp-xdevapi${suffix})
      set(MYSQL_CONCPP_FOUND 1)
      add_library(mysql::concpp${suffix} ALIAS mysql::concpp-xdevapi${suffix})
    endif()

    if(TARGET mysql::concpp-jdbc${suffix})
      set(MYSQL_CONCPP_JDBC_FOUND 1)
    endif()

endforeach()


  # Build the NOT_FOUND message.
  # Note: The different find_xxx() functions set the specific part
  # of the message, such as ${fail_message_devapi}, in case of failure.

  if(fail_message_devapi) # AND MYSQL_CONCPP_FIND_REQUIRED_devapi)
    list(APPEND fail_message ${fail_message_devapi})
  elseif(fail_message_jdbc) # AND MYSQL_CONCPP_FIND_REQUIRED_jdbc)
    list(APPEND fail_message ${fail_message_jdbc})
  endif()

  set_not_found(${fail_message})

  # Build the success message which can optionally contain warnings
  # TODO: Warnings about missing debug/release library variants

  set(MYSQL_CONCPP_FOUND_MSG ${MYSQL_CONCPP_INCLUDE_DIR})
  if(warning_message)
    string(JOIN " " warning_message ${warning_message})
    set(MYSQL_CONCPP_FOUND_MSG
      "${MYSQL_CONCPP_FOUND_MSG} WARNING: ${warning_message}"
    )
  endif()

  include(FindPackageHandleStandardArgs)

  # Note: The _FOUND variable name expected by FPHSA for component CCC
  # is mysql-concpp_CCC_FOUND

  set(mysql-concpp_jdbc_FOUND ${MYSQL_CONCPP_JDBC_FOUND})
  set(mysql-concpp_debug_FOUND ${MYSQL_CONCPP_DEBUG_FOUND})
  set(mysql-concpp_release_FOUND ${MYSQL_CONCPP_RELEASE_FOUND})

  find_package_handle_standard_args(mysql-concpp
    REQUIRED_VARS
      MYSQL_CONCPP_FOUND_MSG
      MYSQL_CONCPP_INCLUDE_DIR
      MYSQL_CONCPP_FOUND
    VERSION_VAR mysql-concpp_VERSION
    HANDLE_COMPONENTS
    FAIL_MESSAGE "${mysql-concpp_NOT_FOUND_MESSAGE}"
  )

  # Set alternative variables

  set(MYSQL_CONCPP_jdbc_FOUND ${MYSQL_CONCPP_JDBC_FOUND})
  set(MYSQL_CONCPP_debug_FOUND ${MYSQL_CONCPP_DEBUG_FOUND})
  set(MYSQL_CONCPP_release_FOUND ${MYSQL_CONCPP_RELEASE_FOUND})

  foreach(var
    ROOT_DIR PLUGIN_DIR
    RUNTIME_LIBRARY_DIR RUNTIME_LIBRARY_DIRS
    RUNTIME_LIBRARY_DIR_DEBUG RUNTIME_LIBRARY_DIRS_DEBUG
    RUNTIME_LIBRARY_DIR_RELEASE RUNTIME_LIBRARY_DIRS_RELEASE
    DEBUG_FOUND debug_FOUND RELEASE_FOUND release_FOUND
    JDBC_FOUND jdbc_FOUND
  )

    if(NOT DEFINED MYSQL_CONCPP_${var})
      continue()
    endif()

    # handle _DIR_ and _DIRS_ variants

    if(var STREQUAL "RUNTIME_LIBRARY_DIR")
      foreach(suffix "" "_RELEASE" "_DEBUG")
        if(NOT DEFINED MYSQL_CONCPP_RUNTIME_LIBRARY_DIR${suffix})
          continue()
        endif()
        set(
          MYSQL_CONCPP_RUNTIME_LIBRARY_DIRS${suffix}
          "${MYSQL_CONCPP_RUNTIME_LIBRARY_DIR${suffix}}"
        )
      endforeach()
    endif()

    set(mysql-concpp_${var} ${MYSQL_CONCPP_${var}} CACHE INTERNAL "mysql-concpp module config variable" FORCE)
    set(MYSQL_CONCPP_${var} ${MYSQL_CONCPP_${var}} CACHE INTERNAL "mysql-concpp module config variable" FORCE)


  endforeach(var)

  foreach(ver "" _MAJOR _MINOR _PATCH _TWEAK _COUNT)

    if(NOT DEFINED mysql-concpp_VERSION${ver})
      continue()
    endif()

    set(mysql-concpp_VERSION${ver} ${mysql-concpp_VERSION${ver}} CACHE INTERNAL "mysql-concpp module config variable" FORCE)
    set(MYSQL_CONCPP_VERSION${ver} ${mysql-concpp_VERSION${ver}} CACHE INTERNAL "mysql-concpp module config variable" FORCE)

  endforeach(ver)

endmacro(main)


# Set the _NOT_FOUND message for the module from a list of strings passed
# to the function.

function(set_not_found)

  list(JOIN ARGN " " message)
  set(mysql-concpp_NOT_FOUND_MESSAGE "${message}" PARENT_SCOPE)

endfunction()


function(find_includes)

  #message(STATUS "Looking for headers at: ${INCLUDE_DIR}")

  find_path(MYSQL_CONCPP_INCLUDE_DIR
    NAMES mysqlx/xdevapi.h
    PATHS ${INCLUDE_DIR}
    NO_DEFAULT_PATH
    NO_CACHE
  )

  if(NOT MYSQL_CONCPP_INCLUDE_DIR)

    if(MYSQL_CONCPP_ROOT_DIR)
      set(fail_message_includes
        "at MYSQL_CONCPP_ROOT_DIR: ${INCLUDE_DIR}"
      )
    else()
      set(fail_message_includes "at ${INCLUDE_DIR}")
    endif()

    set_parent(fail_message_includes)
    return()

  endif()

  set_parent(MYSQL_CONCPP_INCLUDE_DIR)

endfunction()


# Find XDevAPI or JDBC connector libraries, as specified by parameter `which`
# and create interface library targets for them. Both shared and static
# variants are searched for. If some libraries are not found the corresponding
# targets are not created.
#
# If LIBRARY_DIR is set the libraries are searched in that location, otherwise
# they are searched in system-wide locations. In either case LIBRARY_DIR is
# set/updated to the location where libraries were found.
#
# Flags RELEASE_FOUND and DEBUG_FOUND are set if the corresponding variants
# of the libraries were found.

function(find_connector which)

  if(which STREQUAL "JDBC")
    set(base_name "mysqlcppconn")
    set(target_name "jdbc")
  else()
    set(base_name "mysqlcppconnx")
    set(target_name "xdevapi")
  endif()


  # Look for the connector library and if found create the import target for
  # it. Sets ${target_name}_RELEASE and ${target_name}_DEBUG to indicate
  # whether release/debug variant of the library was found. Also sets
  # or updates LIBRARY_DIR to the location where the library was found.

  add_connector_target(${which} ${target_name} ${base_name})

  # Note: if the mysql::openssl target is not defined then we are on Windows
  # and no suitable OpenSSL was found. In that case static libraries will
  # not work and we do not define these targets.

  if(TARGET mysql::openssl)
    add_connector_target(${which} ${target_name}-static ${base_name}-static)
  endif()

  # Process targets created above to do consistency checks and declare required
  # dependencies. Also sets DEBUG/RELEASE_FOUND flags as needed.

  foreach(tgt ${target_name} ${target_name}-static)

    if(${tgt}_RELEASE)
      set_parent(RELEASE_FOUND 1)
    endif()

    if(${tgt}_DEBUG)
      set_parent(DEBUG_FOUND 1)
    endif()

    if(NOT TARGET mysql::concpp-${tgt})
      continue()
    endif()

    if(DEBUG_FOUND AND NOT ${tgt}_DEBUG)
      list(APPEND DEBUG_MISSING ${tgt})
    endif()
    set_parent(DEBUG_MISSING)

    if(RELEASE_FOUND AND NOT ${tgt}_RELEASE)
      list(APPEND RELEASE_MISSING ${tgt})
    endif()
    set_parent(RELEASE_MISSING)

    unset(libs)

    # JDBC dependency on the client library

    if(tgt MATCHES "jdbc" AND JDBC_MYSQL_DEP)

      if(NOT WIN32)
        list(APPEND libs mysqlclient)
      endif()

      if(DEFINED WITH_MYSQL AND EXISTS "${WITH_MYSQL}/lib")

        message_info("Client library path: ${WITH_MYSQL}/lib")

        target_link_directories(mysql::concpp-${tgt}
          INTERFACE "${WITH_MYSQL}/lib"
        )

      endif()

    endif()

    # OpenSSL dependency (target `mysql::openssl` is defined by find_deps())
    #
    # Note: Even though JDBC connector does not use OpenSSL directly it might
    # have the client library statically linked in and get dependency
    # on OpenSSL that way.

    if(TARGET mysql::openssl)
      list(APPEND libs mysql::openssl)
    endif()

    if(tgt MATCHES "-static")

      set_target_properties(mysql::concpp-${tgt} PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES CXX
        INTERFACE_COMPILE_DEFINITIONS STATIC_CONCPP
      )

      # Handle additional dependencies required for static library.

      if(WIN32)

        list(APPEND libs Dnsapi)

      else()

        if(NOT APPLE)
          list(APPEND libs pthread)
        endif()

        # On Solaris we additionally need couple more libs.

        if(CMAKE_SYSTEM_NAME MATCHES "SunOS")
          list(APPEND libs socket nsl)
        endif()

        if(NOT CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
          list(APPEND libs resolv dl)
        endif()

      endif()

    endif()

    target_link_libraries(mysql::concpp-${tgt} INTERFACE ${libs})

    if(libs)
      string(JOIN " " libs ${libs})
      message_info("Link libraries for target ${tgt}: ${libs}")
    endif()

  endforeach(tgt)

endfunction(find_connector)


# Create connector library import target named ${tgt} pointing at library
# with base name ${base_name} if it was found.
#
# If LIBRARY_DIR is set the library is searched in that location, otherwise it
# is searched in system-wide locations. If the library is found, LIBRARY_DIR
# is set/updated to the location where it was found.
#
# Both release and debug variants of the library are searched for. Flags
# ${tgt}_RELEASE and ${tgt}_DEBUG are set to tell which variant was found.
#
# Note: The which parameter, either "XDevAPI" or "JDBC", is used for
# diagnostics only.

function(add_connector_target which tgt base_name)

  set(lib_name "${base_name}")
  set(type "SHARED")
  set(static 0)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})

  if(tgt MATCHES "-static")
    set(type "STATIC")
    set(static 1)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
  endif()

  set(XDevAPI_abi 2)
  set(JDBC_abi 10)
  set(find_lib_paths)
  set(win_opts)

  if(LIBRARY_DIR)
    set(find_lib_paths PATHS "${LIBRARY_DIR}" NO_DEFAULT_PATH)
  endif()

  if(WIN32)
    if(static)
      set(win_opts PATH_SUFFIXES ${vs_suffix})
    else()
      set(lib_name "${base_name}-${${which}_abi}-vs14")
    endif()
  endif()

  #message("!!! looking for ${lib_name} with options: ${find_lib_opts}")

  unset(lib_path CACHE)
  find_library(lib_path
    NAMES ${lib_name}
    ${find_lib_paths} ${win_opts}
    NO_CACHE
  )

  if(lib_path)

    message_info(
      "Found ${which} ${type} library at: ${lib_path}"
    )

    set_parent(${tgt}_RELEASE 1)

    # Note: LIBRARY_DIR is not yet set here if we were looking for the library
    # in system-wide locations. In any case we set/update LIBRARY_DIR to
    # the actual location of the library that was found.

    get_filename_component(LIBRARY_DIR "${lib_path}" DIRECTORY)
    set_parent(LIBRARY_DIR)

    if(WIN32 AND NOT static)
      find_imp_lib(imp_lib_path ${base_name} "${lib_path}")
    endif()

  endif()

  # Look for debug variant of the library if LIBRARY_DIR is set. This is
  # the case in one of these situations:
  #
  # a) the release library was found at LIBRARY_DIR above;
  #
  # b) we have monolithic connector install and LIBRARY_DIR is the library
  #    location inside that monolithic install.
  #
  # Case (b) is if we have not found the release library inside a monolithic
  # connector installation but we still can find a debug library there.

  if(LIBRARY_DIR)

    unset(lib_path_debug CACHE)
    find_library(lib_path_debug
      NAMES ${lib_name}
      PATHS "${LIBRARY_DIR}/debug"
      ${win_opts}
      NO_DEFAULT_PATH
      NO_CACHE
    )

    if(lib_path_debug)

      message_info(
        "Found debug variant of ${which} ${type} library at: ${lib_path_debug}"
      )

      set_parent(${tgt}_DEBUG 1)

      if(WIN32 AND NOT static)
        find_imp_lib(imp_lib_path_debug ${base_name} "${lib_path_debug}")
      endif()

    endif()

  endif()


  if(NOT lib_path AND NOT lib_path_debug)

    message_info("Did not find ${which} ${type} library")
    return()

  endif()

  #
  # Note: At this point we know that either the release or the debug connector
  # was found.
  #
  # On non-Windows platforms the release connector can be used for debug
  # builds if the debug connector was not found and vice-versa, the debug
  # connector can be used for release builds if the release connector was
  # not found.
  #
  # However, on Windows it is not possible to mix debug and release code which
  # is reflected in the logic below.
  #

  if(WIN32 AND NOT lib_path_debug)

    # If debug connector was not found on Windows we still set debug path to its
    # expected location so that:
    #
    # 1. If debug connector is added later it will be used in debug builds.
    #
    # 2. If debug connector is not present then debug builds will fail which
    # is what we want in that case (rather than incorrectly using the release
    # variant of the library).
    #
    # Note: LIBRARY_DIR must be defined because the release library must have
    # been found above.

    set(lib_path_debug
      "${LIBRARY_DIR}/debug/${lib_name}.dll"
    )
    set(imp_lib_path_debug
      "${LIBRARY_DIR}/debug/${vs_suffix}/${base_name}.lib"
    )

    # Note: If debug connector was not found on non-Windows platform then
    # lib_path_debug remains undefined which means that the import target will
    # not have a _DEBUG location defined. Therefore the main (release) location
    # will be used also in debug builds as we want in that case.

  elseif(WIN32 AND NOT lib_path)

    # If release connector was not found on Windows we still set the release
    # path to its expected location so that release builds will fail (because
    # the release library will be not found at the location) or, if release
    # connector is added later release builds will start working.

    set(lib_path "${LIBRARY_DIR}/${lib_name}.dll")
    set(imp_lib_path "${LIBRARY_DIR}/${vs_suffix}/${base_name}.lib")

  elseif(NOT WIN32 AND NOT lib_path)

    # If we are on non-Windows platform and the release connector was not found
    # then use the debug connector as a replacement.

    set(lib_path "${lib_path_debug}")

  endif()


  set(tgt "concpp-${tgt}")
  #message(STATUS "Creating target: mysql::${tgt}")

  add_library(mysql::${tgt} ${type} IMPORTED GLOBAL)
  target_compile_features(mysql::${tgt} INTERFACE cxx_std_11)

  set_target_properties(mysql::${tgt} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${MYSQL_CONCPP_INCLUDE_DIR}"
  )


  if(lib_path)

    set_target_properties(mysql::${tgt} PROPERTIES
      IMPORTED_LOCATION "${lib_path}"
    )

    if(WIN32 AND imp_lib_path)
      set_target_properties(mysql::${tgt} PROPERTIES
        IMPORTED_IMPLIB "${imp_lib_path}"
      )
    endif()

  endif()

  if(lib_path_debug)

    set_target_properties(mysql::${tgt} PROPERTIES
      IMPORTED_LOCATION_DEBUG "${lib_path_debug}"
    )

    if(WIN32 AND imp_lib_path_debug)
      set_target_properties(mysql::${tgt} PROPERTIES
        IMPORTED_IMPLIB_DEBUG "${imp_lib_path_debug}"
      )
    endif()

  endif()

endfunction(add_connector_target)


# On Windows find import library for the DLL library at the given `path` with
# given `base_name`. The location of the import library is stored in variable
# named by `var`.
#
# Note: Not finding an import library for a DLL is a fatal error.

function(find_imp_lib var base_name path)

  get_filename_component(base_path ${path} DIRECTORY)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .lib)

  #message("!!! Looking for import library for: ${path}")
  find_library(${var}
    NAMES ${base_name}
    PATHS ${base_path}
    PATH_SUFFIXES ${vs_suffix}
    NO_DEFAULT_PATH
    NO_CACHE
  )

  if(NOT ${var})
    message(FATAL_ERROR "Could not find import library for ${path}")
  endif()

  set_parent(${var})

endfunction(find_imp_lib)


function(find_deps)

  if(TARGET mysql::openssl)
    message_info(
      "Using custom mysql::openssl target to resolve dependency on OpenSSL"
    )
    return()
  endif()

  unset(ssl_lib)

  if(MYSQL_CONCPP_ROOT_DIR)

    # Try to find the bundled OpenSSL
    # Note: On Windows we look for the import library with .lib extension.

    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".lib" ".dylib")

    unset(ssl_lib CACHE)
    find_library(ssl_lib
      NAMES ssl libssl
      PATHS ${LIB_PATH}
      PATH_SUFFIXES private ${vs_suffix}
      NO_DEFAULT_PATH
      NO_CACHE
    )

    unset(ssl_crypto CACHE)
    find_library(ssl_crypto
      NAMES crypto libcrypto
      PATHS ${LIB_PATH}
      PATH_SUFFIXES private ${vs_suffix}
      NO_DEFAULT_PATH
      NO_CACHE
    )

    if(NOT ssl_lib OR NOT ssl_crypto)
      message_info("Bundled OpenSSL was not found")
      set(ssl_lib false)
    endif()

  endif()

  # Note: For some reason interface libraries can not have names with "::"

  add_library(mysql_concpp_openssl INTERFACE)

  if(ssl_lib)

    message_info("Using bundled OpenSSL")

  elseif(WIN32)

    message(STATUS "mysql-concpp:"
      " For static linking the OpenSSL libraries are required but they are"
      " not bundled with the connector -- static connector targets will not"
      " be created. To use static connector libraries define mysql::openssl"
      " target to point at an OpenSSL installation to be used."
    )

    return()

  else()

    message_info("Using system OpenSSL libraries")

    set(ssl_lib "ssl")
    set(ssl_crypto "crypto")

  endif()

  target_link_libraries(mysql_concpp_openssl INTERFACE
    ${ssl_lib} ${ssl_crypto}
  )

  add_library(mysql::openssl ALIAS mysql_concpp_openssl)

endfunction(find_deps)


# Sets given variable in the parent scope to its current value in this scope.
# Optionally, if new value is given after variable name, variable's value
# is changed first.

macro(set_parent var)
  #message(STATUS "set_parent: ${var} (${ARGN})")
  if(${ARGC} GREATER 1)
    set(${var} "${ARGN}")
  endif()
  set(${var} "${${var}}" PARENT_SCOPE)
endmacro()


main()
