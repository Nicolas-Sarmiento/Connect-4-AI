
cmake_minimum_required(VERSION 3.10)
project(Connect4Game CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_library(board_logic 
    src/board/board_state.cpp
    src/agent/agent.cpp
    src/heuristics/heuristics.cpp
    )


target_include_directories(board_logic
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)



add_executable(connect4 src/main.cpp)


target_link_libraries(connect4 PRIVATE board_logic)



enable_testing()


add_executable(run_tests test/test_board_state.cpp)


target_link_libraries(run_tests PRIVATE board_logic)

add_test(NAME BoardStateTests COMMAND run_tests)