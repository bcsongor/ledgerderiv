cmake_minimum_required(VERSION 3.26)

project(ledgerderiv C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)

set(TARGET_NAME ledgerderiv)
set(TARGET_SOURCES
        main.c
        sleep.h
        bytebuf.h
        hdpath.h
        hdpath.c
        hidutils.h
        hidutils.c
        apdu.h
        apdu.c
        ledger.h
        ledger.c
        ledgereth.h
        ledgereth.c
        luainterop.h
        luainterop.c)
list(TRANSFORM TARGET_SOURCES PREPEND src/)

add_executable(${TARGET_NAME} ${TARGET_SOURCES})
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scripts $<TARGET_FILE_DIR:${TARGET_NAME}>/scripts)

find_package(hidapi CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE hidapi::hidapi hidapi::include)

find_package(Lua REQUIRED)
target_include_directories(${TARGET_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE ${LUA_LIBRARIES})

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(ledgerderiv PUBLIC /ZI /W4 /WX)
    target_link_options(ledgerderiv PUBLIC /INCREMENTAL)
endif ()

MESSAGE(STATUS "Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
MESSAGE(STATUS "Using release flags: ${CMAKE_C_FLAGS_RELEASE}")
