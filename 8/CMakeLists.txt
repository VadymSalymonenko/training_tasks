cmake_minimum_required(VERSION 3.5)

project(MyProject C)


add_executable(part1 src/part1.c)


add_executable(part2 src/part2.c)


message(STATUS "--------------------------------------------------")
message(STATUS "To run the first part of the task, execute part1.exe. To run the second part, execute part2.exe.")
message(STATUS "--------------------------------------------------")
