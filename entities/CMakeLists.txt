target_sources(25d-engine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/player.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp"
)
target_include_directories(25d-engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
