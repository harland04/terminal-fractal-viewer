cmake_minimum_required(VERSION 3.31)
project(terminal-fractal-viewer)

set(CURSES_NEED_NCURSES TRUE)

find_package(Curses REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)


target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
