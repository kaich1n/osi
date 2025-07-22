enable_language(CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Gather all test source files
file(GLOB TEST_SOURCES *.cpp)

# Include parent and project include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Build AllocationTestHarness as a static library
add_library(AllocationTestHarness STATIC AllocationTestHarness.cpp)
target_link_libraries(AllocationTestHarness PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main)

# Add C source for array_test
add_library(array_test_c OBJECT array_test.c)

# Build array_test executable with both C and C++ sources
add_executable(array_test array_test.cpp $<TARGET_OBJECTS:array_test_c>)
set_target_properties(array_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(array_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME array_test COMMAND array_test)

# Add C source for hash_map_test
add_library(hash_map_test_c OBJECT hash_map_test.c)

# Build hash_map_test executable with both C and C++ sources
add_executable(hash_map_test hash_map_test.cpp $<TARGET_OBJECTS:hash_map_test_c>)
set_target_properties(hash_map_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(hash_map_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME hash_map_test COMMAND hash_map_test)

# Add C source for config_test
add_library(config_test_c OBJECT config_test.c)

# Build config_test executable with both C and C++ sources
add_executable(config_test config_test.cpp $<TARGET_OBJECTS:config_test_c>)
set_target_properties(config_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(config_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME config_test COMMAND config_test)

# Add C source for future_test
add_library(future_test_c OBJECT future_test.c)

# Build future_test executable with both C and C++ sources
add_executable(future_test future_test.cpp $<TARGET_OBJECTS:future_test_c>)
set_target_properties(future_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(future_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME future_test COMMAND future_test)

# Add C source for ringbuffer_test
add_library(ringbuffer_test_c OBJECT ringbuffer_test.c)

# Build ringbuffer_test executable with both C and C++ sources
add_executable(ringbuffer_test ringbuffer_test.cpp $<TARGET_OBJECTS:ringbuffer_test_c>)
set_target_properties(ringbuffer_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(ringbuffer_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME ringbuffer_test COMMAND ringbuffer_test)

# Add C source for eager_reader_test
add_library(eager_reader_test_c OBJECT eager_reader_test.c)

# Build eager_reader_test executable with both C and C++ sources
add_executable(eager_reader_test eager_reader_test.cpp $<TARGET_OBJECTS:eager_reader_test_c>)
set_target_properties(eager_reader_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(eager_reader_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME eager_reader_test COMMAND eager_reader_test)

# Add C source for data_dispatcher_test
add_library(data_dispatcher_test_c OBJECT data_dispatcher_test.c)

# Build data_dispatcher_test executable with both C and C++ sources
add_executable(data_dispatcher_test data_dispatcher_test.cpp $<TARGET_OBJECTS:data_dispatcher_test_c>)
set_target_properties(data_dispatcher_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(data_dispatcher_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME data_dispatcher_test COMMAND data_dispatcher_test)

# Add C source for allocation_tracker_test
add_library(allocation_tracker_test_c OBJECT allocation_tracker_test.c)

# Build allocation_tracker_test executable with both C and C++ sources
add_executable(allocation_tracker_test allocation_tracker_test.cpp $<TARGET_OBJECTS:allocation_tracker_test_c>)
set_target_properties(allocation_tracker_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(allocation_tracker_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME allocation_tracker_test COMMAND allocation_tracker_test)

# Remove allocation_tracker_test.cpp from TEST_SOURCES so it is not built twice
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/allocation_tracker_test.cpp")

# Add C source for fixed_queue_test
add_library(fixed_queue_test_c OBJECT fixed_queue_test.c)

# Build fixed_queue_test executable with both C and C++ sources
add_executable(fixed_queue_test fixed_queue_test.cpp $<TARGET_OBJECTS:fixed_queue_test_c>)
set_target_properties(fixed_queue_test PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)
target_link_libraries(fixed_queue_test PRIVATE ct Threads::Threads GTest::gtest GTest::gtest_main AllocationTestHarness)
add_test(NAME fixed_queue_test COMMAND fixed_queue_test)

# Remove fixed_queue_test.cpp from TEST_SOURCES so it is not built twice
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/fixed_queue_test.cpp")

# Remove the foreach loop for TEST_SOURCES, as all test executables are now handled explicitly. 