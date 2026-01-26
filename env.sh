# qsim_vec_state_cpp/env.sh

# Build
alias b="cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build"
# Iterative build and run
alias r='b && ./build/main'
# Build Release
alias br="cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release"
# Clean Build Artifacts
alias cln="rm -rf build coverage coverage.info"
# Run UTs
alias tst="ctest --test-dir build --verbose"
# Run coverage
alias tstcv="cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON && cmake --build build && ctest --test-dir build && lcov -c -d build -o coverage.info --ignore-errors unused,mismatch && lcov --remove coverage.info '/usr/*' '*/tests/*' '*_deps/googletest-src/*' --output-file coverage.info && genhtml coverage.info --output-directory coverage"

echo "Project aliases loaded: r, b, br, cln, tst, tstcv"
