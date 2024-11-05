add_executable(ftest-aos aos_test.cpp)
target_link_libraries(ftest-aos PRIVATE imgaos GTest::gtest_main Microsoft.GSL::GSL)
