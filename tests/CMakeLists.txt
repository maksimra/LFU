find_package (GTest)

if (NOT GTest_FOUND)
        message (STATUS "GTest not found, downloading GoogleTest...")
        include (FetchContent)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG release-1.12.1
        )
        set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
endif ()

enable_testing ()

add_executable(
        cache_tests
        tests.cpp
)

target_link_libraries(
        cache_tests
        GTest::gtest_main
)

target_include_directories (cache_tests PUBLIC ${PROJECT_SOURCE_DIR})

include (GoogleTest)

gtest_discover_tests (cache_tests)
