add_executable(utest-imgaos one_test.cpp)
target_link_libraries(utest-imgaos PRIVATE imgaos GTest::gtest_main Microsoft.GSL::GSL)
