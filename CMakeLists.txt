#
# Copyright (C) 2010 - 2015 Eluna Lua Engine <http://emudevs.com/>
# This program is free software licensed under GPL version 3
# Please see the included DOCS/LICENSE.md for more information
#

# Iterates through all the source files and adds them to the solution
file(GLOB sources_elunadir *.cpp *.h)

# Add files to their own groups
file(GLOB methods methods/*.cpp methods/*.h)
source_group("Eluna methods" FILES ${eluna_methods})

file(GLOB hooks hooks/*.cpp hooks/*.h)
source_group("Eluna hooks" FILES ${eluna_hooks})

set(sources_LuaEngine
  ${sources_LuaEngine}
  ${sources_elunadir}
  ${eluna_methods}
  ${eluna_hooks}
)

if( ${CMAKE_PROJECT_NAME} STREQUAL "TrinityCore" )
  find_package(Boost REQUIRED system filesystem )
  
message(STATUS "ELUNA PREPARATIONS")
#   if( WIN32 )
#     if ( MSVC )
#       add_custom_command(TARGET game
#         POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/lua_scripts/extensions/
#         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/extensions ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/lua_scripts/extensions/
#       )
#     elseif ( MINGW )
#       add_custom_command(TARGET game
#         POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/lua_scripts/extensions/
#         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/extensions ${CMAKE_BINARY_DIR}/bin/lua_scripts/extensions/
#       )
#     endif()
#   endif()
#   
#   if( UNIX )
#     install(DIRECTORY extensions DESTINATION bin/lua_scripts/)
#   elseif( WIN32 )
#     install(DIRECTORY extensions DESTINATION "${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/lua_scripts/")
#   endif()
  

elseif( ${CMAKE_PROJECT_NAME} STREQUAL "MaNGOS" )
  include_directories(
    ${CMAKE_SOURCE_DIR}/dep/g3dlite
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Common
    ${CMAKE_SOURCE_DIR}/src/shared/Utilities
    ${CMAKE_SOURCE_DIR}/src/shared/Log
    ${CMAKE_SOURCE_DIR}/src/shared/DataStores
    ${CMAKE_SOURCE_DIR}/src/shared/Threading
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/src/framework/Platform
    ${CMAKE_SOURCE_DIR}/src/game/BattleGround
    ${CMAKE_SOURCE_DIR}/src/game/Server
    ${CMAKE_SOURCE_DIR}/src/game/vmap
    ${CMAKE_SOURCE_DIR}/src/game/Maps
    ${CMAKE_SOURCE_DIR}/src/game/MotionGenerators
    ${CMAKE_SOURCE_DIR}/src/game/Tools
    ${CMAKE_SOURCE_DIR}/src/game/References
    ${CMAKE_SOURCE_DIR}/src/game/WorldHandlers
    ${CMAKE_SOURCE_DIR}/src/game/Object
  )
  
  if( WIN32 )
    if ( MSVC )
      add_custom_command(TARGET LuaEngine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/lua_scripts/extensions/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/extensions" "${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/lua_scripts/extensions/"
      )
    elseif ( MINGW )
      add_custom_command(TARGET LuaEngine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/lua_scripts/extensions/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/extensions" "${CMAKE_BINARY_DIR}/bin/ua_scripts/extensions/"
      )
    endif()
  endif()
  
  install(DIRECTORY extensions DESTINATION "${BIN_DIR}/lua_scripts/")
  
endif()
