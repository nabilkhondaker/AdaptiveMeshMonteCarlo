add_executable(ammc_unit_tests
  unit/test_rng.cpp
  unit/test_cholesky.cpp
  unit/test_online_stats.cpp
  unit/test_bs_validation.cpp
)

target_link_libraries(ammc_unit_tests PRIVATE ammc_core)
target_include_directories(ammc_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME unit_tests COMMAND ammc_unit_tests)
