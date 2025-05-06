cmake_minimum_required(VERSION 3.10.0)
project(emulator VERSION 0.1.0 LANGUAGES C)

file(GLOB sources "cpu_modules/*" "external_modules/*")

include_directories(cpu_headers)
add_executable(emulator main.c ${sources})
