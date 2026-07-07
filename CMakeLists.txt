cmake_minimum_required(VERSION 3.5)
project(CyberKill VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLTK REQUIRED)

include_directories(${FLTK_INCLUDE_DIR})

set(SOURCES
    main.cpp
    AppDashboard.cpp
    PrivacyManager.cpp
    PrivacySettings.cpp
    UserProfile.cpp
)

add_executable(CyberKill ${SOURCES})

target_link_libraries(CyberKill ${FLTK_LIBRARIES})
