cmake_minimum_required(VERSION 3.23)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

# underscores are not allowed in deb package name, but adding underscore to comply with requirements...
project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_TEST "Whether to build Google test" ON)
option(WITH_CLANG_TIDY "use clang-tidy" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WITH_CLANG_TIDY)
  find_program(CLANGTIDY NAMES clang-tidy-15 clang-tidy-14 clang-tidy REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY}")
  message(NOTICE "found and enabled CLANGTIDY=${CLANGTIDY}")
endif()

configure_file(version.h.in version.h)

add_executable(ip_filter ip_filter.cpp)
add_library(ipaddr ipaddr.cpp)

set_target_properties(ip_filter ipaddr PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(ip_filter PRIVATE
    ipaddr
)

if(WITH_TEST)
    find_package(GTest REQUIRED)
    enable_testing()

    add_executable(gtest_ipaddr gtest_ipaddr.cpp)

    set_target_properties(gtest_ipaddr PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(gtest_ipaddr
	GTest::gtest_main
        ipaddr
    )

    gtest_discover_tests(gtest_ipaddr)

    find_program(SH_PROG sh REQUIRED)
    find_program(MD5_PROG md5sum REQUIRED)
    add_test(testmd5 ${SH_PROG} -c "[ \"24e7a7b2270daee89c64d3ca5fb3da1a  -\" = \"$(./ip_filter < ${CMAKE_SOURCE_DIR}/ip_filter.tsv | ${MD5_PROG})\" ]")
endif()


if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT sena73@ya.ru)
include(CPack)
