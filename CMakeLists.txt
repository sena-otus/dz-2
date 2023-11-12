cmake_minimum_required(VERSION 3.23)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy-15")

configure_file(version.h.in version.h)

add_executable(ip_filter ip_filter.cpp)
add_library(split split.cpp)

set_target_properties(ip_filter split PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(ip_filter PRIVATE
    split
)

if(WITH_GOOGLE_TEST)
    find_package(GTest REQUIRED)
    enable_testing()

    add_executable(gtest_split gtest_split.cpp)

    set_target_properties(gtest_split PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(gtest_split
	GTest::gtest_main
        split
    )

    gtest_discover_tests(gtest_split)
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
