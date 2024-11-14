add_executable(utest-imgsoa one_test.cpp
)
target_link_libraries(utest-imgsoa PRIVATE imgsoa GTest::gtest_main Microsoft.GSL::GSL)
