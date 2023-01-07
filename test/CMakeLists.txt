
add_executable(smem_test main.cpp compile_tests/fixed_string.cpp compile_tests/smem_c_interface.cpp)

target_link_libraries(smem_test eco::smem)
