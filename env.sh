# qsim_vec_state_cpp/env.sh

# Only set aliases if inside this project
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ "$PWD" == "$PROJECT_ROOT"* ]]; then

    alias b="cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build"
    alias br="cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release"
    alias r='b && ./build/my_program'
    alias cln='alias clean="rm -rf build coverage coverage.info"'
    alias tst="ctest --test-dir build --verbose"
    alias tstcv="b && lcov --ignore-errors unused --remove coverage.info '/usr/*' '*/tests/*' '*_deps/googletest-src/*' --output-file coverage.info && genhtml coverage.info --output-directory coverage"

    echo "Project aliases loaded: r, b, br, cln, tst, tstcv"
fi
