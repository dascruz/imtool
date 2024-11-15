add_executable(utest-imgaos one_test.cpp resize_aos_utest.cpp maxlevel_test.cpp metadata_test.cpp)
target_link_libraries(utest-imgaos PRIVATE imgaos GTest::gtest_main Microsoft.GSL::GSL)
