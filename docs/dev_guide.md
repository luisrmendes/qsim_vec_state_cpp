# Dev Guide

## Load Project Aliases
```sh
source env.sh
```

## Build
```sh
cmake -S . -B build && cmake --build build
```

## Clean Build
```sh
rm -rf build && cmake -S . -B build && cmake --build build && ctest --test-dir build
```

## Build with coverage
```sh
rm -rf build && cmake -S . -B build -DENABLE_COVERAGE=ON && cmake --build build && ctest --test-dir build
```

## Generate Coverage
```sh
lcov --remove coverage.info \
    '/usr/*' \
    '*/tests/*' \
    '*_deps/googletest-src/*' \
    --output-file coverage.info  
&& genhtml coverage.info --output-directory coverage
```

## Run Tests
```sh
ctest --test-dir build --verbose
```