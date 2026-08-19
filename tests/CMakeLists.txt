file(GLOB_RECURSE TEST_SOURCES *.cpp)

add_executable(tachyon_tests ${TEST_SOURCES})

target_link_libraries(tachyon_tests PRIVATE tachyon)

enable_testing()
add_test(NAME TachyonTests COMMAND tachyon_tests)