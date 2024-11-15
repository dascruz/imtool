add_executable(ftest-soa soa_test.cpp)
target_link_libraries(ftest-soa PRIVATE imgsoa GTest::gtest_main Microsoft.GSL::GSL)
