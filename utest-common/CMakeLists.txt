add_executable(utest-common one_test.cpp)
target_link_libraries(utest-common PRIVATE common GTest::gtest_main Microsoft.GSL::GSL)
