if (ENABLE_LINTING)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)

    if (CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY
            ${CLANG_TIDY_EXE};
            -extra-arg=-std=c++20
        )
    endif()
endif()
