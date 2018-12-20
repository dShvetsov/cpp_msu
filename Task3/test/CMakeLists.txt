project(Test)

add_executable(run_tests
             main_test.cpp
             dummy_test.cpp
             ProductTest.cpp)

target_link_libraries(run_tests shop gtest gmock)

