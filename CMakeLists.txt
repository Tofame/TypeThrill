cmake_minimum_required(VERSION 3.28)
project(WordTyper)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(fmt)


add_executable(WordTyper
        main.cpp
)
target_link_libraries(WordTyper sfml-graphics sfml-audio fmt)

