add_executable(PlaneGeometryTests
    PlaneGeometryTests.cpp
)

target_link_libraries(PlaneGeometryTests PRIVATE
    PlaneGeometry
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(
    NAME PlaneGeometryTests
    COMMAND
        ${CMAKE_COMMAND} -E env
        "PATH=$<TARGET_FILE_DIR:Qt${QT_VERSION_MAJOR}::Core>;$<TARGET_FILE_DIR:PlaneGeometryTests>;$ENV{PATH}"
        PlaneGeometryTests
)
