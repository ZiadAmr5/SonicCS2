cmake_minimum_required(VERSION 3.16)

project(SonicCS2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES

       src/mainwindow.ui
        src/main.cpp
        src/player.cpp
        src/mainwindow.cpp
        include/player.h
        include/mainwindow.h
        include/GameEngine.h

        # UML scaffold classes (Mario)
        src/GameObject.cpp       include/GameObject.h
        src/Entity.cpp           include/Entity.h
        src/Physics.cpp          include/Physics.h
        src/Enemy.cpp            include/Enemy.h
        src/Coin.cpp             include/Coin.h
        src/Block.cpp            include/Block.h
        src/PowerUp.cpp          include/PowerUp.h
        src/Spring.cpp           include/Spring.h
        src/Hazard.cpp           include/Hazard.h
        src/MovingPlatform.cpp   include/MovingPlatform.h
        src/Endpoint.cpp         include/Endpoint.h
        src/goomba.cpp           include/goomba.h
        include/LevelMap.h
        include/LevelData.h
        include/Levels.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SonicCS2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/gameView.cpp
        assets.qrc
        include/level.h
        src/level.cpp
        include/RayHit.h


    )
target_include_directories(SonicCS2 PRIVATE include)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SonicCS2 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SonicCS2 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SonicCS2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SonicCS2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SonicCS2)
endif()
set_target_properties(SonicCS2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SonicCS2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SonicCS2)
endif()

