set(PLUGIN "windows")
project(${PLUGIN})

find_package(KF5WindowSystem REQUIRED)
find_package(Qt5X11Extras REQUIRED)

set(SRCS
    windows.h
    windows.cpp
    windowswidget.h
    windowswidget.cpp
    ../src/applicationinfo.h
    ../src/applicationinfo.cpp
)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN} SHARED ${SRCS})

target_link_libraries(${PLUGIN} PRIVATE
    ${Qt5Widgets_LIBRARIES}
    KF5::WindowSystem
    Qt5::X11Extras
)
target_include_directories(${PLUGIN} PUBLIC ../interfaces)

install(TARGETS ${PLUGIN} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/Resources/plugins)
