#!/bin/bash
# Profile PCem CPU execution - run this from your desktop terminal
# Generates: perf-cpu.data, perf-cpu-report.txt, perf-cpu-flamegraph.svg (open in browser)

set -e
cd "$(dirname "$0")"

PERF_DATA="perf.data"
PERF_REPORT="perf-report.txt"
FLAMEGRAPH_SVG="perf-flamegraph.svg"
FLAMEGRAPH_DIR="/home/acakar/src/FlameGraph"

echo "=== PCem CPU Profiler ==="
echo "Starting PCem under perf - CPU cycle sampling with call-graphs"
echo ""
echo "Outputs (in $(pwd)):"
echo "  $PERF_DATA          - raw perf data"
echo "  $PERF_REPORT        - text breakdown (hot functions)"
echo "  $FLAMEGRAPH_SVG     - interactive flamegraph (open in browser)"
echo ""
echo "Use PCem normally. Profiling stops when you exit PCem."
echo ""

# Profile CPU cycles with DWARF call-graph unwinding
# -F 999: sample at 999 Hz (below 1000 to avoid throttle)
# --call-graph dwarf,16384: record call stacks via DWARF, 16KB per sample
perf record \
    -F 999 \
    --call-graph dwarf,16384 \
    -o "$PERF_DATA" \
    ./build/src/pcem "$@"

echo ""
echo "=== Profiling complete. Generating reports... ==="

# 1. Text report — hot functions, no children merged
echo "--- Generating text report ---"
perf report -i "$PERF_DATA" --stdio --no-children > "$PERF_REPORT" 2>&1

# 2. Flamegraph — interactive SVG for browser viewing
echo "--- Generating flamegraph ---"
perf script -i "$PERF_DATA" | \
    "$FLAMEGRAPH_DIR/stackcollapse-perf.pl" 2>/dev/null | \
    "$FLAMEGRAPH_DIR/flamegraph.pl" \
        --title "PCem CPU Profile" \
        --width 1200 \
        --minwidth 0.5 \
        --colors java \
        > "$FLAMEGRAPH_SVG" 2>/dev/null

echo ""
echo "=== Done! ==="
echo ""
echo "Quick overview (top CPU consumers):"
head -40 "$PERF_REPORT"
echo ""
echo "Open the flamegraph in your browser:"
echo "  firefox $FLAMEGRAPH_SVG"
echo "  or just double-click the file in your file manager"
echo ""
echo "For more detail:"
echo "  perf report -i $PERF_DATA              # interactive TUI"
echo "  perf annotate -i $PERF_DATA             # hot loops annotated"
