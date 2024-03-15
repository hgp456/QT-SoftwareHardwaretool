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
include_directories(C:/Boost/include/boost-1\_83)

find_package(Qt6 COMPONENTS Svg REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools SvgWidgets)
find_package(PythonLibs REQUIRED)
find_package(Python COMPONENTS NumPy REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
#find_package(Boost  REQUIRED#[[
       #      COMPONENTS system filesystem thread)]]



set(TS_FILES test_en_US.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        taspa.cpp
        taspa.h
        tasparesult.cpp
        tasparesult.h
        tasparesult.ui
        muppa.cpp
        muppa.h
        mupparesult.ui
        mupparesult.cpp
        mupparesult.h
        mmm.cpp
        mmm.h
        mmmresult.ui
        mmmresult.cpp
        mmmresult.h
        matplotlibcpp.h
        ganntplot.cpp
        ganntplot.h
        kl.cpp
        kl.h
        klresult.ui
        klresult.cpp
        klresult.h
        haspa.h
        haspa.cpp
        gchs_hspa.cpp
        gchs_hspa.h
        haspa_hg.cpp
        haspa_hg.h
        haspa_hga.cpp
        haspa_hga.h
        haspa_ahg.cpp
        haspa_ahg.h
        haspa_criticalpath.cpp
        haspa_criticalpath.h
        eshc.cpp
        eshc.h
        hspa_ga.cpp
        hspa_ga.h
        hasparesult.cpp
        hasparesult.ui
        hasparesult.h
        haspacpresult.cpp
        haspacpresult.h
        haspacpresult.ui
        haspagaresult.cpp
        haspagaresult.h
        haspagaresult.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
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
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(test
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

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
