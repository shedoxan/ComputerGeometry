add_executable(Viewer2d
  main.cpp
  mainwindow.h
  mainwindow.cpp
  mainwindow.ui
  drawwidget.cpp
  drawwidget.h
  segmentintersectionwidget.cpp
  segmentintersectionwidget.h
  precisiondrawwidget.cpp
  precisiondrawwidget.h
  precisionconverters.cpp
  precisionconverters.h
)

target_link_libraries(Viewer2d PRIVATE
    PlaneGeometry
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

