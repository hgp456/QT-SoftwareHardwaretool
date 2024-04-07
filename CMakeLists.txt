cmake_minimum_required(VERSION 3.5)

project(test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#set(BOOST_ROOT C:/Boost)
#set(Boost_LIBRARY_DIRS c:/Boost/lib)

link_directories(C:/Boost)

# Add Boost include directory
## add you Boost Version Here
include_directories(C:/Boost/include/boost-1\_84)

find_package(Qt6 COMPONENTS Svg REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools SvgWidgets)
find_package(PythonLibs REQUIRED)
find_package(Python COMPONENTS NumPy REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
#find_package(Boost  REQUIRED#[[
       #      COMPONENTS system filesystem thread)]]


qt6_add_resources(qrc_FILES source/res.qrc)



set(TS_FILES test_en_US.ts)

set(PROJECT_SOURCES
        source/main.cpp
        source/mainwindow.cpp
        inc/mainwindow.h
        source/mainwindow.ui
        source/taspa.cpp
        inc/taspa.h
        source/tasparesult.cpp
        inc/tasparesult.h
        source/tasparesult.ui
        source/muppa.cpp
        inc/muppa.h
        source/mupparesult.ui
        source/mupparesult.cpp
        inc/mupparesult.h
        source/mmm.cpp
        inc/mmm.h
        source/mmmresult.ui
        source/mmmresult.cpp
        inc/mmmresult.h
        inc/matplotlibcpp.h
        source/ganntplot.cpp
        inc/ganntplot.h
        source/kl.cpp
        inc/kl.h
        source/klresult.ui
        source/klresult.cpp
        inc/klresult.h
        inc/haspa.h
        source/haspa.cpp
        source/gchs_hspa.cpp
        inc/gchs_hspa.h
        source/haspa_hg.cpp
        inc/haspa_hg.h
        source/haspa_hga.cpp
        inc/haspa_hga.h
        source/haspa_ahg.cpp
        inc/haspa_ahg.h
        source/haspa_criticalpath.cpp
        inc/haspa_criticalpath.h
        source/eshc.cpp
        inc/eshc.h
        source/hspa_ga.cpp
        inc/hspa_ga.h
        source/hasparesult.cpp
        source/hasparesult.ui
        inc/hasparesult.h
        source/haspacpresult.cpp
        inc/haspacpresult.h
        source/haspacpresult.ui
        source/haspagaresult.cpp
        inc/haspagaresult.h
        source/haspagaresult.ui
        ${TS_FILES}
)

#qt6_wrap_ui(PROJECT_SOURCES
#    ui/mainwindow.ui
#    ui/haspacpresult.ui
#    ui/haspagaresult.ui
#    ui/hasparesult.ui
#    ui/klresult.ui
#    ui/mmmresult.ui
#    ui/mupparesult.ui
#    ui/tasparesult.ui
#    ui/taspa.ui
#)

## 递归地查找源文件目录下所有的.cpp文件
#file(GLOB_RECURSE PROJECT_SOURCES_CPP "${CMAKE_SOURCE_DIR}/source/*.cpp")# 递归地查找源文件目录下所有的.h文件

#file(GLOB_RECURSE PROJECT_SOURCES_H   "${CMAKE_SOURCE_DIR}/inc/*.h")

## 递归地查找源文件目录下所有的.ui文件
#file(GLOB_RECURSE PROJECT_SOURCES_UI
#    "${CMAKE_SOURCE_DIR}/ui/*.ui")# 将所有找到的文件添加到PROJECT_SOURCES变量中


#set(PROJECT_SOURCES    ${PROJECT_SOURCES_CPP}
#    ${PROJECT_SOURCES_H}    ${PROJECT_SOURCES_UI} ${TS_FILES}
#)




# 寻找并添加所有的.ts文件
#file(GLOB TS_FILES
#    "${CMAKE_SOURCE_DIR}/source/*.ts")# 为TS_FILES创建一个资源文件qt5_create_translation(QM_FILES ${TS_FILES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${qrc_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(test SHARED
            ${PROJECT_SOURCES}
            ${qrc_FILES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(test
            ${PROJECT_SOURCES}
            ${qrc_FILES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/inc")


target_link_libraries(test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt6::SvgWidgets)
target_link_libraries(test PRIVATE ${PYTHON_LIBRARIES} Python::NumPy ${Boost_LIBRARIES})


set_target_properties(test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test)
endif()
