# qsim_vec_state_cpp/env.sh

# Only set aliases if inside this project
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ "$PWD" == "$PROJECT_ROOT"* ]]; then

    alias b="cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build"
    alias br="cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release"
    alias r='b && ./build/my_program'
    alias cln="rm -rf build coverage coverage.info"
    alias tst="ctest --test-dir build --verbose"
    alias tstcv="cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON && cmake --build build && ctest --test-dir build && lcov -c -d build -o coverage.info --ignore-errors unused,mismatch && lcov --remove coverage.info '/usr/*' '*/tests/*' '*_deps/googletest-src/*' --output-file coverage.info && genhtml coverage.info --output-directory coverage"

    echo "Project aliases loaded: r, b, br, cln, tst, tstcv"
fi
