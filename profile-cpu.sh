#!/bin/bash
# Profile PCem CPU execution - run this from your desktop terminal
# Generates: perf.data, perf-report.txt

set -e
cd "$(dirname "$0")"

PERF_DATA="perf.data"
PERF_REPORT="perf-report.txt"

cmake -B build -S . -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --clean-first -j 18

# Profile CPU cycles with DWARF call-graph unwinding
perf record -g \
    -F 999 \
    --call-graph dwarf,16384 \
    -o "$PERF_DATA" \
    ./build/src/pcem "$@"

# Standard human-readable report with call graphs and source lines
perf report --stdio \
    --no-children \
    --full-source-path \
    -i "$PERF_DATA" > "$PERF_REPORT"

# To annotate a specific hot function interactively with C source code:
# perf annotate -i "$PERF_DATA" --source <function_name>
