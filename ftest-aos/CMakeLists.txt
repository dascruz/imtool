# Executable for all functional tests with list of sources
add_executable(
    ftest-aos
    aos_test.cpp
)

# Library dependencies
target_link_libraries(
    ftest-aos
    PRIVATE
    imgaos
    GTest::gtest_main
    Microsoft.GSL::GSL
)
