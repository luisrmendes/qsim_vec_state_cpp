# qsim_vec_state_cpp/env.sh

echo "Loading Project aliases"

# Build
alias b="cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build"
echo -e "Loaded '\033[1mb\033[0m'\t: Build"

# Iterative build and run
alias r='b && ./build/main'
echo -e "Loaded '\033[1mr\033[0m'\t: Iterative build and run"

# Build Release
alias br="cmake -S . -B build DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release"
echo -e "Loaded '\033[1mbr\033[0m'\t: Build for release"

# Clean Build Artifacts
alias cln="rm -rf build coverage coverage.info"
echo -e "Loaded '\033[1mcln\033[0m'\t: Clean Build Artifacts"

# Run UTs
alias tst="b && ctest --test-dir build --verbose"
echo -e "Loaded '\033[1mtst\033[0m'\t: Run Unit Tests"

# Run coverage
alias tstcv="b && cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON && cmake --build build && ctest --test-dir build && lcov -c -d build -o coverage.info --ignore-errors unused,mismatch && lcov --remove coverage.info '/usr/*' '*/tests/*' '*_deps/googletest-src/*' --output-file coverage.info && genhtml coverage.info --output-directory coverage"
echo -e "Loaded '\033[1mtstcv\033[0m'\t: Run Unit Tests coverage"

