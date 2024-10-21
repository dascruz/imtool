# Executable for all functional tests with list of sources
add_executable(
    ftest-soa
    soa_test.cpp
)

# Library dependencies
target_link_libraries(
    ftest-soa
    PRIVATE
    imgsoa
    GTest::gtest_main
    Microsoft.GSL::GSL
)
