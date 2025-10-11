add_library(PlaneGeometry STATIC
    PlaneOperations.cpp
    PlaneOperations.h
)

target_link_libraries(PlaneGeometry PUBLIC Boost::boost)

target_include_directories(PlaneGeometry
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
