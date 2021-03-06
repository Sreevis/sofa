cmake_minimum_required(VERSION 2.8.12)
project(Xitact)


set(HEADER_FILES
    IHPDriver.h
    ITPDriver.h
    PaceMaker.h
    XiTrocarInterface.h
    config.h
)

set(SOURCE_FILES
    IHPDriver.cpp
    ITPDriver.cpp
    PaceMaker.cpp
    initXitact.cpp
)

set(README_FILES PluginXitact.txt)

find_package(SofaGui REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${README_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_XITACTPLUGIN")
target_link_libraries(${PROJECT_NAME} SofaComponentMain SofaGui SofaGuiQt XiRobot)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

install(TARGETS ${PROJECT_NAME}
        COMPONENT Xitact_libraries
        EXPORT XitactTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY examples/ DESTINATION share/sofa/plugins/${PROJECT_NAME})
