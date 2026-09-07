cmake_minimum_required(VERSION 3.16)
if(NOT TANMATSU_PLUGIN_SDK)
  message(FATAL_ERROR "Set TANMATSU_PLUGIN_SDK to the launcher's tools/plugin-sdk directory")
endif()
set(CMAKE_TOOLCHAIN_FILE "${TANMATSU_PLUGIN_SDK}/toolchain-plugin.cmake")
project(weather C)
include(${TANMATSU_PLUGIN_SDK}/plugin-build.cmake)
build_tanmatsu_plugin(weather "src/main.c;src/weather.c;src/json.c")
# The SDK helper only depends on the object-library target; also track the
# object files so edits reliably relink the .plugin on incremental builds.
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/weather.plugin"
  APPEND DEPENDS $<TARGET_OBJECTS:weather_obj>)
