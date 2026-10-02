cmake_minimum_required(VERSION 3.18)
project(AdaptiveMeshMonteCarloEngine VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(AMMC_ENABLE_TESTS "Build tests" ON)
add_library(ammc_core STATIC
  src/core/version.cpp
  src/random/rng.cpp
  src/linear_algebra/cholesky.cpp
  src/monte_carlo/engine.cpp
  src/utilities/timer.cpp
  src/automatic_differentiation/dual.cpp
  src/stochastic/gbm.cpp
)
target_include_directories(ammc_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(ammc_price apps/price/main.cpp)
target_link_libraries(ammc_price PRIVATE ammc_core)
if(AMMC_ENABLE_TESTS)
  enable_testing()
  add_executable(ammc_unit_tests
    tests/unit/test_main.cpp
    tests/unit/test_rng.cpp
    tests/unit/test_cholesky.cpp
    tests/unit/test_online_stats.cpp
    tests/unit/test_bs_validation.cpp
  )
  target_link_libraries(ammc_unit_tests PRIVATE ammc_core)
  add_test(NAME unit_tests COMMAND ammc_unit_tests)
endif()
message(STATUS "AMMC core configured")
