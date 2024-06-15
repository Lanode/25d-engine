cmake_minimum_required(VERSION 3.16)
project(25d-engine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(cmake/Dependencies.cmake)

add_executable(25d-engine main.cpp
    engine.h           engine.cpp
    renderer.h         renderer.cpp
    debug.h            debug.cpp
    scene.h            scene.cpp
    resources.h        resources.cpp
    entities/player.h  entities/player.cpp
    entities/entity.h  entities/entity.cpp
    math/constants.h
    math/functions.h   math/functions.cpp
    math/transform.h   math/transform.cpp
    math/math.h
)
set_property(TARGET 25d-engine PROPERTY CXX_STANDARD 20)
target_include_directories(25d-engine PUBLIC entities/)
target_compile_features(25d-engine PRIVATE cxx_std_17)
target_link_libraries(25d-engine PRIVATE sfml-graphics)
target_link_libraries(25d-engine PRIVATE physfs-static)
target_link_libraries(25d-engine PRIVATE glm)

if(WIN32)
    add_custom_command(
        TARGET 25d-engine
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:25d-engine>
        VERBATIM)
endif()

# Copying of engine assets
add_custom_command(
    TARGET 25d-engine
    COMMENT "Copying engine assets"
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:25d-engine>
    VERBATIM)
# file(GLOB ENGINE_ASSETS ${CMAKE_SOURCE_DIR}/assets/*)
# foreach(CurrentAssetFile IN LISTS ENGINE_ASSETS)
#    add_custom_command(
#             TARGET 25d-engine
#             COMMENT "Copy engine asset: ${CurrentAssetFile}"
#             PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CurrentAssetFile} $<TARGET_FILE_DIR:25d-engine>
#             VERBATIM)
# endforeach()

install(TARGETS 25d-engine)
