add_executable(utest-imgsoa one_test.cpp
        resize_soa_utest.cpp)
target_link_libraries(utest-imgsoa PRIVATE imgsoa GTest::gtest_main Microsoft.GSL::GSL)
